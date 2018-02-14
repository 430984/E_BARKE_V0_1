#ifndef COMM_H
#define COMM_H
#include "Arduino.h"
#include "data_typen.h"
#include "timer.h"
#include <SoftwareSerial.h>

#define POLL_TIJD 1000          // Tijd (ms) die tussen pogingen zit met verbinden
#define VERBONDEN_TIMEOUT 5000  // De tijd (ms) die de module verbonden blijft vanaf de laatste communicatie

/** \brief class xbee. De klasse voor de communicatie tussen module en UI. Regelt intern alle inkomende en uitgaande communicatie, timers voor wel of niet verbonden,
 *  data encodering en decodering naar getters en setters, etc.
*/
class xbee{
private:
  unsigned int char_to_uint(char c);
  SoftwareSerial *xb;
  timer *verbinden;
  timer *verbonden;
  UART_data _zenddata;
  UART_data _ontvangendata;
  int verbinding_houden();
  bool is_verbonden = false;

// Rekening houden met REST-libraries bij functies! 
public:
  /** \brief Constructor, maakt een instantie aan voor de communicatie tussen de module en de UI
   *  \param int _rxpin, het nummer van de pin waar de RX (DOUT) van de XBEE op aangesloten is. Elke willekeurige pin op de arduino kan gebruikt worden
   *  \param int _txpin, het nummer van de pin waar de TX (DIN) van de XBEE op aangesloten is. Elke willekeurige pin op de arduino kan gebruikt worden
   *  \param unsigned long baudrate, de baudrate van de communicatie
   */
  xbee(int _rxpin, int _txpin, unsigned long _baudrate){
    xb = new SoftwareSerial(_rxpin, _txpin);
    xb->begin(_baudrate);
    verbinden = new timer(POLL_TIJD, true);   // Nieuwe timer aanmaken en deze gelijk starten (vandaar 'true')
    verbonden = new timer(VERBONDEN_TIMEOUT);
  }
  /** \brief Handling. Deze functie moet altijd bovenaan in de main blijven. Regelt alle communicatie, zowel inkomend als uitgaand
   */
  void handling();
  /** \brief Getter voor de verbonden-status
   *  \returns boolean, verbonden met de UI (true) of niet verbonden (false)
   */
  bool get_UI_verbonden_status();
  /** \brief Getter voor de kracht
   *  \returns unsigned integer, kracht
   */
  unsigned int get_UI_kracht();
  /** \brief Getter voor een eventuele storing
   *  \returns unsigned integer, storingscode (0 = geen storing)
   */
  unsigned int get_UI_storing();
  /** \brief Setter voor het batterijniveau.
   *  \param unsigned integer, batterijniveau (in procenten)
   */
  void set_UI_batterij(unsigned int batterij);
  /** \brief Setter voor de weer te geven snelheid.
   *  \param unsigned integer, batterijniveau (in km/h * 10)
   */
  void set_UI_snelheid(unsigned int snelheid);
  /** \brief Setter voor de storingen.
   *  \param unsigned integer, storingscode
   */
  void set_UI_storing(unsigned int storingen);
};

#endif

