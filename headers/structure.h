#ifndef STRUCTURE_H_INCLUDED
#define STRUCTURE_H_INCLUDED

#include <pthread.h>
#include <unistd.h>

#define NB_DRONE 10 //nombre de drone
#define NB_COLIS 5 //nombre de colis
#define AUTONOMIE 30 //capacite d'autonomie du drone
#define NB_CLIENT 10
#define NB_SLOT NB_DRONE+1 // nombre de slot et le dernier slot correspond au colis dommagee ou tromper
#define ZONE 3
#define NBColisMax (NB_SLOT-1) * NB_COLIS
#define CHARGEMAXI 50
#define NB_COULOIR 2

#define BOOL int //boolean
#define TRUE 1
#define FALSE 0

typedef struct Colis{
    int ID_client;
    int priorite;
    int temps; //temps/2 necessaire pour arriver a destination
    int zone;
    int poids; //poids du colis
    int etat; //1 pour colis correct, 2 colis mauvais
    int etatLivraison; // 0 au slot, 1 sur le drone, 2 en attente, 3 livrer, 4 chez le client
}Colis_t;

typedef struct Client{
    int ID;
    int zone;
    int NBColisAttente;
    int NBColisRecu;
    int etat; // 1 pour il est la, 2 pour absent
    int retourColis;
    Colis_t *colis; //COLISMAX
    int couloir[NB_COULOIR]; // couloir[0] aller, couloir[1] retour  (0 pour vide, 1 pour occupe)
    pthread_mutex_t mClient;
    pthread_cond_t cClient;

}Client_t;

typedef struct Drone{
    int ID_drone;
    int slot;
    int status; //-2 pour perdu, -1 pour en charge, 0 pour pret, 1 pour colis sur lui, 2 pour en cours de livraison, 3 pour livrer, 4 pour backhome
    int charge; // poids total qui peut transporte
    int autonomie;
    int zone; //0 pour dans le vaisseau, -1 pour perdu, et  les autre zones pour aller a destination
    int NBColisLivre;
		int NBColisRetour;
    Colis_t colis; // pour donner un colis au drone
}Drone_t;

typedef struct Slot{
    int NBColisSlot;
    Colis_t *colis;
}Slot_t;

typedef struct Vaisseau{
    Slot_t *slot;
    int NBColis;
    int NBDroneAttente;
    int NBSlot;
    int NBDroneTravail;
    int NBDronePerdu;
		int Status; //0 sur terre, 1 en l'air
		int NBDroneRepos;
		int NBColisRetour;
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
