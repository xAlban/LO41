#include "drone.h"

/*Fonction qui initialise les colis*/
Drone_t Init_drone(int i, Drone_t drone){
    drone.slot = i;
    drone.status = 0;
    drone.charge = get_random(50);
    drone.zone = 0;
    printf("Drone %d prend le slot %d et a pour charge maximal %d kg\n", i, drone.slot, drone.charge);
    return drone;
}

/*Fonction pour que le drone se recharge son autonomie*/
void recharger(Drone_t drone){
    if(drone.autonomie == 0){
        pthread_mutex_lock(&mDrone);
        drone.status = 1;
        Sleep(5000);
        drone.status = 0;
        pthread_mutex_unlock(&mDrone);
    }
}

/*Fonction pour prendre le colis du slot*/
Drone_t prendreColis(Drone_t drone, Colis_t Colis){
    if(drone.charge>=Colis.poids){
        if(drone.autonomie>=Colis.temps){
            pthread_mutex_lock(&mColis);
            drone.colis = Colis;
            drone.status = 0;
            vaisseau.NBColis--;
            pthread_mutex_unlock(&mColis);
        }
    }
    return drone;
}

void* fonction_drone(void* arg){
    int i = (int) arg;
    while(1){
        pthread_mutex_lock(&mDrone);
        if(vaisseau.NBColis<=0){
            pthread_cond_signal(&cVaisseau);
            pthread_cond_wait(&cDrone, &mVaisseau);
        }
        vaisseau.NBColis--;
        printf("Drone %d a pris un colis, la il reste %d colis\n", i, vaisseau.NBColis);

        pthread_mutex_unlock(&mDrone);
    }
    pthread_exit(NULL);
}
