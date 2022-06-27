/*
Créé le 18/08/2020
Vidéos original pour code radio :
https://www.youtube.com/watch?v=7rcVeFFHcFM&feature=youtu.be
https://www.youtube.com/watch?v=-BDCmwNssiw&feature=youtu.be&t=635
*/


//librairies
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>


//Sorties
Servo Smoteur; //créer un object avec "servo" pour controler l'esc
Servo Sservo; // sortie pour controle servo


//radio
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";


//Pack permettant de stocker toutes les données que l'on souhaite envoyer
struct Data_Package {
  byte Emoteur;
  byte Eservo;
};

Data_Package data; //Création d'une variable a partir du pack ci-dessus


//données de temps
unsigned long lastReceiveTime = 0;
unsigned long currentTime = 0;



void setup() {
Serial.begin(9600);


//radio
radio.begin();
radio.openReadingPipe(0, address);
radio.setAutoAck(false);
radio.setDataRate(RF24_250KBPS);
radio.setPALevel(RF24_PA_MAX);
radio.startListening();


//Attribuer au moteur et au servo des numéros de sorties et leur valeurs min et max
Smoteur.attach(6,1000,1900);
Sservo.attach(5);


resetData();
}



void loop () {

//lecture du contenu du pack transmis par la radio
 if (radio.available()) {
    Serial.print("oui ");
    radio.read(&data, sizeof(Data_Package)); //stocker le contenu dans "data"
    lastReceiveTime = millis(); //temps de recevoir toute la data*/
 }


//reset de la data si perte de signal de plus d'une seconde
 currentTime = millis();
 if ( currentTime - lastReceiveTime > 1000 ) {
  resetData();
}


//Controle du moteur
Smoteur.write(map(data.Emoteur,0,255,0,180));

//Controle du servo
Sservo.write(map(data.Eservo,0,255,65,115));
/*setAngle(data.Eservo) ; // utiliser la loop servo pour envoyer le bon signal correspondant*/


//affichage des valeurs dans le moniteur série
Serial.print("Moteur :");
Serial.print(data.Emoteur); 
Serial.print("               Servo  :");
Serial.println(data.Eservo); 

delay(10);
}



//loop d'envoi de données lors du démarrage et en cas de coupure 
void resetData() {
  data.Emoteur = 0;
  data.Eservo = 127;
}
