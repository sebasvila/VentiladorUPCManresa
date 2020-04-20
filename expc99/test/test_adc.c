#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "pt.h"
#include "ticker.h"
#include "shielditic.h"
#include "adc.h"

static uint16_t offset;

/*
 * Runs a rotating led at periods of `offset`.  `offset` cannot be 0,
 * otherwise thread do not yields never and becomes non-collaborative,
 * starvating other threads.
 */
PT_THREAD(sem(struct pt *pt))
{
  static uint16_t chronos;

  PT_BEGIN(pt);

  for(;;) {
    led_toggle(semaph1, green);
    /* 
     * To cope with ticker overflow: compare always duration, not
     * timestamp
     */
    chronos = ticker_get();
    PT_WAIT_WHILE(pt, ticker_get() - chronos <= offset);
    led_toggle(semaph1, green);
    
    led_toggle(semaph1, yellow);
    chronos = ticker_get();
    PT_WAIT_WHILE(pt, ticker_get() - chronos <= offset);
    led_toggle(semaph1, yellow);
    
    led_toggle(semaph1, red);
    chronos = ticker_get();
    /* a `<=` guarantees that thread yields although offset == 0 */
    PT_WAIT_WHILE(pt, ticker_get() - chronos <= offset);
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
    adc_start_conversion(1);
    PT_WAIT_WHILE(pt, adc_converting());
    offset = adc_get() / ((1<<10)*2/ticker_tps());

    /* polling time of potentiometer */
    chronos = ticker_get();
    PT_WAIT_WHILE(pt, ticker_get()-chronos < ticker_tps()/10u);
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
  offset = adc_wait_get(1)/((1<<9)*2/(ticker_tps()));
  
  /* do schedule of threads */
  for(;;) {
    (void)PT_SCHEDULE(sem(&sem_ctx));
    (void)PT_SCHEDULE(pot(&pot_ctx));
  }
}
