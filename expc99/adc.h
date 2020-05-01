#ifndef _ADC_H_
#define _ADC_H_

/*
 * Abstracts an analogic signal input
 *
 * Rationale.
 * Uses the 'file' abstraction like other hardware interface modules.
 */
#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>

/* Max value returned by adc module conversion */
#define ADC_MAX 0xff

/* Special channels */
#define ADC_CHANNEL_0V  017
#define ADC_CHANNEL_11V 016


/* Traslate adc value. Needs more work... */
#define ADC_VALUE(v,M,m) \
  ((int32_t)m + ((int32_t)M-(int32_t)m)*(int32_t)v/(int32_t)ADC_MAX)

/* 
 * Reference voltage source.
 * If Aref is connected, other sources should be avoided
 * (¶23.5.2 pp. 211 datasheet)
 */
typedef enum {Aref=0, Vcc=1, Int11=3} adc_ref;



/* An adc channel proxy object */
typedef uint8_t adc_channel;



/* Bind channel to a given port and reference voltage source */
adc_channel adc_bind(uint8_t ch, adc_ref ref);

/* Unbind channel and */
void adc_unbind(adc_channel *const ch);
		     

/* non-blocking read set of calls. Should be used with utmost care
 * because hardware do not allows simultaneously reading for more than
 * one physical channel. Thus, during conversion time no other read
 * should be issued.
 */

/* prepare to convert from channel `ch` */
void adc_prepare(adc_channel ch);

/* start to read from last prepared channel */
void adc_start_conversion(void);

/* prepare to convert from channel `ch` and start a conversion */
void adc_prepare_start(adc_channel ch);

/* true iff last started conversion is running */
bool adc_converting(void);

/* get the value of the last started conversion */
uint8_t adc_get(void);



/* read from adc channel `ac` until read done.  Typically about 30us
 * of waiting time. Longuer waiting time if it needs to change
 * physical channel and/or reference voltage from last conversion.
 */
uint8_t adc_prep_start_get(adc_channel ch);





/*
 * Adjust channel range.
 * When Vcc external reference is used, this function can be used to
 * calibrate against the internal 1.1V reference. The function returns
 * the real voltage corresponding to Vcc. 
 */
// float adc_adjust(void); not implemented yet


/*
 * Oversampling conversion.
 * Four continuous measures of the same channel are made and the 
 * mean returned. Measuring span is of 13.5*4=54 cycles, approx.
 * 108 us.
 * Function blocks until read done.
 */
//uint8_t adc_oversample(adc_channel ch);
//unimplemented yet


/* initialize the module */
void adc_setup(void);

#endif
