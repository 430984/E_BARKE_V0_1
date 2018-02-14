#ifndef TIMER_H
#define TIMER_H
#include "Arduino.h"

#define STANDAARD_TIJD_WAARDE_MS  1000  // Standaard tijdswaarde in ms (1*10^-3 sec, milliseconden)

class timer{
private:
  unsigned int _tijd = STANDAARD_TIJD_WAARDE_MS;
  unsigned long _mil = 0;
  unsigned long _vorige_mil = 0;
  bool _gestart = false;
  bool _afgelopen = false;
  
public:
  ///// CONSTRUCTORS /////
  // Constructor type 1, geen tijd gegeven
  timer(){
  }
  // Constructor type 2, tijd is gegeven
  timer(unsigned int _tijd){
    if(_tijd > 0) this->_tijd = _tijd;
    else this->_tijd = STANDAARD_TIJD_WAARDE_MS;
  }
  // Constructor type 3, tijd is gegeven, en de timer kan worden gestart als deze wordt aangemaakt!
  timer(unsigned int _tijd, bool _gestart){
    if(_tijd > 0) this->_tijd = _tijd;
    else this->_tijd = STANDAARD_TIJD_WAARDE_MS;
    this->_gestart = _gestart;
  }
  ///// Functies /////
  void start_timer();
  void start_timer(unsigned int _tijd);
  void stop_timer();
  bool timeout();
  void reset_timer();
  void set_time(unsigned int _tijd);
  bool started();
};

#endif
