#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "queue.h"
#include "serial.h"

#define NUM (F_CPU/16)
#define BAUDRATE_L(x) UINT8_C((NUM/x-1)      & 0xff)
#define BAUDRATE_H(x) UINT8_C((NUM/x-1) >> 8 & 0xf)


/*
 * Input and output queues
 */
static queue_t inq;   // For received data  
static queue_t outq;  // For data to be sent


/*
 * ISR's
 */
ISR(USART_UDRE_vect) {
  // USART ready to transmit a byte
  if (queue_is_empty(&outq)) {
    // No data to send => disable UDRE interrupt (19.6.3)
    UCSR0B &= ~_BV(UDRIE0);
  } else {
    UDR0 = queue_front(&outq);
    queue_dequeue(&outq);
  }
}


ISR(USART_RX_vect) {
  // USART byte received: enqueue or discard
  if (queue_is_full(&inq)) {
    // No space left in queue => discard
    uint8_t tmp __attribute__ ((unused)) = UDR0; 
  } else {
    queue_enqueue(&inq, UDR0);
  }
}


void serial_init(void) {
  // Initialize the queues
  queue_empty(&inq);
  queue_empty(&outq);

  // Initialize the UART0
  // According to 19.5 we must wait last transmision finishes
  // and all data received... ignoring it
  // According to 19.5 interrupts must be disabled and restored at end
  UBRR0H = BAUDRATE_H(9600);
  UBRR0L = BAUDRATE_L(9600);
  // set normal baud rate 
  UCSR0A = UINT8_C(0);
  UCSR0C = 
    (_BV(UCSZ01)   | _BV(UCSZ00)) &   // 8 bit frame
    ~_BV(UMSEL01) & ~_BV(UMSEL00) &   // asincronous mode
    ~_BV(UPM01)   & ~_BV(UPM00)   &   // no parity
    ~_BV(USBS0)   ;                   // 1 stop bit
  // Enable transmision and reception
  UCSR0B = _BV(RXEN0) | _BV(TXEN0);
  // Enable interrupts only for receiving
  UCSR0B |= _BV(RXCIE0);
}



uint8_t serial_get(void) {
  while (queue_is_empty(&inq)); // better put to sleep
  uint8_t r = queue_front(&inq);
  queue_dequeue(&inq);
  return r;
}


bool serial_can_read(void) {
  // test whether there is something to read
  return !queue_is_empty(&inq);
}


/*
 * Enqueue data to be sent. Blocks if queue is full.
 */
void serial_put(uint8_t c) {
  while (queue_is_full(&outq)); // better put to sleep
  queue_enqueue(&outq, c);
  // Once there is data, it can transmit => activate interrupts
  UCSR0B |= _BV(UDRIE0);
}

