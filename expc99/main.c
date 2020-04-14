#include <stdbool.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include "pt.h"
#include "vaction.h"
#include "pin.h"



int main(void) {
  /* context dels threads */
  struct pt vaction_ctxt;

  /* init modules */
  vaction_ctxt = vaction_setup();
  sei();
  
  /* do schedule */
  for(;;) {
    (void)PT_SCHEDULE(vaction_thread(&vaction_ctxt));
  }
}
