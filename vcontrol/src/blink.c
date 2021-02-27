/**
 * The blink module takes care of the blinking state of a param when gets selected.
 */

#include <stdbool.h>
#include <stdint.h>
#include "blink.h"


uint8_t blink_count = 0;

/**
 * @ brief Increases the blink counter up to the UNBLINK_FREQ and resets the counter to 0
 */
void update_blink_counter(void) {
  if (blink_count < UNBLINK_FREQ) {
    blink_count++;
  } else {
    blink_count = 0;
  }
}


/**
 * @brief Resets the blink counter to the blinking state. Useful when a param is selected and the user gets a fast response.
 */
void reset_blink_counter(void){
    blink_count = BLINK_FREQ;
}


/**
 * @brief Resets the blink counter to the unblinking state. Useful when a param is changed and the user can quickly see the new value.
 */
void reset_unblink_counter(void){
    blink_count = UNBLINK_FREQ;
}


/**
 * @brief   Returns true when it's time to blink
 * @return  A boolean. True if needs blinking
 */
bool is_blink_time(){
    return blink_count == BLINK_FREQ;
}


/**
 * @brief   Returns true when it's time to unblink
 * @return  A boolean. True if needs unblinking
 */
bool is_unblink_time(){
    return blink_count == UNBLINK_FREQ;
}
