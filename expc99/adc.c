#include <avr/io.h>
#include <avr/interrupt.h>
#include "adc.h"


/* number of samples in oversampling (2's power) */
#define N_SAMPLES 4

/* remembers last channel used */
static adc_channel last_channel_used;

/*
 * adc_channel internal representation:
 * - bits [0-3] hardware channel number
 * - bits [4-5] reserved
 * - bits [6-7] voltage reference
 *
 *  bit  7 6 5 4 3 2 1 0
 *       R R - - C C C C
 */

/*
 * adc_channel utility macros 
 */
#define G_CH(x) (x & 017)                 /* get channel number */
#define G_RE(x) (x>>6)                    /* get reference voltage */
#define M_CH(x) (x & 017)                 /* masked channel number */
#define M_RE(x) (x & 0300)                /* masked reference voltage */
#define C_ADC(c,r) (c|r<<6)               /* adc_channel constructor */



adc_channel adc_bind(uint8_t ch, adc_ref ref) {
  /* disable digital port if needed */
  if (ch < 6)
    DIDR0 |= _BV(ch);
  /* return external repr of adc channel */
  return C_ADC(ch,ref);
}


void adc_unbind(adc_channel *const ch) {
  /* enable digital port if needed */
  if (*ch < 6)
    DIDR0 &= ~_BV(*ch);
}



void adc_prepare(adc_channel ch) {
  /* test if same enviroment that last conversion */
  if (ch != last_channel_used) {
    if (M_RE(ch) != M_RE(last_channel_used)) {
      /* must change reference source */
      ADMUX = (ADMUX & 077) | M_RE(ch);
      /* wait if needed */
    }
    if (M_CH(ch) != M_CH(last_channel_used)) {
      /* must change physical channel */
      ADMUX = (ADMUX &  0360) | M_CH(ch);
      /* wait if needed */
    } 
    /* update last conversion */
    last_channel_used = ch;
  }
}


void adc_start_conversion(void) {
  /* avoid overreads */
  while (ADCSRA & _BV(ADSC));

  /* start single conversion: write ’1′ to ADSC */
  ADCSRA |= _BV(ADSC);
}


void adc_prepare_start(adc_channel ch) {
  adc_prepare(ch);
  adc_start_conversion();
}

 
bool adc_converting(void) {
  return ADCSRA & _BV(ADSC);
}


uint8_t adc_get(void) {
  return ADCH;  // only 8 higher bits
}
 

uint8_t adc_prep_start_get(adc_channel ch)
{
  adc_prepare_start(ch);
  while (adc_converting());
  return adc_get();
}




/*********************************************************
 * Oversampling read
 *********************************************************/
static uint16_t sample_sum;
static uint8_t num_samples;


void adc_start_oversample(void) {
  sample_sum = 0;
  num_samples = 0;
  /* enable trigger mode and enable interrupts */
  ADCSRA |= _BV(ADATE) | _BV(ADIE);

  /* avoid overreads */
  while (ADCSRA & _BV(ADSC));

  /* start conversions */
  ADCSRA |= _BV(ADSC);
}


bool adc_oversampling(void) {
  return num_samples != N_SAMPLES;
}


uint8_t adc_get_oversample(void) {
  /* divide and round */
  if( (sample_sum & (N_SAMPLES-1)) >= N_SAMPLES/2)
    return sample_sum / N_SAMPLES + 1;
  else
    return sample_sum / N_SAMPLES;
}


ISR(ADC_vect) {
  sample_sum += ADCH;
  if (++num_samples == N_SAMPLES) {
    /* no more sampling */
    /* disable trigger mode and disable interrupts */
    ADCSRA &= ~(_BV(ADATE) | _BV(ADIE));
  }
}




void adc_setup(void) {
  /* Registre DIDR0 s'ha de usar en els canals emprats */
  /* Mes estable 3.3V que els Vcc per que alimentacio no estable */
  /* potenciometre usa Vcc -> com canviar-ho per canal? */
  
  /* disable power reduction for ADC */
  PRR &= ~_BV(PRADC);
  /* ADC Enable and prescaler of 64
   * 16000000/32 = 500000 kHz. 
   * typical conversion time 13 cycles = 13*500000^-1 = 26 us
   */
  ADCSRA = _BV(ADPS2) | _BV(ADPS0) ;
  /* want only 8 bits resolution: shift reading left */
  ADMUX = _BV(ADLAR);
  /* ADC enable */
  ADCSRA |= _BV(ADEN);
  /* set trigger source to interrupt flag (for oversampling) */
  ADCSRB &= ~(_BV(ADTS2) | _BV(ADTS1) | _BV(ADTS0));
  /* set module state: last adc_channel converted is none */
  last_channel_used = 0x0;
  /* force and discard very first read.  This sets up
   * `last_channel_used`, and waits for first unusually long reading
   * time (25 cycles, pp. 208 datasheet).  Force to select an
   * yet unused channel and reference voltage.
   * As a lateral effect it sets up the `last_channel_used` private
   * module attribute.
   * use internal channel to avoid potential conflicts.
   */
  (void)adc_prep_start_get(C_ADC(ADC_CHANNEL_11V,Int11));
}








