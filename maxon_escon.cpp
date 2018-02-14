#include "Arduino.h"
#include "maxon_escon.h"
#include "timer.h"

maxon_escon::maxon_escon(unsigned int pin_motorsnelheid, unsigned int pin_motorstroom, unsigned int pin_enable, unsigned int pin_pwm, unsigned int pin_status)
{
  this->pin_motorsnelheid = pin_motorsnelheid;
  this->pin_motorstroom = pin_motorstroom;
  this->pin_enable = pin_enable;
  this->pin_pwm = pin_pwm;
  this->pin_status = pin_status;
  pinMode(pin_enable, OUTPUT);
  pinMode(pin_status, INPUT_PULLUP);
  pinMode(pin_motorsnelheid, INPUT);
}

void maxon_escon::handling()
{
  static timer timer_enable(100, true);
  bool status = digitalRead(pin_status);
  static int oldstap = 1;
  static int oldKracht = 1;
  int pwmWaarde;
  
  if(stap != oldstap){
    oldstap = stap;
    Serial.print("Stap: ");
    Serial.println(stap);
  }
  if(kracht != oldKracht){
    oldKracht = kracht;
    Serial.print("Kracht: ");
    Serial.println(kracht);
  }
  switch(stap)
  {
    case 0:   digitalWrite(pin_enable, false);
              analogWrite(pin_pwm, PWM_MIN);
              //if(timer_enable.started() == false) timer_enable.start_timer();
              if(regelaar_enable == true && kracht > 0 && timer_enable.timeout()){
                stap = 1;
              }
    break;
    
    case 1:   digitalWrite(pin_enable, true);
              //analogWrite(pin_pwm, PWM_MIN);
              timer_enable.stop_timer();
              stap = 2;
    break;
    
    case 2:  switch(kracht){
                case 1:
                  pwmWaarde = 40; // 60
                break;

                case 2:
                  pwmWaarde = 60; // 100
                break;

                case 3:
                  pwmWaarde = 80; // begrensd
                break;
                
                default:
                  stap = 0;
                  timer_enable.start_timer();
                break;
              }
              analogWrite(pin_pwm, pwmWaarde);
              
              if(status == false){
                stap = 3;
                timer_enable.start_timer();
              }
    break;
    
    case 3:   
              if(error_teller < 2){
                error_teller++;
                stap = 0;
              }
              else
                stap = 4;
    break;
    case 4:   this->fout = true;
              digitalWrite(pin_enable, false);
              analogWrite(pin_pwm, PWM_MIN);
              stap = 0;
    break;
    default:  stap = 0;
    break;
  }
}



bool maxon_escon::get_fout(){
  return this->fout;
}

void maxon_escon::enable(){
  this->regelaar_enable = true;
}

void maxon_escon::disable(){
  this->regelaar_enable = false;
}

void maxon_escon::set_kracht(unsigned int kracht){
  if(kracht > 3) kracht = 3;
  this->kracht = kracht;
}
/*
void maxon_escon::set_snelheid(unsigned int setpoint_snelheid_rpm)
{
  if(setpoint_snelheid_rpm > PWM_BEGRENSD) setpoint_snelheid_rpm = PWM_BEGRENSD;
  this->setpoint_snelheid_rpm = setpoint_snelheid_rpm;
}
*/
unsigned int maxon_escon::get_snelheid()
{
  // y = map(x, xmin, xmax, ymin, ymax);
  // xmin = 0   (ADC) = 0.0V
  // xmax = 818 (ADC) = 4.0V
  // ymin = 0 RPM
  // ymax = 4000 RPM
  unsigned int snelheid = map(analogRead(pin_motorsnelheid), 0, 818, 0, 4000);
  return snelheid;
}

unsigned int maxon_escon::get_snelheid_kmh()
{
  // y = map(x, xmin, xmax, ymin, ymax);
  // xmin = 0   (ADC) = 0.0V
  // xmax = 484 (ADC)
  // ymin = 0 kmh
  // ymax = 25 kmh * 10 = 250
  unsigned int snelheid = map(analogRead(pin_motorsnelheid), 0, 484, 0, 250);
  return snelheid;
}
/*
unsigned int maxon_escon::get_snelheid_kmh()
{
  unsigned int kmh = pwm_to_kmh(snelheid_pwm);
  return this->snelheid;
}

unsigned int maxon_escon::get_motorstroom()
{
  
}

unsigned int maxon_escon::pwm_to_kmh(unsigned int pwm)
{
  
}
*/
