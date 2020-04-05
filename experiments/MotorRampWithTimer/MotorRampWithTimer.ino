/*
  This sketch assumes a single
  DRV8825 DRIVER in Y slot of the CNC Board

  Objective: to produce a speed profile depending on table

  1-cos() in CW direction
  constant speed in the CCW direction
*/

# define EN    8
# define DIR   6
# define STP   3

# define CW    LOW
# define CCW   HIGH

# define nTargets 22

struct target_t{
  int t;  // time in ms
  long int p;  // period in us
  
};

// ==================== TARGET ====================
target_t target[nTargets+1] = {
// millis, stepPeriod  
{ 50 , 110000 }, // steps : 1
{ 100 , 2565 }, // steps : 39
{ 100 , 1348 }, // steps : 75
{ 100 , 980 }, // steps : 103
{ 100 , 833 }, // steps : 121
{ 100 , 793 }, // steps : 127
{ 100 , 833 }, // steps : 121
{ 100 , 980 }, // steps : 103
{ 100 , 1348 }, // steps : 75
{ 100 , 2565 }, // steps : 39
{ 50 , 110000 }, // steps : 1
// Total steps :805
{ 25 , -55000 }, // steps : 1
{ 50 , -1285 }, // steps : 39
{ 50 , -675 }, // steps : 75
{ 50 , -491 }, // steps : 102
{ 50 , -417 }, // steps : 120
{ 50 , -397 }, // steps : 126
{ 50 , -417 }, // steps : 120
{ 50 , -491 }, // steps : 102
{ 50 , -675 }, // steps : 75
{ 50 , -1285 }, // steps : 39
{ 25 , -55000 }, // steps : 1
// Total steps :800
{ 1500, 1600000 }
};
unsigned int index; //0..nTargets

long int step_us; // microseconds

unsigned long time0,time1; // microseconds
unsigned long tms0,tms1;   // milliseconds

uint8_t currDir; // current direction
unsigned long totalCWSteps, totalCCWSteps;


// Auxiliary variables
long int period;

//========================================================================
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(EN , OUTPUT);
  pinMode(DIR, OUTPUT);
  pinMode(STP, OUTPUT);
  
  digitalWrite(EN , LOW );
  setDir(CW);

  Serial.begin(115200);

  index=0;
  step_us=target[0].p;

  totalCWSteps=0;
  totalCCWSteps=0;

  tms0=millis();
  time0=micros();
}

void setDir(uint8_t val){
  currDir=val;
  digitalWrite(DIR, currDir);
}

void doStep() {
  digitalWrite(STP, HIGH);
  delayMicroseconds(2);
  digitalWrite(STP, LOW);
  delayMicroseconds(2);
  if (currDir==CW){
    totalCWSteps++;
  }
  else {
    totalCCWSteps++;
  }
}


// the loop function runs over and over again forever
void loop() {

  //Do the fast stuff
  time1=micros();
  if ((time1-time0) >= step_us) {
    doStep();
    time0=time1;
  }

  //See if step_us has to be updated
  tms1= millis();
  if ((tms1-tms0) > target[index].t) {
    index++;
    if (index>nTargets){
      index=0;
    }
    tms0=tms1;
    period= target[index].p; //period has sign: CW/CCW
    step_us=abs(period);
    if (period>0){
      if (currDir==CCW){
        Serial.print("CCW >>");
        Serial.println(totalCCWSteps);
      }
      setDir(CW);
    }
    else{
      if (currDir==CW) {
        Serial.print("CW  >>");
        Serial.println(totalCWSteps);        
      }
      setDir(CCW);
      
    }
//    Serial.println(index);
//    Serial.println(period);
  }
}
