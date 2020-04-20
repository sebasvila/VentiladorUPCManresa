#ifndef _ADC_H_
#define _ADC_H_

#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>




/*
 * Abstracts an analogic signal input
 */


/* initialize the module. Reference voltage = 5V */
void adc_setup(void);

/* read from channel `ch`, which corresponds to PORTC `ch` pin. Blocks
   until read done (about 1ms). Max value = 2^10 */
uint16_t adc_wait_get(uint8_t ch);

/* non-blocking read set of calls. Should be used with utmost care
 * because hardware do not allows simultaneously reading for more than
 * one channel. Thus, during reading time no other read should be
 * issued.
 */
void adc_start_conversion(uint8_t ch);
bool adc_converting(void);
uint16_t adc_get(void);



#endif
