#ifndef _MOTOR_H_
#define _MOTOR_H_


typedef enum {m200, m400, m800, m1600} motor_stepsperturn_t;
typedef enum {motor_a, motor_b} motor_dir_t;


void motor_setup(motor_stepsperturn_t s);

void motor_steps(motor_dir_t d, uint16_t steps);

void motor_step(void);

void motor_set_dir(motor_dir_t d);

void motor_reverse(void);

#endif
