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

# define specialStep 0

struct target_t {
  unsigned long t;  // time in ms
  unsigned long p;  // period in us

};


// ==================== TARGET ====================
target_t target[nTargets] = {
  // millis, stepPeriod
  {   0 , specialStep }, // steps : 0
  { 100 , 2565 }, // steps : 39
  { 200 , 1348 }, // steps : 75
  { 300 ,  980 }, // steps : 103
  { 400 ,  833 }, // steps : 121
  { 500 ,  793 }, // steps : 127
  { 600 ,  833 }, // steps : 121
  { 700 ,  980 }, // steps : 103
  { 800 , 1348 }, // steps : 75
  { 900 , 2565 }, // steps : 39
  {1000 , specialStep }, // steps : 0
  // Total steps :798
};


unsigned long step_us = 20000; // microseconds

unsigned long time0, time1; // microseconds
unsigned long t_ms0, t_ms1;  // milliseconds

uint8_t motor_currDir; // current direction
uint8_t main_dir; // direction in main loop
unsigned long totalCWSteps, totalCCWSteps;

// Auxiliary variables
long int period;

//TODO: convert to class
//=============================================================TABLE===========
boolean table_active = false;
boolean table_scheduled = false;
boolean table_finished = false;
int table_index;
target_t table[nTargets];
//------------------------------------------------------------------------
void table_schedule_at(unsigned long t_ms,
                       uint8_t dir,
                       unsigned long nSteps,
                       unsigned long dur_ms) {
  // build the table for the scheduled
  for (int i = 0; i < nTargets; i++) {
    table[i].t = (target[i].t * dur_ms) / 1000 + t_ms;
    table[i].p = (800 * target[i].p) * dur_ms / (nSteps * 1000);
  }
  table_scheduled = true;
  table_finished=false;
  motor_setDir(dir);

  for (int i = 0; i < nTargets; i++) {
    Serial.print(table[i].t);
    Serial.print(" : ");
    Serial.println(table[i].p);
  }

}

void table_activate() {
  table_active = true;
  table_finished = false;
  table_index = 0;
  Serial.println("A");
}

void table_deactivate() {
  table_active = false;
  table_scheduled = false;
  Serial.println("D");
}

void table_update() {
  t_ms1 = millis();
  if (table_active) {
    if (t_ms1 >= table[table_index].t) {
      table_index++;
      if (table_index > nTargets) {
        table_deactivate();
        table_finished = true;
        Serial.print("CCW >> ");
        Serial.println(totalCCWSteps);
        Serial.print("CW  >> ");
        Serial.println(totalCWSteps);
      }
      step_us = table[table_index].p;
    }
  }
  else { //see if it has to become active
    if ((table_scheduled) && (t_ms1 >= table[0].t)) {
      table_activate();
    }
  }
}

//=============================================================MOTOR===========
void motor_setup() {
  pinMode(EN , OUTPUT);
  pinMode(DIR, OUTPUT);
  pinMode(STP, OUTPUT);

  digitalWrite(EN , LOW );
  motor_setDir(CW);

  totalCWSteps = 0;
  totalCCWSteps = 0;
}

void motor_setDir(uint8_t val) {
  motor_currDir = val;
  digitalWrite(DIR, motor_currDir);
}
//------------------------------------------------------------------------
void motor_doStep() {
  if (step_us != specialStep) {
    digitalWrite(STP, HIGH);
    delayMicroseconds(2);
    digitalWrite(STP, LOW);
    delayMicroseconds(2);
    if (motor_currDir == CW) {
      totalCWSteps++;
    }
    else {
      totalCCWSteps++;
    }
  }
}

void motor_update() {
  time1 = micros();
  if ((time1 - time0) >= step_us) {
    if (table_active) {
      motor_doStep();
    }
    time0 = time1; // TODO: think if this has to be only when table_active
  }
}

//========================================================================
void setup() {
  motor_setup();

  Serial.begin(115200);

  step_us = target[0].p;

  t_ms0 = millis();
  time0 = micros();

  main_dir=CW;
  table_schedule_at(2000, main_dir, 4000, 1000); // schedule a movement at 1s from now

  Serial.println(table_active);
  Serial.println(table_scheduled);
  Serial.println(table[0].t);
}

//========================================================================
// the loop function runs over and over again forever
void loop() {

  //Do the fast stuff
  motor_update();

  //Do the slow stuff
  table_update();
  if (table_finished) {
    if (main_dir==CW) {
      main_dir=CCW;
    }
    else {
      main_dir=CW;
    }
    table_schedule_at(millis() + 2000, main_dir, 4000, 1000);
  }
}
