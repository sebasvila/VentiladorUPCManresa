#include <stdbool.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "pin.h"





int main() {
  const pin_t led = pin_bind(&PORTB, 5, Output);

  //  _delay_ms(1000);
  pin_set_true(led);
  //  _delay_ms(1000);
  pin_set_false(led);

  return 0;
}
  

  
