//librairies
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>


//Sorties
Servo moteur; //créer un object avec "servo" pour controler l'esc
Servo servo; // sortie pour controle servo


//radio
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";


//Pack permettant de stocker toutes les données que l'on souhaite envoyer
struct Data_Package {
  byte Emoteur;
  byte Eservo;
};
Data_Package data; //Création d'une variable data a partir du pack ci-dessus


//données de temps
unsigned long lastReceiveTime = 0;
unsigned long currentTime = 0;


//declare the motors variables
int motorValue = 0;
int servoValue = 0;





void setup() {
  Serial.begin(9600);


  //radio
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setAutoAck(false);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();


  //Attribuer au moteur et au servo des pins de sorties et leur valeurs min et max
  moteur.attach(6);
  servo.attach(5);

  
  delay(5000);
}





void loop () {

  //lecture du contenu du pack transmis par la radio
  if (radio.available()) {
    Serial.print("R ");
    radio.read(&data, sizeof(Data_Package)); //stocker le contenu dans "data"
    lastReceiveTime = millis(); //temps de recevoir toute la data*/
  }


  //reset de la data si perte de signal de plus d'une seconde
  currentTime = millis();
  if ( currentTime - lastReceiveTime > 1000 ) {
    data.Emoteur = 0;
    data.Eservo = 127;
  }


  //Controle du moteur
  motorValue = map(data.Emoteur, 0, 255, 1515, 680);
  moteur.write(motorValue);


  //Controle du servo
  servoValue = map(data.Eservo, 0, 255, 65, 115);
  servo.write(servoValue);


  //affichage des valeurs dans le moniteur série
  Serial.print("Moteur: ");
  Serial.print(motorValue);
  Serial.print("  Servo: ");
  Serial.println(servoValue);
}
