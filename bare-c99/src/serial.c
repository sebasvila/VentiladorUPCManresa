#include <stdlib.h>
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "queue.h"
#include "serial.h"

#define NUM (F_CPU/16)
#define BAUDRATE_L(x) UINT8_C((NUM/x-1)      & 0xff)
#define BAUDRATE_H(x) UINT8_C((NUM/x-1) >> 8 & 0x0f)


/*
 * Input and output queues
 */
#ifdef _SER_RX_
static queue_t inq;   // For received data
#endif
#ifdef _SER_TX_
static queue_t outq;  // For data to be sent
#endif

/*
 * ISR's
 */

#ifdef _SER_RX_
ISR(USART_RX_vect) {
  // USART byte received: enqueue or discard
  if (queue_is_full(&inq)) {
    // No space left in queue => discard
    uint8_t tmp __attribute__ ((unused)) = UDR0; 
  } else {
    queue_enqueue(&inq, UDR0);
  }
}
#endif

#ifdef _SER_TX_
ISR(USART_UDRE_vect) {
  // USART ready to transmit a byte
  if (queue_is_empty(&outq)) {
    // No data to send => disable UDRE interrupt (¶19.6.3)
    UCSR0B &= ~_BV(UDRIE0);
  } else {
    UDR0 = queue_front(&outq);
    queue_dequeue(&outq);
  }
}
#endif



#ifdef _SER_RX_
bool serial_can_read(void) {
  // test whether there is something to read
  return !queue_is_empty(&inq);
}


uint8_t serial_read(void) {
  while (queue_is_empty(&inq)); // better put to sleep
  uint8_t r = queue_front(&inq);
  queue_dequeue(&inq);
  return r;
}
#endif



#ifdef _SER_TX_
bool serial_can_write(void) {
  return !queue_is_full(&outq);
}


void serial_write(uint8_t c) {
  while (queue_is_full(&outq)); // better put to sleep
  queue_enqueue(&outq, c);
  // Once there is data, it can transmit => activate interrupts
  UCSR0B |= _BV(UDRIE0);
}
#endif



/***********************************************************
 * Serial high level functions
 ***********************************************************/


#ifdef _SER_TX_
void serial_eol(void) {
  serial_write('\r');
  serial_write('\n');
}

void serial_write_s(char t[]) {
  for (int i=0;  t[i] != '\0'; i++) {
    serial_write(t[i]);
  }
}

void serial_write_ui(unsigned int i) {
  char t[6];

  utoa(i, t, 10);
  (void)serial_write_s(t);
}
#endif








void serial_open(void) {
  #ifdef _SER_RX_
  // Enable reception
  UCSR0B |= _BV(RXEN0);
  // Enable rx interrupts
  UCSR0B |= _BV(RXCIE0);
  #endif
  #ifdef _SER_TX_
  // Enable transmision
  UCSR0B |=  _BV(TXEN0);
  #endif

}


void serial_close(void) {
  #ifdef _SER_TX_
  /* wait last byte sent 
   * waits that tx-isr disables "data register empty" interrupt:
   * this means that tx queue emptied and last char was sent 
   */
  while (UCSR0B & _BV(UDRIE0));
  /* disable transmision*/
  UCSR0B &= ~_BV(TXEN0);  
  #endif
  #ifdef _SER_RX_
  /* disable receive interrupts */
  UCSR0B &= ~_BV(RXCIE0);  
  /* disable reception */
  UCSR0B &= ~_BV(RXEN0);
  #endif
}



void serial_setup(void) {
  // Initialize the queues
  #ifdef _SER_RX_
  queue_empty(&inq);
  #endif
  #ifdef _SER_TX_
  queue_empty(&outq);
  #endif

  // Initialize the UART0 According to ¶19.5 we must wait last
  // transmision finishes and all data received: ignoring it.
  // According to ¶19.5 interrupts must be disabled and restored at
  // end: assume setup is done with interrupts disabled always.
  UBRR0H = BAUDRATE_H(9600);
  UBRR0L = BAUDRATE_L(9600);
  // set normal baud rate 
  UCSR0A = UINT8_C(0);
  UCSR0C = 
    (_BV(UCSZ01)   | _BV(UCSZ00)) &   // 8 bit frame
    ~_BV(UMSEL01) & ~_BV(UMSEL00) &   // asincronous mode
    ~_BV(UPM01)   & ~_BV(UPM00)   &   // no parity
    ~_BV(USBS0)   ;                   // 1 stop bit
}

