#include "drone.h"

/*Fonction qui initialise les colis*/
Drone_t Init_drone(int i, Drone_t drone){
    drone.ID_drone = i;
    drone.slot = i;
    drone.status = 0;
    drone.charge = get_random(50, 1);
    drone.zone = 0;
    printf("Drone %d prend le slot %d et a pour charge maximal %d kg\n", drone.ID_drone, drone.slot, drone.charge);
    return drone;
}

/*Fonction pour que le drone se recharge son autonomie*/
void recharger(Drone_t drone){
    pthread_mutex_lock(&mDrone);
    pthread_cond_wait(&cDrone, &mDrone);
    if(drone.autonomie == 0){
        drone.status = 1;
        Sleep(5000);
        drone.status = 0;
    }
    pthread_mutex_unlock(&mDrone);
}

/*Fonction pour prendre le colis du slot*/
Drone_t prendreColis(Drone_t drone, Colis_t Colis){
    pthread_mutex_lock(&mColis);
    pthread_cond_wait(&cColis, &mColis);
    if(drone.charge>=Colis.poids){
        if(drone.autonomie>=Colis.temps){
            drone.colis = Colis;
            drone.status = 0;
            drone.zone = drone.colis.zone;
            vaisseau.NBColis--;
        }
    }
    pthread_mutex_unlock(&mColis);
    return drone;
}

void descendDrone(Drone_t drone){
    pthread_mutex_lock(&mDrone);
    Sleep(5000);
    pthread_mutex_unlock(&mDrone);
}

void monterDrone(Drone_t drone){
    pthread_mutex_lock(&mDrone);
    Sleep(5000);
    drone.status = 4;
    pthread_mutex_unlock(&mDrone);
}

 void rentrerDrone(Drone_t drone){
    pthread_mutex_lock(&mDrone);
    Sleep(5000);
    drone.status = 1;
    pthread_mutex_unlock(&mDrone);
 }

void livrerColis(Drone_t drone){
    pthread_mutex_lock(&mColis);
    vaisseau.NBDroneTravail++;
    if(client[drone.colis.ID_client].etat==1){
        descendDrone(drone);
    }else{
        rentrerDrone(drone);
    }
    pthread_mutex_unlock(&mColis);
}

void* fonction_drone(void* arg){
    Drone_t *drone = (Drone_t*) arg;
    while(1){
        pthread_mutex_lock(&mDrone);
        if(vaisseau.NBColis<=0){
            pthread_cond_signal(&cVaisseau);
            pthread_cond_wait(&cDrone, &mVaisseau);
        }
        vaisseau.NBColis--;
        printf("Drone %d a pris un colis, la il reste %d colis\n", drone->ID_drone, vaisseau.NBColis);

        pthread_mutex_unlock(&mDrone);
    }
    pthread_exit(NULL);
}
