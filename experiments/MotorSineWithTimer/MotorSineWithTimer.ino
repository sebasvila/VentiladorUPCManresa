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

# define nTargets 11

struct target_t{
  int t;  // time in ms
  long int p;  // period in us
  
};

// ==================== TARGET ====================
target_t target[nTargets] = {
// millis, stepPeriod  
{   0 ,  5000 }, // steps : 0
{  50 , 2565 }, // steps : 39
{ 150 , 1348 }, // steps : 75
{ 250 ,  980 }, // steps : 103
{ 350 ,  833 }, // steps : 121
{ 450 ,  793 }, // steps : 127
{ 550 ,  833 }, // steps : 121
{ 650 ,  980 }, // steps : 103
{ 750 , 1348 }, // steps : 75
{ 850 , 2565 }, // steps : 39
{ 950 , 5000 }, // steps : 0
                // Total steps :798
};


long int step_us = 20000; // microseconds

unsigned long time0,time1; // microseconds
unsigned long tms0,tms1;   // milliseconds

uint8_t currDir; // current direction
unsigned long totalCWSteps, totalCCWSteps;

// Auxiliary variables
long int period;

//TODO: convert to class
//========================================================================
boolean table_active = false;
boolean table_scheduled = false;
int table_index;
target_t table[nTargets];
//------------------------------------------------------------------------
void table_schedule_at(unsigned long tms) {
  // build the table for the scheduled
  for (int i=0; i<nTargets; i++) {
    table[i].t=target[i].t+tms;
    table[i].p=target[i].p;     // TODO: might be unnecessary: always the same
  }
  table_scheduled=true;
/*
  for (int i=0; i<nTargets; i++) {
    Serial.print(table[i].t);
    Serial.print(" : ");
    Serial.println(table[i].p);
  } 
*/  
}

void table_activate() {
  table_active=true;
  table_index=0;
  Serial.println("A");
}

void table_deactivate(){
  table_active=false;
  table_scheduled=false;
  Serial.println("D");
}


//========================================================================
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(EN , OUTPUT);
  pinMode(DIR, OUTPUT);
  pinMode(STP, OUTPUT);
  
  digitalWrite(EN , LOW );
  setDir(CW);
  table_active=false;
  Serial.begin(115200);

  step_us=target[0].p;

  totalCWSteps=0;
  totalCCWSteps=0;

  tms0=millis();
  time0=micros();

  table_schedule_at(2000); // schedule a movement at 1s from now

  Serial.println(table_active);
  Serial.println(table_scheduled);
  Serial.println(table[0].t);
  
}
//========================================================================
void setDir(uint8_t val){
  currDir=val;
  digitalWrite(DIR, currDir);
}
//========================================================================
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

//========================================================================
// the loop function runs over and over again forever
void loop() {

  //Do the fast stuff
  time1=micros();
  if ((time1-time0) >= step_us) {
    if (table_active) {
      doStep();
    }
    time0=time1; // TODO: think if this has to be only when table_active
  }

  //Do the slow stuff
  tms1= millis();
  if (table_active){
    if (tms1 >= table[table_index].t) {
      table_index++;
      if (table_index>nTargets){
        table_deactivate();
        Serial.print("CCW >> ");
        Serial.println(totalCCWSteps);
        Serial.print("CW  >> ");
        Serial.println(totalCWSteps);  

        table_schedule_at(tms1+2000); 
      }
      
      period= table[table_index].p; //period has sign: CW/CCW
      step_us=abs(period);
    }
  }  
  else { //see if it has to become active
//   delayMicroseconds(2);
//    Serial.println("=");
//    Serial.println(tms1);
//    Serial.println(table[0].t);
    if ((table_scheduled) && (tms1 >= table[0].t)) {
      table_activate();
    }
    
  }
  
}
