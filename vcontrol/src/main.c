#include <avr/interrupt.h>
#include "pt.h"
#include "ui.h"
#include "vaction.h"


int main(void) {
  /* context dels threads */
  struct pt vaction_ctxt, display_ctxt, buttons_ctxt, encoder_ctxt;

  /* init modules */
  sei();
  UI_setup();
  vaction_setup();


  /* init thread contexts */
  PT_INIT(&buttons_ctxt);
  PT_INIT(&encoder_ctxt);
  PT_INIT(&display_ctxt);
  PT_INIT(&vaction_ctxt);
  
  /* do schedule */
  for(;;) {
    (void)PT_SCHEDULE(buttons_thread(&buttons_ctxt));
    (void)PT_SCHEDULE(encoder_thread(&encoder_ctxt));
    (void)PT_SCHEDULE(display_thread(&display_ctxt));
    (void)PT_SCHEDULE(vaction_thread(&vaction_ctxt));
  }
}
