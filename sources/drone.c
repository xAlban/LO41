#include "../headers/drone.h"

/*Fonction qui initialise les colis*/
Drone_t Init_drone(int i, Drone_t drone){
    drone.ID_drone = i;
    drone.slot = i;
    drone.status = 0;
    drone.autonomie = AUTONOMIE;
    drone.charge = get_random(CHARGEMAXI, 1);
    drone.zone = 0;
    printf("Drone %d prend le slot %d et a pour charge maximal %d kg\n", drone.ID_drone, drone.slot, drone.charge);
    return drone;
}

/*Fonction pour que le drone se recharge son autonomie*/
void recharger(Drone_t *drone){

    printf("Recharge du drone %d\n", drone->ID_drone);
    Sleep(10*AUTONOMIE);
    drone->autonomie = AUTONOMIE;
    printf("Fin du rechargement du drone %d\n", drone->ID_drone);
    drone->status = 0;

}

/*Fonction pour prendre le colis du slot*/
void prendreColis(Drone_t *drone, Colis_t Colis){

    if(drone->charge>=Colis.poids){
        if(drone->autonomie>=Colis.temps){
            printf("Drone %d a pris le colis pour le client %d\n", drone->ID_drone, drone->colis.ID_client);
            drone->colis = Colis;
            drone->status = 1;
            drone->zone = drone->colis.zone;
            drone->colis.etatLivraison = 1;
        }else{
            recharger(drone);
        }
    }

}

void descendDrone(Drone_t *drone){

    //int i = 0;
    printf("Drone %d est arrive a destination, je descends\n", drone->ID_drone);
    /*while(client[drone->colis.ID_client].colis[i]!=drone->colis){
        i++;
    }*/
    //psleep(2000);
    drone->colis.etatLivraison = 2;
    pthread_cond_signal(&client[drone->colis.ID_client].cClient);

}

void monterDrone(Drone_t *drone){

    pthread_cond_wait(&client[drone->colis.ID_client].cClient, &client[drone->colis.ID_client].mClient);
    printf("Le client %d a pris son colis, le drone %d remonte\n", drone->colis.ID_client, drone->ID_drone);
    Sleep(2000);
    drone->status = 4;

}

 void rentrerDrone(Drone_t *drone){

    printf("Drone %d arrive au vaisseau mere\n", drone->ID_drone);
    Sleep(10*(drone->colis.temps/2));
    vaisseau.NBDroneTravail--;
    drone->zone = 0;
    if(drone->colis.etat==2){
        if(vaisseau.NBColisRetour!=0){
            vaisseau.NBColisRetour++;
        }
        printf("Colis mavais etat, placement dans le dernier slot\n");
        vaisseau.slot[NB_SLOT].colis[vaisseau.NBColisRetour] = drone->colis;
        vaisseau.slot[NB_SLOT].NBColisSlot++;
    }
    drone->status = 1;

 }

void DecrementerTotalColis(int idDrone){
    vaisseau.NBColis--;
    vaisseau.slot[idDrone].NBColisSlot--;
}

void donneColis(Drone_t *drone){
    if(client[drone->colis.ID_client].etat==1){
        printf("Le client %d est chez lui\n", drone->colis.ID_client);
        descendDrone(drone);
        monterDrone(drone);
    }else{
        printf("Le client %d n'est pas chez lui\n", drone->colis.ID_client);
    }
    rentrerDrone(drone);
}

void livrerColis(Drone_t *drone){

    vaisseau.NBDroneTravail++;
    Sleep(10*(drone->colis.temps/2));
    donneColis(drone);

}

void* fonction_drone(void* arg){
    Drone_t drone;
    //int idDrone = drone->ID_drone;
    //int i = 0; // pointer sur le colis du slot
    //while(vaisseau.slot[drone.ID_drone].NBColisSlot>0){

      //  pthread_cond_wait(&vaisseau.cVaisseau, &vaisseau.mVaisseau);
        /*if(vaisseau.NBColis==0){
            pthread_cond_signal(&cVaisseau);
            pthread_cond_wait(&cDrone, &mVaisseau);
        }*/
        //printf("Thread Drone %d\n", idDrone);
        /*vaisseau.NBColis--;
        printf("Drone %d a pris un colis, la il reste %d colis\n", drone.ID_drone, vaisseau.NBColis);
        if(drone.status == 0 && drone.zone == 0){
            prendreColis(&drone, vaisseau.slot[drone.ID_drone].colis[i]);
            if(i==NB_COLIS){
                i = 0;
            }else{
                i++;
            }
            DecrementerTotalColis(drone.ID_drone);
            livrerColis(&drone);
        }else if(drone.autonomie == 0 && drone.zone == 0){
            recharger(&drone);
        }else if(drone.status == -2){
            //mort du drone
        }
    }*/
    pthread_exit(NULL);
}
