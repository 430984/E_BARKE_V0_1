#include "timer.h"
#include "storingen.h"
#include "IO.h"
#include "comm.h"
#include "data_typen.h"
#include "maxon_escon.h"

#define BAUDRATE 9600

int global_storing = ERR_GEEN_STORING;

int main(){
  init();
  pinMode(7, OUTPUT);
  digitalWrite(7, HIGH);
  Serial.begin(9600);
  xbee wireless(XB_TX, XB_RX, BAUDRATE);  
  maxon_escon regelaar(MOT_SPD, MOT_CUR, MOT_EN, MOT_PWM, MOT_STAT);
  
  while (true){
    // Draadloze communicatie afhandelen.
    wireless.handling();
    // Maxon escon regelaar handling
    regelaar.handling();
    
    regelaar.set_kracht(wireless.get_UI_kracht());
    wireless.set_UI_snelheid(regelaar.get_snelheid_kmh());
    
    if(wireless.get_UI_verbonden_status() == true){
      regelaar.enable();
    }
    else{
      regelaar.disable();
    }
/*
    if(regelaar.get_fout() != 0){
      Serial.println("Storing!");
      wireless.set_UI_storing(ERR_MAXON_STORING);
    } 
    */
  }
}
