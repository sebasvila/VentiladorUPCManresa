/*
  This sketch assumes a single
  DRV8825 DRIVER in Y slot of the CNC Board
*/

# define EN    8
# define DIR   6
# define STP   3

# define stepsPerTurn 800
# define step_us  80
# define maxTurns 10


unsigned long time0;

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(EN , OUTPUT);
  pinMode(DIR, OUTPUT);
  pinMode(STP, OUTPUT);
  
  digitalWrite(EN , LOW );
  digitalWrite(DIR, LOW);

  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
  time0 = millis();
  for (int i = 0; i < stepsPerTurn; i++) {
    
    digitalWrite(STP, HIGH);
    digitalWrite(LED_BUILTIN, HIGH);
    delayMicroseconds(step_us);
    
    digitalWrite(STP, LOW);
    digitalWrite(LED_BUILTIN, LOW); 
    delayMicroseconds(step_us);
    
  }
  Serial.println(millis()-time0);
  
  delay(1000);
}
