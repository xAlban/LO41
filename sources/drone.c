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

void recharger(Drone_t drone){
    if(drone.autonomie == 0){
        pthread_mutex_lock(&drone.mDrone);
        drone.status = 1;
        Sleep(5000);
        drone.status = 0;
        pthread_mutex_unlock(&drone.mDrone);
    }
}

Drone_t prendreColis(Drone_t drone, Colis_t Colis){
    if(drone.charge>=Colis.poids){
        if(drone.autonomie>=Colis.temps){
            pthread_mutex_lock(&Colis.mColis);
            drone.colis = Colis;
            drone.status = 0;
            vaisseau.NBColis--;
            pthread_mutex_unlock(&Colis.mColis);
        }
    }
    return drone;
}

void* fonction_drone(void* arg){
    int i = (int) arg;
    pthread_mutex_lock(&drone[i].mDrone);
    pthread_mutex_unlock(&drone[i].mDrone);
    pthread_exit(NULL);
}
