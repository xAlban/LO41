#include "../headers/drone.h"

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
void recharger(Drone_t *drone){
    pthread_mutex_lock(&drone->mDrone);
    //pthread_cond_wait(&cDrone, &mDrone);
    if(drone->autonomie == 0){
        drone->zone = 0;
        //Sleep(10*AUTONOMIE);
        sleep(10*AUTONOMIE);
        drone->status = 0;
    }
    pthread_mutex_unlock(&drone->mDrone);
}

/*Fonction pour prendre le colis du slot*/
void prendreColis(Drone_t *drone, Colis_t Colis){
    pthread_mutex_lock(&mColis);
    //pthread_cond_wait(&cColis, &mColis); ou mettre le signal pour qu'il s'active ?
    if(drone->charge>=Colis.poids){
        if(drone->autonomie>=Colis.temps){
            drone->colis = Colis;
            drone->status = 1;
            drone->zone = drone->colis.zone;
        }
    }
    pthread_mutex_unlock(&mColis);
}

void descendDrone(Drone_t *drone){
    pthread_mutex_lock(&drone->mDrone);
    //Sleep(2000);
    sleep(2000);
    pthread_cond_signal(&client[drone->colis.ID_client].cClient);
    pthread_mutex_unlock(&drone->mDrone);
}

void monterDrone(Drone_t *drone){
    pthread_mutex_lock(&drone->mDrone);
    pthread_cond_wait(&drone->cDrone, &client[drone->colis.ID_client].mClient);
    //Sleep(2000);
    sleep(2000);
    drone->status = 4;
    pthread_mutex_unlock(&drone->mDrone);
}

 void rentrerDrone(Drone_t *drone){
    pthread_mutex_lock(&drone->mDrone);
    //Sleep(10*(drone->colis.temps/2));
    sleep(10*(drone->colis.temps/2));
    vaisseau.NBDroneTravail--;
    if(drone->colis.etat==2){
        if(vaisseau.NBColisRetour!=0){
            vaisseau.NBColisRetour++;
        }
        vaisseau.slot[NB_SLOT].colis[vaisseau.NBColisRetour] = drone->colis;
        vaisseau.slot[NB_SLOT].NBColisSlot++;
    }
    drone->status = 1;
    pthread_mutex_unlock(&drone->mDrone);
 }

void DecrementerTotalColis(int idDrone){
    vaisseau.NBColis--;
    vaisseau.slot[idDrone].NBColisSlot--;
}

void donneColis(Drone_t *drone){
    if(client[drone->colis.ID_client].etat==1){
        descendDrone(drone);
        monterDrone(drone);
    }
    rentrerDrone(drone);
}

void livrerColis(Drone_t *drone){
    pthread_mutex_lock(&mColis);
    vaisseau.NBDroneTravail++;
    //Sleep(10*(drone->colis.temps/2));
    sleep(10*(drone->colis.temps/2));
    donneColis(drone);
    pthread_mutex_unlock(&mColis);
}

void* fonction_drone(void* arg){
    Drone_t *drone = (Drone_t*) arg;
    int idDrone = pthread_self() - 1;
    int i = 0; // pointer sur le colis du slot
    while(vaisseau.slot[idDrone].NBColisSlot>0){
        pthread_mutex_lock(&drone->mDrone);
        /*if(vaisseau.NBColis==0){
            pthread_cond_signal(&cVaisseau);
            pthread_cond_wait(&cDrone, &mVaisseau);
        }
        //printf("Thread Drone %d\n", idDrone);
        vaisseau.NBColis--;
        printf("Drone %d a pris un colis, la il reste %d colis\n", drone->ID_drone, vaisseau.NBColis);*/
        if(drone->status == 0 && drone->zone == 0){
            prendreColis(drone, vaisseau.slot[idDrone].colis[i]);
            if(i==NB_COLIS){
                i = 0;
            }else{
                i++;
            }
            DecrementerTotalColis(idDrone);
            livrerColis(drone);
        }
        pthread_mutex_unlock(&drone->mDrone);
    }
    pthread_exit(NULL);
}
