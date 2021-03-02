#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "pt.h"
#include "pt-delay.h"
#include "i2c.h"
#include "lcd_i2c.h"
#include "ticker.h"
#include "charmaps.h"
#include "display_items.h"
#include "switch.h"
#include "encoder.h"
#include "blink.h"
#include "vaction.h"


#define REFRESH_TIME 50  //times the ticker freq (16ms)


//MACHINE STATUS
#define STOP 0
#define WORKING 1

uint8_t machine_status = STOP;      /*!< The current machine's state */

#define LCD_I2C_ADDRESS 0x27
lcd_t l = {LCD_I2C_ADDRESS,4,
	   0,
	   0,
	   0,
	   Success,
	   0xFF};
	 
//FORM IDs
#define STARTING 0
#define MAIN_SCREEN 1
#define MENU 2

uint8_t form_id = 99;               /*!< The loaded form id */
bool is_form_changed = true;        /*!< True when a new form has changed and needs to be refreshed */
field_t *form = NULL;               /*!< A pointer to the current loaded form */
uint8_t num_of_fields = 0;          /*!< Contains the number of fields contained on the loaded form */

field_t *selected_field = NULL;     /*!< A pointer to the selected field */
uint16_t new_selected_value = 0;

/**
 * Buttons 
 */
switch_t btn_1;
switch_t btn_2;
switch_t btn_3;
switch_t btn_menu;
switch_t btn_start;
switch_t btn_enc;

#define SWITCH_ON_CLICK(btn, function_to_call) \
  do { \
    switch_poll(btn); \
    PT_WAIT_UNTIL(pt, switch_ready(btn)); \
    if (switch_state(btn) && switch_changed(btn)){ \
      function_to_call; \
    }; \
  } while (0)

//Encoder
encoder_t encoder;


/**
 * @brief     Sets the selected screen status to be displayed asynchronously
 * 
 * @param     new_form_id The new form's identifier
 */
void set_form(uint8_t new_form_id) {
  form_id = new_form_id;
  is_form_changed = true;
  switch (new_form_id) {
    case STARTING:
      form = starting_form;
      num_of_fields = STARTING_FORM_NUM;
      break;
    case MAIN_SCREEN:
      form = main_form;
      num_of_fields = MAIN_FORM_NUM;
      break;
    case MENU:
      form = menu_form;
      num_of_fields = MENU_FORM_NUM;
      break;
  }
}


/**
 * @brief   Prints the field's value (not the label)
 * @param   field The field to print its value
 */
void print_field_value(field_t *field){
  lcd_move_cursor(&l, field->value_col, field->value_row);
  sprintf(value_ch, field->val_format, field->value);
  lcd_print(&l, value_ch);
}


/**
 * @brief   Prints the field's new selected value (not the label)
 * @param   field The field to print its value
 */
void print_field_new_value(field_t *field){
  lcd_move_cursor(&l, field->value_col, field->value_row);
  sprintf(value_ch, field->val_format, new_selected_value);
  lcd_print(&l, value_ch);
}


/**
 * @brief     Selects the given parameter and starts blinking it asynchronously and disables any other blinking parameter to make sure that just one is selected.
 * @param     field_id The selected parameter id. Use the defined macro from display_items.h
 */
void select_field(uint8_t field_id) {
  reset_blink_counter();

  //Previous selected field
  if (selected_field != NULL){
    print_field_value(selected_field);
    selected_field->is_selected = false;
  }

  //Actual selected field
  if (field_id != 255){
    selected_field = &form[field_id];
    selected_field->is_selected = true;
    new_selected_value = selected_field->value;
  } else {
    selected_field = NULL;
    new_selected_value = 0;
  }
}


/**
 * @brief Unselects the current selected field. Internally selects the last value (255)
 */
void unselect_field(){
  select_field(255);
}


/**
 * @brief Changes the new selected value applying the increment param
 * 
 * @param increment The value to increment (positive or negative)
 */
void change_value_selected_field(int8_t increment) {
  if (selected_field != NULL){
    //Calculate 
    new_selected_value += (increment * selected_field->step_val);
    
    //Check limits (min & max)
    if (new_selected_value > selected_field->max_val){
      new_selected_value = selected_field->max_val;
    } else if (new_selected_value < selected_field->min_val){
      new_selected_value = selected_field->min_val;
    }

    selected_field->is_changed = true;
  }
}


/**
 * @brief Save the modified value to the corresponding field's struc and send it to "vaction" module
 */
void save_selected_value(){
  selected_field->value = new_selected_value;
  if (selected_field == &main_form[VOLUME]){
    vaction_set_tr(selected_field->value);
  } else if (selected_field == &main_form[RATIO]){
    vaction_set_ie(selected_field->value * 10);
  } else if (selected_field == &main_form[FREQ]){
    vaction_set_rr(selected_field->value * 10);
  }
}


/**
 * @brief Print the corresponding field to the lcd
 * 
 * @param field Pointer to the field 
 */
void print_field(field_t *field){
  lcd_move_cursor(&l, field->label_col,field->label_row);
  lcd_print(&l, field->label);
  if (field->value != 255){
    print_field_value(field);
  }
}



/**
 * @brief Print the blinking full-squared characters to the corresponding field
 * 
 * @param field Pointer to the field
 */
void blink_item_value(field_t *field){
  lcd_move_cursor(&l, field->value_col, field->value_row);
  sprintf(value_ch, field->val_format, new_selected_value);
  for (uint8_t i = 0; value_ch[i] != '\0'; i++) {
    lcd_print_ch(&l, BLINKING_CHAR);
  }
}



/**
 * @brief Creates and send the custom charmaps to the LCD
 */
