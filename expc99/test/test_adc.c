#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "pt.h"
#include "ticker.h"
#include "shielditic.h"
#include "adc.h"

static uint16_t offset;

/*
 * Runs a moving semaphore led at offset period
 */
PT_THREAD(sem(struct pt *pt))
{
  static uint16_t chronos;

  PT_BEGIN(pt);

  for(;;) {
    led_toggle(semaph1, green);
    chronos = ticker_get() + offset;
    PT_WAIT_UNTIL(pt, ticker_get() > chronos);
    led_toggle(semaph1, green);
    
    led_toggle(semaph1, yellow);
    chronos = ticker_get() + offset;
    PT_WAIT_UNTIL(pt, ticker_get() > chronos);
    led_toggle(semaph1, yellow);
    
    led_toggle(semaph1, red);
    chronos = ticker_get() + offset;
    PT_WAIT_UNTIL(pt, ticker_get() > chronos);
    led_toggle(semaph1, red);
  }

  PT_END(pt);
}


/*
 * Polls potentiometer at 0.1 s freq and updates `offset`
 * accordingly.
 */
PT_THREAD(pot(struct pt *pt))
{
  static uint16_t chronos;
  
  PT_BEGIN(pt);

  for(;;) {
    /* non-blocking adc read from shield itic potentiometer (ch 1) */
    adc_start_reading(1);
    PT_WAIT_UNTIL(pt, adc_read_finished());
    offset = adc_get_read()/((1<<10)/(ticker_ticks_per_second()));

    /* polling time of potentiometer */
    chronos = ticker_get() + ticker_ticks_per_second()/10;
    PT_WAIT_UNTIL(pt, ticker_get() > chronos);
  }

  PT_END(pt);
}




int main(void) {
  /* context dels threads */
  struct pt sem_ctx, pot_ctx;

  /* init modules */
  ticker_setup();
  ticker_start();
  shielditic_setup();
  adc_setup();
  sei();
  
  /* init contexts */
  PT_INIT(&sem_ctx);
  PT_INIT(&pot_ctx);

  /* read initial position of potentiometer */
  offset = adc_read(1)/((1<<10)/(ticker_ticks_per_second()));
  
  /* do schedule of threads */
  for(;;) {
    (void)PT_SCHEDULE(sem(&sem_ctx));
    (void)PT_SCHEDULE(pot(&pot_ctx));
  }
}
