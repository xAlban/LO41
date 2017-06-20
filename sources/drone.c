#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "../headers/structure.h"
#include "../headers/fonctions.h"
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
    //Sleep(100*AUTONOMIE);
    sleep(100*AUTONOMIE);
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
    sleep(2000);
    //Sleep(2000);
    drone->colis.etatLivraison = 2;
    drone->autonomie = drone->autonomie - (drone->colis.temps/2);
    pthread_cond_signal(&client[drone->colis.ID_client].cClient);

}

void monterDrone(Drone_t *drone){

    pthread_cond_wait(&client[drone->colis.ID_client].cClient, &client[drone->colis.ID_client].mClient);
    printf("Le client %d a pris son colis, le drone %d remonte\n", drone->colis.ID_client, drone->ID_drone);
    sleep(2000);
    //Sleep(2000);
    drone->status = 4;

}

 void rentrerDrone(Drone_t *drone){

    printf("Drone %d arrive au vaisseau mere\n", drone->ID_drone);
    //Sleep(100*(drone->colis.temps/2));
    sleep(100*(drone->colis.temps/2));
    drone->autonomie = drone->autonomie - (drone->colis.temps/2);
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
    //Sleep(100*(drone->colis.temps/2));
    sleep(100*(drone->colis.temps/2));
    donneColis(drone);

}

void* fonction_drone(void* arg){

    Drone_t *drone = (Drone_t*) arg;
    int idDrone = drone->ID_drone;
    //Sleep(2000);
    sleep(2000);
    int i = 0; // pointer sur le colis du slot

    pthread_cond_wait(&vaisseau.cVaisseau, &vaisseau.mVaisseau);

    while(vaisseau.slot[idDrone].NBColisSlot>0 && drone->status!=-2){

        /*Si il n'y a plus de colis on envoie un signal au vaisseau pour dire qu'on rentre*/
        if(vaisseau.NBColis==0){
            pthread_cond_signal(&vaisseau.cVaisseau);
            drone->status = -2;
        }

        /*Si le drone a pour status 0 et qui est au vaisseau mere on cherche le colis*/
        if(drone->status == 0 && drone->zone == 0){

            if(drone->charge>=vaisseau.slot[idDrone].colis[i].poids){

                /*Si l'autonomie du drone est inferieur a celui du colis on va se recharger*/
                if(drone->autonomie<vaisseau.slot[idDrone].colis[i].temps){

                    printf("Recharge du drone %d\n", idDrone);
                    drone->status = -1;
                    //Sleep(100*AUTONOMIE);
                    sleep(100*AUTONOMIE);
                    drone->autonomie = AUTONOMIE;
                    printf("Fin du rechargement du drone %d\n", idDrone);
                    drone->status = 0;

                }

                /*Prendre le colis*/
                drone->colis = vaisseau.slot[idDrone].colis[i];
                drone->status = 1;
                drone->colis.etatLivraison = 1;
                printf("Drone %d a pris le colis pour le client %d\n", drone->ID_drone, drone->colis.ID_client);

                /*on incremente i pour prendre le prochain colis du slot*/
                if(i==NB_COLIS){
                    i = 0;
                }else{
                    i++;
                }

                /*on decremente le nombre de colis*/
                vaisseau.NBColis--;
                vaisseau.slot[idDrone].NBColisSlot--;

            }
            /*Si le couloir pour aller vers le client est occupe alors on attends un signal*/
            if(client[drone->colis.ID_client].couloir[0] == 1){
                    printf("Couloir utilise pour aller au client %d, j'attends\n", drone->colis.ID_client);
                    pthread_cond_wait(&client[drone->colis.ID_client].cClient, &client[drone->colis.ID_client].mClient);
            }

            printf("Couloir vide pour aller au client %d, j'y vais\n", drone->colis.ID_client);
            vaisseau.NBDroneTravail++;
            //Sleep(100*(drone->colis.temps/2));
            sleep(100*(drone->colis.temps/2));
            drone->autonomie = drone->autonomie - (drone->colis.temps/2);
            drone->zone = drone->colis.zone;
            printf("Le drone %d arrive a la zone %d\n", idDrone, drone->zone);

            /*Si le client est chez lui on livre son colis sinon on rentre*/
            if(client[drone->colis.ID_client].etat==1){

                printf("Le client %d est chez lui, drone %d descends\n", drone->colis.ID_client, idDrone);
                //Sleep(2000);
                sleep(2000);
                drone->colis.etatLivraison = 2;
                drone->autonomie = drone->autonomie - (drone->colis.temps/2);

                /*on envoie le signal au client pour lui dire qu'on est la*/
                pthread_cond_signal(&client[drone->colis.ID_client].cClient);

            }else{

                printf("Le client %d n'est pas chez lui\n", drone->colis.ID_client);
                drone->colis.etat = 1;

            }

            /*on attends le signal pour continuer*/
            pthread_cond_wait(&client[drone->colis.ID_client].cClient, &client[drone->colis.ID_client].mClient);

            /*Si le couloir pour rentrer est occupe on attends*/
            if(client[drone->colis.ID_client].couloir[1] == 1){
                pthread_cond_wait(&client[drone->colis.ID_client].cClient, &client[drone->colis.ID_client].mClient); // wwait qui
            }

            /*on libere le couloir apres avoir emprunte le couloir pour rentrer*/
            client[drone->colis.ID_client].couloir[0] = 0;
            drone->status = 4;
            //Sleep(100*(drone->colis.temps/2));
            sleep(100*(drone->colis.temps/2));
            printf("Drone %d arrive au vaisseau mere\n", idDrone);
            drone->autonomie = drone->autonomie - (drone->colis.temps/2);
            vaisseau.NBDroneTravail--;
            drone->zone = 0;

            /*On libere le couloir pour rentrer apres arriver au vaisseau*/
            client[drone->colis.ID_client].couloir[1] = 0;

            /*On teste si le l'etat du colis est mauvais alors on le met dans le dernier slot du vaisseau*/
            if(drone->colis.etat==2){
                printf("Colis mavais etat, placement dans le dernier slot\n");
                vaisseau.slot[NB_SLOT].colis[vaisseau.NBColisRetour] = drone->colis;
                vaisseau.slot[NB_SLOT].colis[vaisseau.NBColisRetour].etatLivraison = 0;
                vaisseau.slot[NB_SLOT].NBColisSlot++;
                vaisseau.NBColisRetour++;
            }

        }

        /*Si le drone n'a plus d'autonomie et qu'il se trouve au vaisseau on recharge le drone*/
        if(drone->autonomie == 0 && drone->zone == 0){
            printf("Recharge du drone %d\n", idDrone);
            //Sleep(100*AUTONOMIE);
            sleep(100*AUTONOMIE);
            drone->autonomie = AUTONOMIE;
            printf("Fin du rechargement du drone %d\n", idDrone);
            drone->status = 0;
        }

        /*Si le drone est perdu, on stop ce thread*/
        if(drone->status == -2 && drone->zone == -1){
            //mort du drone
            drone->status = -2;
        }

        drone->status = 0;

    }

    pthread_cond_signal(&vaisseau.cVaisseau);

    pthread_exit(NULL);
}