void create_custom_chars(void) {
  lcd_create_char(&l, U, U_char);
  lcd_create_char(&l, P, P_char);
  lcd_create_char(&l, C, C_char);
}


/**
 * @brief Initializes the display
 */
void display_init(void) {
  /* l = lcd_constructor(LCD_I2C_ADDRESS,4); */
  _delay_ms(50);
  lcd_init(&l);

  create_custom_chars();
  lcd_clear(&l, true);

  set_form(STARTING);
}


/**
 * @brief Initializes the buttons and the encoder
 */
void buttons_init(void){
  btn_1 = switch_bind(&PORTB, 1);
  btn_2 = switch_bind(&PORTB, 0);
  btn_3 = switch_bind(&PORTD, 7);
  btn_menu = switch_bind(&PORTD, 6);
  btn_start = switch_bind(&PORTC, 3);
  btn_enc = switch_bind(&PORTD, 4);
  
  encoder = enc_create(&PORTD, 2, &PORTD, 3);
}


/**
 * @brief Prints the whole current form to the lcd 
 */
void print_form(void){
  for (uint8_t i = 0; i < num_of_fields; i++) {
    print_field(&form[i]);
  }
  is_form_changed = false;
}


/**
 * @brief Searches for the changed fields inside the current form and prints its values
 */
void print_changed_fields(void){
  for (uint8_t i = 0; i < num_of_fields; i++) {
    if(form[i].is_selected){
      if (is_blink_time()) {
        blink_item_value(&form[i]);
      } else if (is_unblink_time() | form[i].is_changed) {
        print_field_new_value(&form[i]);
        form[i].is_changed = false;    
        reset_unblink_counter();
      }
    } else if (form[i].is_changed) {
      print_field_value(&form[i]);
      form[i].is_changed = false;
      reset_unblink_counter();
    }
  }
}


/**
 * @brief Module setup. Initializes the display, buttons, encoder, and the ticker.
 */
void UI_setup(void){
  ticker_setup();

  i2c_setup();
  i2c_open();
  
  display_init();
  switch_setup();
  buttons_init();
  ticker_start();
}

/*****************************************************************
 * @brief   Protothread to refresh the display if needed
 *****************************************************************/
PT_THREAD(display_thread(struct pt *pt))
{
  static uint16_t chronos;

  PT_BEGIN(pt);

  for(;;) {
    if (is_form_changed) {
      lcd_clear(&l, true);
      reset_blink_counter();
      unselect_field();
      print_form();
    } else {
      print_changed_fields();
    }

    update_blink_counter();

    chronos = ticker_get();
    PT_WAIT_WHILE(pt, ticker_get() - chronos <= REFRESH_TIME);
  }
  PT_END(pt);
}




/****************************************************************************************
 * @brief   Protothread to read the buttons status and make changes to the display if needed
 ****************************************************************************************/
PT_THREAD(buttons_thread(struct pt *pt))
{
  // static uint16_t chronos2;
  PT_BEGIN(pt);

  for(;;) {
    if (form_id == STARTING){
      PT_DELAY(pt, 100); //To be changed. Simulates the homing seqüence
      set_form(MAIN_SCREEN);
    } else if (form_id == MAIN_SCREEN){
      //Volume button
      SWITCH_ON_CLICK(btn_1, select_field(VOLUME));

      //I:R button
      SWITCH_ON_CLICK(btn_2, select_field(RATIO));
      
      //Freq button
      SWITCH_ON_CLICK(btn_3, select_field(FREQ));
      
      //Encoder button
      SWITCH_ON_CLICK(btn_enc,
        save_selected_value();
        unselect_field()
      );
      
      //Start/stop button
      switch_poll(btn_start);
      PT_WAIT_UNTIL(pt, switch_ready(btn_start));
      if (switch_state(btn_start) && switch_changed(btn_start)){
        if (machine_status == STOP){                  //*** Start cycle ***
          main_form[START_STOP].val_format = STOP_ST;
          main_form[PLATEAU].val_format = UINT_FORMAT;
          main_form[PEAK].val_format = UINT_FORMAT;
          machine_status = WORKING;
          /* start_breathe(); */
        } else if (machine_status == WORKING){        //*** Stop cycle ***
          main_form[START_STOP].val_format = START_ST;
          main_form[PLATEAU].val_format = OFF_ST;
          main_form[PEAK].val_format = OFF_ST;
          machine_status = STOP;
          /* stop_breathe(); */
        }
        main_form[START_STOP].is_changed = true;
        main_form[PLATEAU].is_changed = true;
        main_form[PEAK].is_changed = true;   
      }
      //Encoder rotation
      if (get_position(&encoder) != 0){
        change_value_selected_field(get_position(&encoder));
        reset_position(&encoder);
      }
    }

    if ((form_id == MAIN_SCREEN) | (form_id == MENU)){
      //Menu button
      switch_poll(btn_menu);
      PT_WAIT_UNTIL(pt, switch_ready(btn_menu));
      if (switch_state(btn_menu) & switch_changed(btn_menu)){
        if (form_id == MAIN_SCREEN) set_form(MENU);
        else if (form_id == MENU) set_form(MAIN_SCREEN);
      }
    }

    PT_DELAY(pt, 10);
  }
  PT_END(pt);
}




/**********************************************************************************
 * @brief   Protothread to read the encoder's pulses and update its position
 **********************************************************************************/
PT_THREAD(encoder_thread(struct pt *pt))
{
  PT_BEGIN(pt);

  for(;;) {
    enc_update_position(&encoder);
    PT_YIELD(pt);
  }
  PT_END(pt);
}
