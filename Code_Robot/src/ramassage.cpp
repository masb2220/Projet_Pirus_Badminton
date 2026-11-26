#include "Arduino.h"
#include "LibRobus.h"
#include "Servo.h"

Servo barrel;
Servo pince_gauche;
Servo pince_droite;

int pin_servo_barrel = 9; // pin servo scoop
int pin_servo_pince_gauche = 10; //pin servo pince gauche
int pin_servo_pince_droite = 11; //pin servo pince droite

int angle_initial_servo_barrel = 0; //angle initial du servo scoop
int angle_initial_servo_pince_gauche = 0; //angle initial du servo pince gauche
int angle_initial_servo_pince_droite = 0; //angle initial du servo pince droite

int angle_final_servo_barrel = 90; //angle final du servo scoop
int angle_final_servo_pince_gauche = 90; //angle final du servo pince gauche
int angle_final_servo_pince_droite = 90; //angle final du servo pince droite


void setup(){
barrel.attach(pin_servo_barrel); //attacher les servos à la librairie servo
pince_gauche.attach(pin_servo_pince_gauche); //attacher les servos à la librairie servo
pince_droite.attach(pin_servo_pince_droite); //attacher les servos à la librairie servo
}

void ouvrir_pince() {
    barrel.write(angle_final_servo_barrel); //ouvrir le scoop   
}

void fermer_pince() {
    barrel.write(angle_initial_servo_barrel); //fermer le scoop
}

void lever_pince() {
    pince_gauche.write(angle_final_servo_pince_gauche); //lever la pince gauche
    pince_droite.write(angle_final_servo_pince_droite); //lever la pince droite
}

void baisser_pince() {
    pince_gauche.write(angle_initial_servo_pince_gauche); //baisser la pince gauche
    pince_droite.write(angle_initial_servo_pince_droite); //baisser la pince droite
}