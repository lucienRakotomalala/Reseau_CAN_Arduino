// a pwm command for motor

// Plan d'action ; 
/* 1. init 
 *    CAN : une boite au lecture en lecture avec un filtre précis.
 *    PWM : un bit en output  
 *          le pwm en 12 bits
 *    
 * 2. function float readCAN() (float entre 0 et 100)
 * 
 * 3. fonction void WritePWM(byte RapportCyclique) (entre 0 et 255)
 * 
 * 
 */
 // lib
 
//1.CAN
#include <due_can.h>
#include <string.h>
#include "variant.h"
//1.PWM
#include <Servo.h>
Servo servo;

  // define
//1.CAN
#define MAILBOX 0
#define ID 0x400
#define MSGINDEX 7
// 1.PWM
#define PWMPIN 2


/***************************************** fonction PWM **************************************/
void writePWM(int  CycleRatio) /*   -90 -- 90   */
{
      servo.write(90 + CycleRatio ) ; // envoie le msg (0 -- 180 )
      Serial.print("angle en degré :");
      Serial.println(90 + CycleRatio );
}


/***************************************** fonction CAN **************************************/
int readCAN(CAN_FRAME &MSGread) /*   -90 -- 90  */
{
  
  int angle=0;
  unsigned int readValue = MSGread.data.bytes[MSGINDEX];
  angle = 180/255*int(readValue) - 90;
  return angle;
}





/***************************************** SETUP **************************************/
void setup() {
    Serial.begin(115200);
    Serial.println("Début Init");

  // 1.CAN
      Can0.begin(CAN_BPS_500K);// vitesse can
      
      // definition d'une mailbox (indice MAILBOX) qui filtre les message avec un identifiant (ID), 
      // message de type standard
      Can0.setRXFilter(MAILBOX, ID, 0, false); 

      Can0.watchFor();
  // 1.PWM
      servo.attach(PWMPIN);
      Serial.println("Fin Init");
}
/***************************************** LOOP **************************************/
void loop() 
{
  Serial.println("loop");
  static unsigned int ratioCycle= 0;
  CAN_FRAME frameRead ;
   /* 1. si msg reçu : 
   *              le lire msg
   *              actualiser cycle
   *  2. écrire PWM
   *              
   */              
    if (Can0.available() > 0) 
    {
        Can0.read(frameRead);
        ratioCycle = readCAN(frameRead);
    }

  writePWM(ratioCycle);
}
