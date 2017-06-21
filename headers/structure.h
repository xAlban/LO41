#ifndef STRUCTURE_H_INCLUDED
#define STRUCTURE_H_INCLUDED

#include <pthread.h>
#include <unistd.h>

#define AUTONOMIE 30 //batterie du drone
#define ZONE 5
#define CHARGEMAXI 50
#define NB_COULOIR 2

#define BOOL int //boolean
#define TRUE 1
#define FALSE 0

typedef struct Colis{
    int ID_client;
    int priorite;
    int temps; //temps/2 necessaire pour arriver a destination
    int zone;//destination du colis
    int poids; //poids du colis
    int etat; //1 pour colis correct, 2 colis mauvais
    int etatLivraison; // 0 au slot, 1 sur le drone, 2 en attente, 3 livrer, 4 chez le client
}Colis_t;

typedef struct Client{
    int ID;
    int zone;//ou se trouve le client
    int NBColisAttente;//Nbre de colis en attente
    int NBColisRecu;//Nbre de colis qui a recu
    int etat; // 1 pour il est la, 2 pour absent
    int retourColis; // nbre de colis refuser
    Colis_t *colis;
    int couloir[NB_COULOIR]; // couloir[0] aller, couloir[1] retour  (0 pour vide, 1 pour occupe)
    pthread_mutex_t mClient;
    pthread_cond_t cClient;

}Client_t;

typedef struct Drone{
    int ID_drone;
    int slot; //numero du slot correspond au drone
    int status; //-2 pour perdu, -1 pour en charge, 0 pour pret, 1 pour colis sur lui, 2 pour en cours de livraison, 3 pour livrer, 4 pour backhome
    int charge; // poids total qui peut transporte
    int autonomie;//batterie
    int zone; //0 pour dans le vaisseau, -1 pour perdu, et  les autre zones pour aller a destination
    int NBColisLivre;//nbre de colis livre
		int NBColisRetour;//nbre de colis que le client a refuser et que le drone la repris
    Colis_t colis; // colis correspond pendant la livraison du drone
}Drone_t;

typedef struct Slot{
    int NBColisSlot;// nbre de colis dans un slot
    Colis_t *colis;
}Slot_t;

typedef struct Vaisseau{
    Slot_t *slot;
    int NBColis;//nbre total de colis
    int NBDroneAttente;// nbre de drone qui ne travail pas
    int NBSlot;//nbre de slot
    int NBDroneTravail;//nbre de drone qui est en cours de livraison
    int NBDronePerdu;//nbre de drone perdu
		int Status; //0 sur terre, 1 en l'air
		int NBDroneRepos;//nbre de drone qui se recharge
		int NBColisRetour;//nbre total de colis refuser par les clients
    pthread_mutex_t mVaisseau;
    pthread_cond_t cVaisseau;
}Vaisseau_t;

typedef struct Data{
		int nbColis;
		int nbDrone;
		int nbClient;
		int nbSlot;
		int nbColisMax;
}Data_t;

Data_t data;
Vaisseau_t vaisseau;
Client_t *client;
Drone_t *drone;

#endif // STRUCTURE_H_INCLUDED
