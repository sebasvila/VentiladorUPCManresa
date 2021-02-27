#include <avr/interrupt.h>
#include "pt.h"
#include "ui.h"
#include "vaction.h"


int main(void) {
  /* context dels threads */
  struct pt vaction_ctxt, display_ctx, buttons_ctx, encoder_ctx;

  /* init modules */
  UI_setup();
  vaction_setup();
  sei();


  /* init thread contexts */
  PT_INIT(&buttons_ctx);
  PT_INIT(&encoder_ctx);
  PT_INIT(&vaction_ctxt);
  PT_INIT(&display_ctx);
  
  /* do schedule */
  for(;;) {
    (void)PT_SCHEDULE(buttons_thread(&buttons_ctx));
    (void)PT_SCHEDULE(encoder_thread(&encoder_ctx));
    (void)PT_SCHEDULE(vaction_thread(&vaction_ctxt));
    (void)PT_SCHEDULE(display_thread(&display_ctx));
  }
}
