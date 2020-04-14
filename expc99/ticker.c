#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "ticker.h"

/* Implemented on the 8.bit TIMER2 */

static volatile uint16_t ticks;


void ticker_setup(void) {
  // Configure timer to mode CTC, no output, and no clock
  TCCR2A = _BV(WGM20) | _BV(WGM21);
  TCCR2B = _BV(WGM22);
  // No interrupts from timer
  TIMSK2 = 0;
  // Count
  OCR2A = UINT8_C(125);
}


void ticker_start(void) {
  /* reset counter */
  TCNT2 = 0;
  /* enable interrupts */
  TIMSK2 = _BV(OCIE2A);
  // Select 1024 prescaler and start counting
  TCCR2B |=  _BV(CS22) | _BV(CS21) | _BV(CS20);
}



uint16_t ticker_get(void) {
  return ticks;
}


extern uint16_t ticker_tps(void);


void ticker_stop(void) {
  // No interrupts from timer
  TIMSK2 = 0;
  // Select 0 prescaler and end counting
  TCCR2B &=  ~(_BV(CS22) | _BV(CS21) | _BV(CS20));
}


ISR(TIMER2_COMPA_vect) {
  ticks++;
}
