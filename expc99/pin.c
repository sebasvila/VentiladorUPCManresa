#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include <stdio.h>
#include "pin.h"



/* static void disable_all_pullups(void) { */
/*   MCUCR |= _BV(PUD); */
/* } */


pin_t pin_bind(volatile uint8_t *port, uint8_t pin, pin_direction_t d) {
  pin_t p;
  
  // Setup pin `p`
  p.port = port;
  p.pin = pin;
  // set port pin direction
  switch (d) {
  case Output:
    DDR(port) |= _BV(pin);
    // set port pin low  	
    PORT(port) &= ~_BV(pin);
    break;
  case Input:
    DDR(port) &= ~_BV(pin);
    break;
  case InputPullup:
    DDR(port) &= ~_BV(pin);
    // set port pin high (activate pull up)
    PORT(port) |= _BV(pin);
    break;
  }
  return p;
}

inline void pin_set_true(pin_t p);

inline void pin_set_false(pin_t p);

inline bool pin_r(pin_t p);

void pin_toggle(pin_t p);


void pin_w(pin_t p, bool v) {
  if (v) 
    pin_set_true(p);
  else
    pin_set_false(p);
}


void pin_unbind(pin_t *const p) {
  DDR(p->port) &= ~_BV(p->pin);
  PORT(p->port) &= ~_BV(p->pin);
  p->port = NULL;
}

