#include "i2cr.h"
#include "i2cq.h"
#include "i2c.h"




static i2cq_t requests;


void i2c_setup(void) {
  i2cq_empty(&requests);
}


void i2c_open(void) {
}


void i2c_close(void) {
}


void i2c_send(i2cr_addr_t node,
	      const uint8_t *const  buffer,
	      uint8_t length,
	      volatile i2cr_status_t *const status) {
  i2cr_request_t r =
    {
     .rt = I2Csend,
     .node = node,
     .send_buffer = buffer,
     .send_length = length,
     .status = status,
    };

  while (i2cq_is_full(&requests));
  i2cq_enqueue(&requests, &r);
}



void i2c_receive(i2cr_addr_t node,
		 uint8_t *const  buffer,
		 uint8_t length,
		 volatile i2cr_status_t *const status) {
  i2cr_request_t r =
    {
     .rt = I2Creceive,
     .node = node,
     .receive_buffer = buffer,
     .receive_length = length,
     .status = status,
    };

  while (i2cq_is_full(&requests));
  i2cq_enqueue(&requests, &r);
}


void i2c_sandr(i2cr_addr_t node,
	       const uint8_t *const  s_buffer,
	       uint8_t s_length,
	       uint8_t *const  r_buffer,
	       uint8_t r_length,
	       volatile i2cr_status_t *const status) {
  i2cr_request_t r =
    {
     .rt = I2Csendrec,
     .node = node,
     .send_buffer = s_buffer,
     .send_length = s_length,
     .receive_buffer = r_buffer,
     .receive_length = r_length,
     .status = status,
    };

  while (i2cq_is_full(&requests));
  i2cq_enqueue(&requests, &r);
}



bool i2c_swamped(void) {
  return i2cq_is_full(&requests);
}



void i2c_send_b(i2cr_addr_t node,
		uint8_t b,
		volatile i2cr_status_t *const status) {};


void i2c_send_bb(i2cr_addr_t node,
		 uint16_t b,
		 volatile i2cr_status_t *const status) {};

void i2c_receive_b(i2cr_addr_t node,
		   uint8_t b,
		   volatile i2cr_status_t *const status) {};

void i2c_receive_bb(i2cr_addr_t node,
		    uint16_t b,
		    volatile i2cr_status_t *const status) {};




/********************************************************
 * Interrupt driven master automata
 ********************************************************/


typedef enum {
	      Idle,
	      WaitStartDone,
	      WaitSlaveHit,
	      End
} ida_state;


typedef enum {
	      GoOperative,
	      StartSent,
	      SlaveAddressSent,
	      
	      ErroneousCoditionReceived
} ida_event;


static void ida_next(ida_event e) {
  static i2cr_request_t current_req; // could be a pointer?

  
  switch (ida_state) {
  case Idle:
    /* In idle state. Interrupts disabled */
    if (e == GoOperative) {
      // get next request, must exist for sure
      i2cq_front(&requests, &current_req);
      i2cq_dequeue(&requests);
      // Throw a START to get the bus control
      enable_i2c_interrupts();
      throw_start();
      // next state
      ida_state = WaitStartDone;
    } else {
      // remain Iddle
    }
    break;

  case WaitStartDone:
    /* START sent, waiting it to finish */
    if (e = StartSent) {
      // The bus is available, begin messaging a node
      throw_byte(current_req.node |
		 (current_req.rt != I2Creceive)?(0):(1)
		 );
      // next state
      ida_state = WaitSlaveHit;
    } else {
      // if other event arrives, its an error
      error();
    }
    break;

  case WaitSlaveHit:
    break;
  }
}


	     
