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
uint16_t adc_read(uint8_t ch);

/* non-blocking read set of calls. Should be used with utmost care
 * because hardware do not allows simultaneously reading for more than
 * one channel. Thus, during reading time no other read should be
 * issued.
 */
void adc_start_reading(uint8_t ch);
inline bool adc_read_finished(void) {return ADCSRA & _BV(ADSC);}
inline uint16_t adc_get_read(void) {return ADC;}

#endif
