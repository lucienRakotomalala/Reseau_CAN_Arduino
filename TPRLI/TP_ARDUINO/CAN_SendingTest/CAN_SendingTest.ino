#include <due_can.h>
#include <string.h>
//Reads all traffic on CAN0 and forwards it to CAN1 (and in the reverse direction) but modifies some frames first.
// Required libraries
#include "variant.h"

//Leave defined if you use native port, comment if using programming port
#define Serial SerialUSB

void setup()
{

  Serial.begin(115200);
  // Initialize CAN0 and CAN1, Set the proper baud rates here
  Can0.begin(CAN_BPS_500K);
  Can1.begin(CAN_BPS_500K);

  int filter;
  //extended
  for (filter = 0; filter < 3; filter++) {
    Can0.setRXFilter(filter, 0, 0, true);
    Can1.setRXFilter(filter, 0, 0, true);
  }
  //standard
  for (int filter = 3; filter < 7; filter++) {
    Can0.setRXFilter(filter, 0, 0, false);
    Can1.setRXFilter(filter, 0, 0, false);
  }

  Can0.watchFor();
}

/**********************  Réceptionet affichage Message reçus     ******/
void printFrame(CAN_FRAME &frame) {
  Serial.print("ID: 0x");
  Serial.print(frame.id, HEX);
  Serial.print(" Len: ");
  Serial.print(frame.length);
  Serial.print(" Data: 0x");
  for (int count = 0; count < frame.length; count++) {
    Serial.print(frame.data.bytes[count], HEX);
    Serial.print(" ");
  }
  Serial.print("\r\n");
}

/**********************  Envoie Message     ******/

void sendData()
{
  CAN_FRAME outgoing;
  outgoing.id = 0x400;
  outgoing.extended = false;
  outgoing.priority = 4; //0-15 lower is higher priority

  outgoing.data.s0 = 0xDADA;
  outgoing.data.byte[2] = 0xDD;
  outgoing.data.byte[3] = 0x55;
  outgoing.data.high = 0xDEADBEEF;
  outgoing.length = sizeof(outgoing.data);
  
  Can0.sendFrame(outgoing);
}


/**********************  loop     ******/
void loop()
{
  CAN_FRAME incoming;
  static unsigned long lastTime = 0;
  int unsigned long currentTime = 0; //mod
  /*
    if (Can0.available() > 0) {
  	Can0.read(incoming);
  	Can1.sendFrame(incoming);
        Serial.println("Message reçu sur CAN0");
     }
    if (Can1.available() > 0) {
  	Can1.read(incoming);
  	Can0.sendFrame(incoming);
        Serial.println("Message reçu sur CAN1");
    }
    */
  /// affchage temps debbug
  // currentTime=millis();
  // Serial.print("curentTime = ");
  // Serial.print(currentTime);
  // Serial.print(" lastTime= ");
  // Serial.println(lastTime);


  // Envoie a fréquence fixe (1/4 s)
  if ((millis() - lastTime) > 4000)
  {
    lastTime = millis();
    Serial.println("Envoie Message");

    sendData();
    Serial.println(Can0.available());
    Serial.println(Can1.available());
  }


  //CAN_FRAME incoming;
  //Serial.println(Can0.available());
  //Serial.println(Can1.available());

  if (Can0.available() > 0)
  {
    Serial.println(Can0.available());
    Serial.println(Can1.available());
    Can0.read(incoming);
    printFrame(incoming);
  }
  if (Can1.available() > 0)
  {
    Can1.read(incoming);
    printFrame(incoming);
  }
  //  delay(10000);
  //  sendData();
}


