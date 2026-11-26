#include <Arduino.h>

int etat = 0;           // 0: repos, 1: ramassage de volants, 2: gestion du bac
int etatRamassage = 2;  // 2: roaming/scan, 3: récupération du volant
int etatBac = 1;        // Sous-états pour l'échange de bac

// put function declarations here:
int myFunction(int, int);

void setup()
{
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
}

void loop() {
  // put you main code here, to run repeatedly:sadasdasdasdasdad
} 

void cases(int num_case) {


switch (etat) {
  case 0: //repos, on attend le signal de la telecommande
    // Quand le signal est reçu :
    // etat = 1;
    // etatRamassage = 2;
    break;

  case 1: // Ramassage de volants
    switch (etatRamassage) {
      case 2:
        //roaming habituel avec scans intermittent pour les volants
        avancer(1); //on avance d'une distance, la fonction avancer devrait checker pour la bordure aussi(imu ou detecteur ou les 2)


        if (scanVolants()>0) {
          etatRamassage = 3; //on scanne pour un volant, si on en trouve un, on va au case 3, on doit avoir la distance du volant en sortie de la fonction, si aucun volant est trouvé, la fct retoune 0
        } else {
          etat = 0; //si on ne trouve pas de volant, on retourne en mode repos
        }
        break;

      case 3: //on a trouvé un volant, on va le chercher
        avancer(dist_volant); //on avance vers le volant, peut etre quon avance jusqu'à ce qu'on le détecte dans la pince
        fermer_pince(); //on ferme la pince pour attraper le volant
        lever_pince(); 
        ouvrir_pince();
        tourner(180); //on se tourne vers notre posisition initiale, la fonction tourner doit églamenet modifier un variable dangle pour pouvoir revenir à l'angle initial
        avancer(dist_volant); //on avance vers notre position initiale
        tourner(angle_initial); //on se tourne vers notre position initiale

        if (nb_volants > 4) {
          etat = 2; //echange de bac (on passe au switch du bac)
          etatBac = 1; //on initialise le sous-état du bac
        } else {
          etatRamassage = 2; //on retourne en mode roaming
        }
        break;
    }
    break;

  case 2: // Gestion / échange de bac
    switch (etatBac) {
      case 1:
        tourner((angle_bac-angle_actuel)); //on tourne vers le bac || Pour cette section je ne sais honnetement pas quelle est la meilleure facon de se rendre au bac
        avancer(dist_bac); //on avance vers les bacs

        //on va a la position du bac x, qu'on trouve avec j'imagine le detecteur de couleur, il faut que les bacs soient dans des coins opposés
        deposer_bac();// on depose le bac à son endroit
          
        //ensuite on va vers l'autre bac

        scan_bac(); //on trouve le bac avec le capteur

        alignement_bac(); //on s'aligne avec le bac selon les donées du capteur
        prendre_bac(); //On le ramasse le petit tabarnak
        break;

      case 2:
        // Étape supplémentaire pour le bac si nécessaire
        break;
    }

    case 3: //controle manuel


    break;
}