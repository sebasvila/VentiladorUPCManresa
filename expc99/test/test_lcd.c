#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"


int main() {
  lcd_init();
  lcd_on();
  lcd_clear();
  // for (int i = 0; i < 1000; i++) {
  //   lcd_printf("em dic %s %i","pau", i);
  //   lcd_set_cursor(0,1);
  //   lcd_printf("em dic %s %i","pau", i);
  //   lcd_set_cursor(0,2);
  //   lcd_printf("em dic %s %i","pau", i);
  //   lcd_set_cursor(0,3);
  //   lcd_printf("em dic %s %i","pau", i);
  //   _delay_ms(5);
  //   lcd_return_home();
  // }

  lcd_return_home();
  lcd_disable_autoscroll();
  lcd_set_left_to_right();
  lcd_puts("Owa");
  _delay_ms(500);
  lcd_set_cursor(17,3);
  lcd_puts("Owa");
  _delay_ms(500);

  _delay_ms(10000);

  lcd_off();
  return 0;
}
