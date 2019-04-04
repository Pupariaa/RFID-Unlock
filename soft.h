//Pour commencer, il faut ajouter la librairie SPI, RFID ainsi que Keyboard.
//Ce système nessesite obligatoirement un arduino avec un ATMEGA 32U4 soit un arduino LEONARDO.
//Maxime ZORLONI
//04.04.2019

#include <SPI.h> //Intégration de la librairie SPI
#include <RFID.h> //Intégration de la librairie RFID
#include "Keyboard.h" //Intégration de la librairie Keyboard

RFID monModuleRFID(10,9); //Définition des pin MISO et MOSI du lecteur RFID

int UID[5]={}; //Définition du nombre de badge reconnu
int MASTERKEY[5]={249,142,189,85,159}; //Définition du badge valide

void setup()
{
  Serial.begin(9600); //Créaation de la comuunication série avec le moniteur
  SPI.begin(); //Création de la communication série entre le lecteur et l'arduino
  monModuleRFID.init(); 
}

void loop()
{
    if (monModuleRFID.isCard()) {  //Si mon module RFID reçois une information
          if (monModuleRFID.readCardSerial()) {  //Si c'est une carte       
            Serial.print("L'UID est: "); // On affiche dans le moniteur série son UID
            for(int i=0;i<=4;i++) // Là c'est jute pour la mise en forme x)
            {
              UID[i]=monModuleRFID.serNum[i];
              Serial.print(UID[i],DEC);
              Serial.print(".");
            }
            Serial.println("");//On envoie tout au moniteur série
          }
          
          if (UID[0] == MASTERKEY[0] //Si l'UID ne correspond pas à la masterkey 0
           && UID[1] == MASTERKEY[1] //ou la 1
           && UID[2] == MASTERKEY[2] //ou la 2
           && UID[3] == MASTERKEY[3] //ou la 3
           && UID[4] == MASTERKEY[4])//ou la 4 mais la 5, on envoie le code
          {
             
              Keyboard.write((char) 0x20); //On fait déjà un espace pour soulever le bandeau de veille
              delay(150); // On attend 150mili seconde
              Keyboard.println(" "); //On marque le mot de passe (attention c'est du querty)
              Keyboard.releaseAll(); //On rénitialise tout
              delay(500); //On attend 500ms pour pas marquer trop de fois le code et tout faire buger
          }
          else //Sinon 
          {
              //Bah on fait rien
          }          
          monModuleRFID.halt(); //On dit à module de plus rien envoyer concernant cette carte, jusqu'à la nouvelle détection
    }

}
