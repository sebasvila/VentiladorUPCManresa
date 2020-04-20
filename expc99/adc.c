#include <avr/io.h>
#include "adc.h"


uint16_t adc_read(uint8_t ch)
{
  /* avoid overreads */
  while (ADCSRA & _BV(ADSC));
	 
  // select the corresponding channel 0~7
  // ANDing with ’7′ will always keep the value
  // of ‘ch’ between 0 and 7
  ch &= 0x07;  // AND operation with 7
  ADMUX = (ADMUX & 0xF8) | ch; // clears the bottom 3 bits before ORing
 
  // start single convertion
  // write ’1′ to ADSC
  ADCSRA |= (1<<ADSC);
 
  // wait for conversion to complete
  // ADSC becomes ’0′ again
  // till then, run loop continuously
  while (ADCSRA & (1<<ADSC));
 
  return ADC;
}



void adc_start_reading(uint8_t ch) {
  /* avoid overreads */
  while (ADCSRA & _BV(ADSC));

  // select the corresponding channel 0~7
  // ANDing with ’7′ will always keep the value
  // of ‘ch’ between 0 and 7
  ch &= 0x07;  // AND operation with 7
  ADMUX = (ADMUX & 0xF8) | ch; // clears the bottom 3 bits before ORing
 
  // start single convertion
  // write ’1′ to ADSC
  ADCSRA |= _BV(ADSC);
}

 
bool adc_read_finished(void) {
  return !(ADCSRA & _BV(ADSC));
}


uint16_t adc_get_read(void) {
  return ADC;
}
 

void adc_setup(void) {
  /* disable power reduction for ADC */
  PRR &= ~_BV(PRADC);

  // AREF = AVcc
  ADMUX = _BV(REFS0);
 
  // ADC Enable and prescaler of 128
  // 16000000/128 = 125000
  ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
}








