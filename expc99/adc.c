#include <avr/io.h>





uint16_t adc_read(uint8_t ch)
{
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



void adc_setup(void)
{
    // AREF = AVcc
    ADMUX = _BV(REFS0);
 
    // ADC Enable and prescaler of 128
    // 16000000/128 = 125000
    ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
}








