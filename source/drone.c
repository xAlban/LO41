#include "drone.h"
#include "fonctions.h"

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
        drone.status = 1;
        //sleep();
        drone.status = 0;
    }
}

Drone_t prendreColis(Drone_t drone, Colis_t Colis){
    if(drone.charge>=Colis.poids){
        if(drone.autonomie>=Colis.temps){
            drone.colis = Colis;
            drone.status = 0;
            vaisseau.NBColis--;
        }
    }
    return drone;
}

void* fonction_drone(void* arg){
    //int i = (int) arg;
    //drone[i] = Init_drone(i, drone[i]);
    pthread_exit(NULL);
}
