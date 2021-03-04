#ifndef BLINK_H
#define BLINK_H

//Blinking frequency
#define BLINK_FREQ 2                //The number of display refresh loops it takes to blink
#define UNBLINK_FREQ BLINK_FREQ*2   //Same loops to unblink

void update_blink_counter(void);
void reset_blink_counter(void);
void reset_unblink_counter(void);
bool is_blink_time();
bool is_unblink_time();

#endif
