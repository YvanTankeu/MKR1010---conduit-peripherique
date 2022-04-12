/*
  Titre      : Vitesse deplament conduit de déchargement
  Auteur     : Yvan Tankeu
  Date       : 12/04/2022
  Description: examen final, Vitesse deplament conduit de déchargement - Peripherique MKR1010
  Version    : 0.0.1
*/

#include <Arduino.h>
#include <Wire.h> // Pour la communication I2C
#include <Servo.h>// Include servo biblio
#include "WIFI_NINA_Connector.h"
#include "MQTTConnector.h"


#define peri_addr 9

const int pinLED = 2;
const int pinServo = 1;

int pos = 0;    // variable pour stocker la position du servo
int posPrecedent =  0;

char receivedData; // Val de reception de la donnée du controler

bool etat, etatPrecedant =  0;


Servo myservo;  // créer un objet servo pour contrôler un servo

// fonction à exécuter lorsque des données sont reçues du periphérique
void receiveEvent(int bytes) {
  receivedData = Wire.read();    // lire un caractère à partir de l'I2C 
  /*Serial.print("valeur du clavier");
  Serial.println(receivedData);
  Serial.print("position servo");
  Serial.println(pos);*/

  switch (receivedData)
  {
    // si on presse sur A,B,C ou D
    case 'A':
      pos = 180; 
      break;
    case 'D':
      pos = 0;
      break;
    case 'B':
      if (pos >= 180 ){
        pos = 180;
      }else{
        pos++;
      }
      break;
    case 'C': 
      if (pos <= 0 ){
          pos = 0;
        }else{
          pos--;
        }
      break;
  }
}

void setup() {

  wifiConnect(); // Branchement au réseau WIFI
  MQTTConnect(); // Branchement au broker MQTT
  myservo.attach(pinServo);  // attache le servo sur la broche 1 à l'objet servo
  pinMode(pinLED, OUTPUT);
  
  // Démarrer le bus I2C comme periphérique à l'adresse 9
  Wire.begin(peri_addr); 

  // Attachez une fonction qui se déclenche lorsque une donée est reçue
  Wire.onReceive(receiveEvent);

  Serial.begin(9600);

}

void loop() {
  
  if(pos != 0){
    digitalWrite(pinLED, etat);
  } 
  if(pos != posPrecedent){
    
    myservo.write(pos);
    appendPayload("position", pos);
    sendPayload();  
    posPrecedent = pos; //Conserve la position
  }
}
