#include "comm.h"
#include "timer.h"
#include "data_typen.h"
#include "storingen.h"
#include <SoftwareSerial.h>

#define BUFFERGROOTTE 18

//#define TEST

unsigned int xbee::char_to_uint(char c){
  int i = 0;
  if(c >= 48 && c <= 57) 
    i = (uint8_t)c -48;
  return i;
}

void xbee::handling()
{
  static timer uart_timeout(20);
  static unsigned int pointer = 0;
  static byte buffer[BUFFERGROOTTE];

////////// COMMUNICATIE //////////
  // Zenden wanneer nodig. Gebeurt altijd 1 cyclus nadat de module iets heeft ontvangen.
  // Op deze manier wordt alle data die door setters veranderd wordt, de volgende cyclus meegenomen
  // met het zenden!
  if(_zenddata.nieuwedata){
    _zenddata.nieuwedata = false;
    String teZenden = "{";
    teZenden +=  (_zenddata.storingen / 100);
    teZenden += ((_zenddata.storingen / 10 ) %10);
    teZenden +=  (_zenddata.storingen %10);
    teZenden += ';';
    teZenden +=  (_zenddata.batterij / 100);
    teZenden += ((_zenddata.batterij / 10 ) %10);
    teZenden += (_zenddata.batterij %10);
    teZenden += ';';
    teZenden += (_zenddata.kracht / 100);
    teZenden += ((_zenddata.kracht / 10 ) %10);
    teZenden += (_zenddata.kracht %10);
    teZenden += ';';
    teZenden += (_zenddata.snelheid / 100);
    teZenden += ((_zenddata.snelheid / 10 ) %10);
    teZenden += (_zenddata.snelheid %10);
    teZenden += '}';
    xb->print(teZenden);
  }
  
  // Ontvangen als er iets beschikbaar is
  while(xb->available() > 0){
    char c = xb->read();
    if(pointer < BUFFERGROOTTE){
      buffer[pointer] = c;
      pointer++;
    }
    uart_timeout.start_timer();
  } 
  if(uart_timeout.timeout())
  {
    if(buffer[0] == '{' && buffer[4] == ';' && buffer[8] == ';' && buffer[12] == ';' && buffer[16] == '}'){
      // Het voldoet! Sla dit op als data die we verderop in het programma herkennen.
      _ontvangendata.storingen   = char_to_uint(buffer[1] ) * 100;
      _ontvangendata.storingen  += char_to_uint(buffer[2] ) *  10;
      _ontvangendata.storingen  += char_to_uint(buffer[3] );
      _ontvangendata.batterij    = char_to_uint(buffer[5] ) * 100;
      _ontvangendata.batterij   += char_to_uint(buffer[6] ) *  10;
      _ontvangendata.batterij   += char_to_uint(buffer[7] );
      _ontvangendata.kracht      = char_to_uint(buffer[9] ) * 100;
      _ontvangendata.kracht     += char_to_uint(buffer[10]) *  10;
      _ontvangendata.kracht     += char_to_uint(buffer[11]);
      _ontvangendata.snelheid    = char_to_uint(buffer[13]) * 100;
      _ontvangendata.snelheid   += char_to_uint(buffer[14]) *  10;
      _ontvangendata.snelheid   += char_to_uint(buffer[15]);

      
    }
    verbonden->start_timer();
    is_verbonden = true;
    uart_timeout.stop_timer();
    pointer = 0;
    // Zenden in de volgende keer dat handling wordt uitgevoerd.
      _zenddata.nieuwedata = true;
  }
  
////////// AFHANDELEN OVERIGE FUNCTIES //////////
  // Als de verbonden-timer afgelopen is, dan is de UI niet langer verbonden!
  if(verbonden->timeout()){
    verbonden->stop_timer();
    is_verbonden = false;
  }
}

bool xbee::get_UI_verbonden_status()
{
  return this->is_verbonden;
}
unsigned int xbee::get_UI_kracht()
{
  return this->_ontvangendata.kracht;
}
unsigned int xbee::get_UI_storing()
{
  return this->_ontvangendata.storingen;
}
void xbee::set_UI_batterij(unsigned int batterij)
{
  _zenddata.batterij = batterij;
}
void xbee::set_UI_snelheid(unsigned int snelheid)
{
  _zenddata.snelheid = snelheid;
}
void xbee::set_UI_storing(unsigned int storingen)
{
  _zenddata.storingen = storingen;
}
