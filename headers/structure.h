#ifndef STRUCTURE_H_INCLUDED
#define STRUCTURE_H_INCLUDED

#include <pthread.h>

#define NB_DRONE 2 //nombre de drone
#define NB_COLIS 2 //nombre de colis
#define AUTONOMIE 30 //capacite d'autonomie du drone
#define NB_CLIENT 2
#define NB_SLOT NB_DRONE+1 // nombre de slot et le dernier slot correspond au colis dommagee ou tromper
#define ZONE 2

#define BOOL int //boolean
#define TRUE 1
#define FALSE 0


typedef struct Client{
    int ID;
    int zone;
}Client_t;

typedef struct Colis{
    int ID_client;
    int priorite;
    int temps; //temps necessaire pour arriver a destination
    int zone;
    int poids; //poids du colis
}Colis_t;

typedef struct Drone{
    int slot;
    int status; //-2 pour perdu, -1 pour en charge, 0 pour pret, 1 pour dans le vaisseau, 2 pour en cours de livraison, 3 pour livrer
    int charge; // poids total qui peut transporte
    int autonomie;
    int zone; //0 pour dans le vaisseau, -1 pour perdu
    Colis_t colis; // pour donner un colis au drone
}Drone_t;

typedef struct Slot{
    int NBColisSlot;
    Colis_t colis[NB_COLIS];
}Slot_t;

typedef struct Vaisseau{
    Slot_t slot[NB_DRONE];
    int NBColis;
    int NBDroneAttente;
    int NBSlot;
    int NBDroneTravail;
    int NBDronePerdu;
	int Status; //0 sur terre, 1 en l'air
	int NBDroneRepos;
}Vaisseau_t;

pthread_mutex_t mVaisseau, mClient, mDrone, mColis;
pthread_cond_t cVaisseau, cClient, cDrone, cColis;

Vaisseau_t vaisseau;
Client_t client[NB_CLIENT];
Drone_t drone[NB_DRONE];

#endif // STRUCTURE_H_INCLUDED
