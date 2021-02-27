#ifndef CHARMAPS_H_
#define CHARMAPS_H_

#define BLINKING_CHAR 255 //Full-squared character

//LCD Custom Chars memory index
#define U 0x01
#define P 0x02
#define C 0x03
#define UPC_STR ((char []){U,P,C,'\0'})

/*
 * User-defined charmaps
 */

//Bold "U" (UPC style)
static uint8_t U_char[8] = {
  0b11011,
  0b11011,
  0b11011,
  0b11011,
  0b11011,
  0b11011,
  0b11111,
  0b01110
};

//Bold "P" (UPC style)
static uint8_t P_char[8] = {
  0b11110,
  0b11111,
  0b11011,
  0b11011,
  0b11111,
  0b11110,
  0b11000,
  0b11000
};

//Bold "C" (UPC style)
static uint8_t C_char[8] = {
  0b01111,
  0b11111,
  0b11000,
  0b11000,
  0b11000,
  0b11000,
  0b11111,
  0b01111
};

#endif
