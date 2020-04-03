/*
  This sketch just sets de Enable signal High to be able to set
  the Maximum Current

  To have 1A, turn the pot to read 0.5V
*/


# define EN    8

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  // make enable pin output and set low
  pinMode(EN, OUTPUT);
  digitalWrite(EN, LOW);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);                       // wait 0.1s
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(900);                       // wait 0.9s
}
