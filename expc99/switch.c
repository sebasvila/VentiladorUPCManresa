#include <stdint.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "switch.h"

#include "shielditic.h"
#include <util/delay.h> 

/* max allowed number of switches */
#define MAX_SWITCH 8

/* 
 * sampling window mask.length (max 8) 
 * A number of 1's followed by a number of 0's
 * Number of 0's is tha samplin window length
 */
#define WINDOW_MASK (0xf8)


#define PORT(x) (*(x))
#define DDR(x)  (*(x-1))   // consider DDRy = PORTy - 1
#define PIN(x)  (*(x-2))   // consider PINy = PORTy - 2


/**************************************************
 * Private switch table and its operations
 **************************************************/

/* internal switch representation */
typedef struct {
  volatile uint8_t *port;
  uint8_t pin_mask;
  uint8_t d_history;        // debouncing history
  unsigned int used:1;      // 0 = slot not used
  unsigned int sampling:1;  // 0 = not sampling this switch
  unsigned int state:1;     // 0 = low
  unsigned int changed:1;   // 0 = not changed from last query
} sw_t;

/* table of currently bound switches */
volatile static struct {
  sw_t    t[MAX_SWITCH];
  uint8_t on_sampling;
}swt;

static void empty_table(void) {
  for (uint8_t i=0; i<MAX_SWITCH; i++) {
    swt.t[i].used     = 0;
    swt.t[i].sampling = 0;
  }
  swt.on_sampling = 0;
}

static int8_t get_free_slot(void) {
  for (uint8_t i = 0; i < MAX_SWITCH; ++i) {
    if (!swt.t[i].used) return i;
  }
  return -1;
}


/**************************************************
 * Private timer operations
 **************************************************/


static void timer_setup(void) {
  // Configure timer to mode CTC, no output
  TCCR0A = _BV(WGM01);
  TCCR0B = 0;
  // No interrupts from timer
  TIMSK0 = 0;
  // Select 1024 prescaler and start counting
  TCCR0B |=  _BV(CS02) | _BV(CS00);
  // Count
  OCR0A = UINT8_C(156); // almost 10 ms
}


static void enable_timer(void) {
  /* reset counter */
  TCNT0 = 0;
  /* enable interrupts */
  TIMSK0 = _BV(OCIE0A);
}


static void disable_timer(void) {
  // No interrupts from timer
  TIMSK0 = 0;
}


ISR(TIMER0_COMPA_vect) {
  /* debouncing active switches */
  for (uint8_t i=0; i<MAX_SWITCH; i++) {
    if (swt.t[i].sampling) {
      /* get current sampling, LOW = on */
      bool state_high = !(PIN(swt.t[i].port) & swt.t[i].pin_mask);
      swt.t[i].d_history =
	(swt.t[i].d_history << 1) | (state_high & 1);
      /* check if stable and proceed */
      if ((swt.t[i].d_history | WINDOW_MASK) == 0xff) {
	/* stable and high */
	swt.t[i].changed = (swt.t[i].state ^ 1);	
	swt.t[i].state = 1;
	swt.t[i].sampling = 0;
	if (--swt.on_sampling ==  0) disable_timer();
      } else if ((swt.t[i].d_history & ~WINDOW_MASK) == 0x0) {
	/* stable and low */
	swt.t[i].changed = (swt.t[i].state & 1);
	swt.t[i].state = 0;
	swt.t[i].sampling = 0;
	if (--swt.on_sampling ==  0) disable_timer();
      } else {
	/* yet unstable */
      }
    }
  }
}





switch_t switch_bind(volatile uint8_t *port, uint8_t pin) {
  /* allocate a slot */
  int8_t i = get_free_slot();
  if (i<0) return SWITCH_ERR;
  /* initialize the slot */
  swt.t[i].used = 1;
  swt.t[i].sampling = 0;
  swt.t[i].state = 0;         // assume switch was low
  swt.t[i].changed = 0;
  swt.t[i].port = port;
  swt.t[i].pin_mask  = _BV(pin);
  /* configure port as input with pullup active */
  DDR(port)  &= ~_BV(pin);
  PORT(port) |=  _BV(pin);  // pull up active

  return i;
}
  

void switch_unbind(switch_t i) {
  /* check if bound */
  if (!swt.t[i].used) return;
  /* configure port as output in low state */
  DDR(swt.t[i].port)  |=  swt.t[i].pin_mask ; // NEEDS CHECK
                                              // possible shortcircuit?
  PORT(swt.t[i].port) &= ~swt.t[i].pin_mask;
  /* 
   * if in sampling mode: deactivate 
   * ATOMIC block because timer interrupt can alter preconditions
   * if enters after `if`.
   * Forceon because if this module is running, timer should be
   * and thus global interrupts enabled.
   * Would be clever to disable only timer.
   */
  ATOMIC_BLOCK(ATOMIC_FORCEON) {
    if (swt.t[i].sampling) {
      // force invariant: not used => not sampling
      swt.t[i].sampling = 0; 
      if (--swt.on_sampling == 0) disable_timer();
    }
  }
  /* free slot */
  swt.t[i].used = 0;
}


/*
 * Non-blocking queries
 */

void switch_ask(switch_t i) {
  swt.t[i].sampling = 1;
  if (swt.on_sampling == 0) enable_timer();
  swt.on_sampling++;
}

bool switch_ready(switch_t i) {
  return swt.t[i].sampling == 0;
}

bool switch_get(switch_t i) {
  return swt.t[i].state;
}

bool switch_changed(switch_t i) {
  return swt.t[i].changed;
}


/*
 * Blocking queries 
 */

bool switch_get_blk(switch_t i) {
  switch_ask(i);
  while (!switch_ready(i));
  return switch_get(i);
}

bool switch_changed_blk(switch_t i) {
  switch_ask(i);
  while (!switch_ready(i));
  return switch_changed(i);
}



void switch_setup(void) {
  empty_table();
  timer_setup();
}
