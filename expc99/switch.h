#ifndef _SWITCH_H_
#define _SWITCH_H_


#include <stdint.h>

/* `switch_t` is a handler to internal switch */
typedef int8_t switch_t;

/* Some functions can return error */
#define SWITCH_ERR ((switch_t)-1)





/* setup the module */
void switch_setup(void);

/* 
 * Binds a switch object to a port+pin.
 * All query operations require a switch to be bound.
 * Returns `SWITCH_ERR` if no more switches allowed 
 */
switch_t switch_bind(volatile uint8_t *port, uint8_t pin);
/* unbinds a switch object */
void switch_unbind(switch_t i);


/*
 * Non-blocking queries
 */
void switch_ask(switch_t i);
bool switch_ready(switch_t i);
bool switch_get(switch_t i);
bool switch_changed(switch_t i);


/*
 * Blocking queries 
 */
bool switch_get_blk(switch_t i);
bool switch_changed_blk(switch_t i);

#endif
