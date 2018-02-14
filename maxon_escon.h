#ifndef MAXON_ESCON_H
#define MAXON_ESCON_H
#include "Arduino.h"

#define PWM_UIT 0
#define PWM_MIN 25
#define PWM_MAX 230
#define PWM_BEGRENSD 152

/** \brief Class voor de Maxon Escon motorregelaars.
 */
class maxon_escon
{
private:
  unsigned int pin_motorstroom, 
               pin_motorsnelheid, 
               pin_enable,
               pin_pwm,
               pin_status;
               
  bool regelaar_enable = false;
  bool fout = false;
  unsigned int actuele_snelheid_rpm = 0;
  unsigned int setpoint_snelheid_rpm = 0;
  //unsigned int pwm_to_kmh(unsigned int pwm);
  unsigned int stap = 0;
  unsigned int error_teller = 0;
  unsigned int kracht = 0;
  
public:
  /** \brief Constructor. Maakt een instantie aan voor de Maxon Escon-serie regelaars.
   *  \param pin_motorsnelheid Pin defintie. Geef de pin op waar de analoge uitgang voor de motorsnelheid aan gekoppeld is.
   *  \param pin_motorstroom Pin defintie. Geef de pin op waar de analoge uitgang voor de motorstroom aan gekoppeld is.
   *  \param pin_enable Pin defintie. Geef de pin op waar de digitale ingang voor enable aan gekoppeld is.
   *  \param pin_pwm Pin defintie. Geef de pin op waar de pwm-ingang voor de motorsnelheid aan gekoppeld is.
   *  \param pin_status Pin defintie. Geef de pin op waar de status-uitgang aan gekoppeld is.
   */
  maxon_escon(unsigned int pin_motorsnelheid, 
              unsigned int pin_motorstroom, 
              unsigned int pin_enable, 
              unsigned int pin_pwm, 
              unsigned int pin_status);
  /** \brief Handling. Moet altijd aangeroepen worden in de loop.
   */
  void handling();
  /** \brief Setter voor de huidige motorsnelheid.
   *  \param snelheid. Stelt de huidige motorsnelheid in. Gegeven in (RPM)
   */
  void set_snelheid(unsigned int snelheid);
  /** \brief Setter voor de huidige kracht.
   *  \param kracht. De krazcht die de motor moet leveren (0~3) 
   */
  void set_kracht(unsigned int kracht);
  /** \brief Getter voor de huidige motorsnelheid.
   *  \returns unsigned int, huidige snelheid in (RPM)
   */
  unsigned int get_snelheid();
  /** \brief Getter voor een eventuele fout.
   *  \returns boolean, true bij fout, false bij normaal bedrijf
   */ 
  bool get_fout();
  /** \brief Getter voor de huidige motorsnelheid.
   *  \returns unsigned int, huidige snelheid in (km/h*10)
   */
  unsigned int get_snelheid_kmh();
  /** \brief Getter voor de huidige motorstroom.
   *  \returns unsigned int, huidige motorstroom in (A)
   */
/*
  unsigned int get_motorstroom();
  /** \brief Schakelt de motorregelaar in
   */
  void enable();
  /** \brief Schakelt de motorregelaar uit
   */
  void disable();
};
#endif
