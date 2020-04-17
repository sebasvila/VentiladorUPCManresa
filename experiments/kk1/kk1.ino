/*
  This sketch assumes a single
  DRV8825 DRIVER in Y slot of the CNC Board

  Objective: to produce a speed profile depending on table

  1-cos() in CW direction
  constant speed in the CCW direction
*/
#include <LiquidCrystal.h>
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 7, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

# define EN    8
# define DIR   6
# define STP   3

# define CW    LOW
# define CCW   HIGH

# define nTargets 11

# define specialStep 0

uint32_t step_us = 20000; // microseconds

uint32_t totalSteps, totalCWSteps, totalCCWSteps;
uint8_t motor_currDir; // current direction
uint8_t main_dir; // direction in main loop

uint32_t time0, time1; // microseconds
uint32_t t_main0, t_main1;  // milliseconds

uint32_t cntMain = 0;

//=============================================================TABLE===========
enum states {INACTIVE, SCHEDULED, ACTIVE, FINISHED};

struct table_t {
  uint16_t s;  // number of steps
  uint16_t p;  // period in us
};

enum states table_state = INACTIVE;

uint8_t  table_index = 0;
uint32_t table_startTime;
boolean  table_keepEnabled = true;

uint16_t table_nSteps, table_dur_ms;




table_t table[nTargets];
//------------------------------------------------------------------------
void table_schedule_at(uint32_t t_ms,   // schedule time in ms
                       uint8_t dir,          // CW / CCW
                       uint16_t nSteps, // how many steps: 800 = 1 rev
                       uint16_t dur_ms, // how long lasts in ms
                       boolean keep_enabled) // keep motor enabled after end?
{
  const table_t baseTable[nTargets] = {
    // step,period
    { 16 , 5682 },
    { 47 , 1934 },
    { 75 , 1212 },
    { 96 , 947 },
    { 109 , 834 },
    { 114 , 797 },
    { 109 , 834 },
    { 96 , 947 },
    { 75 , 1212 },
    { 47 , 1934 },
    { 16 , 5682 },
  };

  table_startTime = t_ms;
  motor_setDir(dir);
  table_state = SCHEDULED;
  table_nSteps = nSteps;
  table_dur_ms = dur_ms;
  table_keepEnabled = keep_enabled;
  for (int i = 0; i < nTargets; i++) {
    table[i].s = (uint32_t) nSteps * (uint32_t)baseTable[i].s / (uint32_t) 800;
    table[i].p = ((uint32_t) dur_ms * (uint32_t) baseTable[i].p * (uint32_t) 4) / ((uint32_t) 5 * (uint32_t) nSteps);
  }
  step_us = table[0].p;
}
//------------------------------------------------------------------------
void table_activate() {
  motor_enable(true);
  table_state = ACTIVE;
  motor_resetCounters();
  table_index = 0;
  step_us = table[0].p;
  Serial.print("A");
  Serial.println(millis());
}

//------------------------------------------------------------------------
void table_update() {
  if (table_state == ACTIVE) {
    if (totalSteps >= table[table_index].s) {
      totalSteps = 0;
      table_index++;
      if (table_index < nTargets) {
        step_us = table[table_index].p;
      }
      else {
        motor_enable(table_keepEnabled);
        table_state = FINISHED;
      }
    }
  }
  else { //see if it has to become active
    if ((table_state = SCHEDULED) && (millis() >= table_startTime)) {
      table_activate();
    }
  }
}

//=============================================================MOTOR===========
void motor_setup() {
  pinMode(EN , OUTPUT);
  pinMode(DIR, OUTPUT);
  pinMode(STP, OUTPUT);

  motor_setDir(CW);
  motor_resetCounters();

}

void motor_resetCounters() {
  totalCWSteps = 0;
  totalCCWSteps = 0;
}

void motor_enable(boolean enable) {
  if (enable) {
    digitalWrite(EN , LOW );
  }
  else {
    digitalWrite(EN , HIGH );
  }
}

void motor_setDir(uint8_t val) {
  motor_currDir = val;
  digitalWrite(DIR, motor_currDir);
}
//------------------------------------------------------------------------
void motor_doStep() {
  if (table_state == ACTIVE) {
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
    totalSteps++;
  }
}

void motor_update() {
  time1 = micros();
  if ((time1 - time0) >= step_us) {

    motor_doStep();

    time0 = time1; // TODO: think if this has to be only when table_active
    //time0 = time0+step_us; // TODO: think if this has to be only when table_active
  }
}

//========================================================================
void setup() {
  motor_setup();
  lcd.begin(16, 2);
  Serial.begin(115200);

  table_schedule_at(2000 , main_dir=CW, 800, 1000,     true);
  t_main0 = micros();


}

//========================================================================

uint32_t tsched = 7000;
void loop() {



  motor_update();
  table_update();

  if (table_state == FINISHED) {
    t_main1 = micros();
    Serial.println("Time / Steps ");

    Serial.print(t_main1 - t_main0);
    Serial.print(" / ");
    Serial.println(totalCWSteps);
    Serial.print(t_main1 - t_main0);
    Serial.print(" / ");
    Serial.println(totalCCWSteps);

    if (main_dir == CW) {
      table_schedule_at(tsched , main_dir = CCW, 1000, 1000,     true);
    }
    else {
      table_schedule_at(tsched , main_dir = CW , 1000, 1000,     true);
    }
    tsched+=5000;
  }
}
