#include "timer.h"
#include "Arduino.h"

void timer::start_timer(){          // Start timer with the predetermined time
  _gestart = 1;
  _mil = _vorige_mil = millis();
}

void timer::start_timer(unsigned int _tijd){ // Start timer, and set a new time
  if(_tijd > 0){
    this->_tijd = _tijd;
    _gestart = 1;
    _mil = _vorige_mil = millis();
  }
}

void timer::stop_timer(){         // Stop timer
  _gestart = 0;
  _afgelopen = 0;
}

bool timer::timeout(){            // If timeout has occurred
  _mil = millis();
  if(_gestart && (_mil - _vorige_mil) >= _tijd){
    _vorige_mil = _mil;
    _afgelopen = 1;
  }
  return _afgelopen;
}

void timer::reset_timer(){          // Reset timer
  _mil = _vorige_mil = millis();
  _afgelopen = 0;
}

void timer::set_time(unsigned int _tijd){  // Set the time. Can be done while running.
  if(_tijd > 0) this->_tijd = _tijd;
}

bool timer::started(){
  return _gestart;
}
