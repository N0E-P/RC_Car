//Créé le 18/08/2020


//librairies
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


//radio
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";


//Pack permettant de stocker toutes les données que l'on souhaite envoyer
struct Data_Package {
  byte Emoteur;
  byte Eservo;
};

Data_Package data ; //Création d'une variable a partir du pack ci-dessus



void setup() {
  Serial.begin(9600);

  //radio communication
  radio.begin();
  radio.openWritingPipe(address);
  radio.setAutoAck(false);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.stopListening();

  //Données pré-définies pour les variables Moteur et ServoMoteur
  data.Emoteur = 0;
  data.Eservo = 127;

}



void loop () {

  //Lire les entrées analogiques, les convertir entre 0 et 255 puis les transféré en Byte value dans le datapack
  data.Emoteur = map(analogRead(A1), 0, 1023, 0, 255);
  data.Eservo = map(analogRead(A0), 0, 1023, 0, 255);


  //envoyer toute la data a partir de la structure au recepteur
  radio.write(&data, sizeof(Data_Package));


  //test si l'arduino détecte les potentiommètre ou pas
  Serial.print("Moteur: ");
  Serial.print (data.Emoteur);
  Serial.print ("  Servo: ");
  Serial.println (data.Eservo);


  delay(5);
}
