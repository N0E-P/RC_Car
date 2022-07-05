
#include <Servo.h>
Servo moteur;


void setup() {
  moteur.attach(6);
  delay(5000);
}


void loop() {
  int motorValue = map(analogRead(A0),0,1023, 1515, 680);
  moteur.write(motorValue);
}
