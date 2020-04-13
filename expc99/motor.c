#include <util/delay.h>
#include "pin.h"
#include "motor.h"


typedef enum {m200, m400, m800, m1600} motor_stepsperturn_t;
typedef enum {motor_a, motor_b} motor_dir_t;


static pin_t led;


void motor_setup(motor_stepsperturn_t s) {
  led = pin_bind(PORTB, 5, Output);
}

void motor_steps(motor_dir_t d, uint16_t steps) {
}

void motor_step(void) {
  pin_toggle(led);
  _delay_us(1);
  pin_toggle(led);
}

void motor_set_dir(motor_dir_t d) {

}

void motor_reverse(void) {

}
