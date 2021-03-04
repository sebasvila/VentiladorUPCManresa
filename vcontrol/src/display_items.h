#ifndef DISPLAY_ITEMS_H_
#define DISPLAY_ITEMS_H_

#include "charmaps.h"

#define SENTINEL "0"

/*
 * Screen items IDs
 */

//Main screen items IDs
#define MAIN_FORM_NUM 6
#define VOLUME 0
#define RATIO 1
#define FREQ 2
#define PEAK 3
#define PLATEAU 4
#define START_STOP 5

//Starting screen items IDs
#define STARTING_FORM_NUM 2
#define UPC 0
#define MANRESA 1
#define RESPIRADOR 2

//Menu screen items IDs
#define MENU_FORM_NUM 1   //Including sentinel
#define MENU_ID 0

//Char array where the converted values will be stored
char value_ch[20]; 

//Start & stop strings
#define START_ST "START->"
#define STOP_ST " STOP->"

#define OFF_ST "--"
#define UINT_FORMAT "%u"

/*
 * Type to define a display item formed by a label and its value
 */
typedef struct {
  char *label;
  uint8_t label_col;
  uint8_t label_row;
  uint16_t value;
  char *val_format;
  uint8_t value_col;
  uint8_t value_row;
  uint16_t min_val;
  uint16_t max_val;
  bool is_selected;
  bool is_changed;
  uint8_t step_val;
} field_t;



/*************************************
 * Main screen fields
 *************************************/

field_t main_form[MAIN_FORM_NUM] = {
  {
    "VOLUME",
    0,      //Label Column
    2,      //Label Row
    50,     //Value
    "%u%%", //Value's format
    2,      //Value Column
    3,      //Value Row
    10,     //Min Value
    99,     //Max Value
    false,  //Blinking
    false,  //Is changed
    1       //Step value
  },
  {
    "I:R",
    9,      //Label Column
    2,      //Label Row
    1,      //Value     **************Canviar******************
    "1:%u", //Value's format
    9,      //Value Column
    3,      //Value Row
    1,      //Min Value
    4,      //Max Value
    false,  //Blinking
    false,  //Is changed
    1       //Step value
  },
  {
    "FREQ",
    16,     //Label Column
    2,      //Label Row
    22,     //Value
    UINT_FORMAT,   //Value's format
    17,     //Value Column
    3,      //Value Row
    10,     //Min Value
    25,     //Max Value
    false,  //Blinking
    false,  //Is changed
    1       //Step value
  },
  {
    "PEAK:",
    0,      //Label Column
    0,      //Label Row
    36,     //Value
    OFF_ST, //Value's format
    6,      //Value Column
    0,      //Value Row
    0,      //Min Value
    500,    //Max Value
    false,  //Blinking
    false   //Is changed
  },
  {
    "PLATEAU:",
    0,      //Label Column
    1,      //Label Row
    25,     //Value
    OFF_ST, //Value's format
    9,      //Value Column
    1,      //Value Row
    0,      //Min Value
    500,    //Max Value
    false,  //Blinking
    false   //Is changed
  },
  {
    " ",
    12,     //Label Column
    0,      //Label Row
    0,      //Value: Not used
    START_ST,//Value's format
    13,     //Value Column
    0       //Value Row
  },
};



/*************************
 * Starting screen items
 *************************/

field_t starting_form[STARTING_FORM_NUM] = {
  {
    UPC_STR,
    8,     //Label Column
    0,      //Label Row
    0,      //Value (not used)
    "Manresa", //Value format
    6,      //Value Column
    1       //Value Row
  },
  {
    "VENTILATOR",
    5,      //Label Column
    2,      //Label Row
    2020,   //Value
    UINT_FORMAT,   //Value Format
    8,      //Value Column
    3       //Value Row
  }
};





/*************************
* Menu screen fields
*************************/

field_t menu_form[MENU_FORM_NUM] = {
  {
    "MENU",
    8,    //Label Column
    0,    //Label Row
    0,    //Value (not used)
    ""    //Value format
  }
};



#endif
