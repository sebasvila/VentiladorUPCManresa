#ifndef UI_H
#define UI_H

#include "pt.h"

/**
 * @brief Protothread to refresh the display asyncronously
 */
PT_THREAD(display_thread(struct pt *pt));

/**
 * @brief   Protothread to read the buttons status and make changes to the display if needed
 */
PT_THREAD(buttons_thread(struct pt *pt));

/**
 * @brief   Protothread to read the encoder's pulses and update its position
 */
PT_THREAD(encoder_thread(struct pt *pt));

/**
 * @brief   Module setup. Initializes the display, buttons, encoder, and the ticker.
 */
void UI_setup(void);

#endif
