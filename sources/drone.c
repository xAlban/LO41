#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "../headers/structure.h"
#include "../headers/fonctions.h"
#include "../headers/drone.h"

/*Fonction qui initialise les colis*/
Drone_t Init_drone(int i, Drone_t drone){
    drone.ID_drone = i;
    drone.slot = i;
    drone.status = 0;
    drone.autonomie = AUTONOMIE;
    drone.charge = get_random(CHARGEMAXI, 5);
    drone.zone = 0;
    drone.NBColisLivre = 0;
    printf("Drone %d prend le slot %d et a pour charge maximal %d kg\n", drone.ID_drone, drone.slot, drone.charge);
    return drone;
}

void DecrementerTotalColis(int idDrone){
    vaisseau.NBColis--;
    vaisseau.slot[idDrone].NBColisSlot--;
}

void* fonction_drone(void* arg){

    Drone_t *drone = (Drone_t*) arg;
    int idDrone = drone->ID_drone;
    //Sleep(2000);
    fflush(stdout);
    sleep(2);
    int i = 0; // pointer sur le colis du slot
    int idClient;
    //printf("drone %d, slot %d, status %d, charge %d, autonomie %d, zone %d, nbre colis %d\n", drone->ID_drone, drone->slot, drone->status, drone->charge, drone->autonomie, drone->zone, drone->NBColisLivre);

    while(vaisseau.slot[drone->slot].NBColisSlot>0 && drone->status!=-2){

        pthread_mutex_lock(&drone->mDrone);

        /*Si il n'y a plus de colis on envoie un signal au vaisseau pour dire qu'on rentre*/
        if(vaisseau.NBColis == 0){
            pthread_cond_signal(&vaisseau.cVaisseau);
        }

        /*Si le drone a pour status 0 et qui est au vaisseau mere on cherche le colis*/
        if(drone->status == 0 && drone->zone == 0){

            if(drone->charge>=vaisseau.slot[drone->slot].colis[i].poids){

                /*Si l'autonomie du drone est inferieur a celui du colis on va se recharger*/
                if(drone->autonomie<vaisseau.slot[drone->slot].colis[i].temps){

                    VERT("Recharge du drone ");
                    printf("%d\n", drone->ID_drone);
                    drone->status = -1;
                    //Sleep(100*AUTONOMIE);
                    fflush(stdout);
                    sleep(AUTONOMIE/10);
                    drone->autonomie = AUTONOMIE;
                    printf("Fin du rechargement du drone ");
                    printf("%d\n", drone->ID_drone);
                    drone->status = 0;

                }

                /*Prendre le colis*/
                drone->colis = vaisseau.slot[drone->slot].colis[i];
                idClient = drone->colis.ID_client;
                drone->status = 1;
                drone->colis.etatLivraison = 1;
                printf("%sDrone %d a pris le colis pour le client %d\n%s", GREEN, drone->ID_drone, idClient, INIT);

                /*on incremente i pour prendre le prochain colis du slot*/
                i++;

                /*on decremente le nombre de colis*/
                vaisseau.NBColis--;
                vaisseau.slot[drone->slot].NBColisSlot--;

            }
            /*Si le couloir pour aller vers le client est occupe alors on attends un signal*/
            if(client[idClient].couloir[0] == 1){

                printf("%sCouloir utilise pour aller au client %d, j'attends\n%s", GREEN, idClient, INIT);
                pthread_cond_wait(&drone->cDrone, &drone->mDrone);
                printf("%sCouloir libere pour aller au client %d\n%s", GREEN, idClient, INIT);

            }

            /*J'emprunte le couloir pour aller a la zone*/
            printf("%sCouloir vide pour aller au client %d, j'y vais\n%s", GREEN, idClient, INIT);
            client[idClient].couloir[0] = 1;

            vaisseau.NBDroneTravail++;
            //Sleep(100*(drone->colis.temps/2));
            fflush(stdout);
            sleep((drone->colis.temps/2)/10);
            drone->autonomie = drone->autonomie - (drone->colis.temps/2);
            drone->zone = drone->colis.zone;
            printf("%sLe drone %d arrive a la zone %d\n%s", GREEN, drone->ID_drone, drone->zone, INIT);

            /*Si le client est chez lui on livre son colis sinon on rentre*/
            if(client[idClient].etat==1){

                printf("%sLe client %d est chez lui, drone %d descends\n%s", GREEN, idClient, drone->ID_drone, INIT);
                //Sleep(2000);
                //sleep(2000);
                drone->colis.etatLivraison = 2;
                drone->autonomie = drone->autonomie - (drone->colis.temps/2);

                /*on envoie le signal au client pour lui dire qu'on est la*/
                pthread_cond_signal(&client[idClient].cClient);

                /*on attends le signal pour continuer*/
                pthread_cond_wait(&client[idClient].cClient, &client[idClient].mClient);
                printf("%sDrone %d, colis etat livraison %d\n%s", GREEN, drone->ID_drone, drone->colis.etatLivraison, INIT);

            }else{

                printf("%sLe client %d n'est pas chez lui, drone %d rentre\n%s", GREEN, idClient, drone->ID_drone, INIT);
                drone->colis.etat = 1;

            }

            drone->NBColisLivre++;
            printf("%sDrone %d a livre %d colis\n%s", GREEN, drone->ID_drone, drone->NBColisLivre, INIT);

            /*Si le couloir pour rentrer est occupe on attends*/
            if(client[idClient].couloir[1] == 1){

                printf("%sDrone %d attends que le couloir pour rentrer soit vide\n%s", GREEN, drone->ID_drone, INIT);
                pthread_cond_wait(&drone->mDrone, &drone->mDrone);
                printf("%sCouloir libere pour rentrer au vaisseau Drone %d peut y aller\n%s", GREEN, drone->ID_drone, INIT);

            }

            /*J'emprunte le couloir pour rentrer*/
            printf("%sDrone %d prend le couloir retour\n%s", GREEN, drone->ID_drone, INIT);
            client[idClient].couloir[1] = 1;

            /*on libere le couloir apres avoir emprunte le couloir pour rentrer*/
            client[idClient].couloir[0] = 0;
            printf("%sDrone %d libere le couloir aller du client %d\n%s", GREEN, drone->ID_drone, drone->colis.ID_client, INIT);
            pthread_cond_signal(&drone->cDrone);

            drone->status = 4;
            //Sleep(100*(drone->colis.temps/2));
            fflush(stdout);
            sleep((drone->colis.temps/2)/10);
            printf("%sDrone %d arrive au vaisseau mere\n%s", GREEN, drone->ID_drone, INIT);
            drone->autonomie = drone->autonomie - (drone->colis.temps/2);
            vaisseau.NBDroneTravail--;
            drone->zone = 0;

            /*On libere le couloir pour rentrer apres arriver au vaisseau*/
            client[idClient].couloir[1] = 0;
            pthread_cond_signal(&drone->cDrone);
            printf("%sDrone %d libere le couloir 1 du client %d\n%s", GREEN, drone->ID_drone, drone->colis.ID_client, INIT);

            /*On teste si le l'etat du colis est mauvais alors on le met dans le dernier slot du vaisseau*/
            if(drone->colis.etat==2){

                ROUGE("Colis mavais etat, placement dans le dernier slot\n");
                vaisseau.slot[NB_SLOT].colis[vaisseau.NBColisRetour] = drone->colis;
                vaisseau.slot[NB_SLOT].colis[vaisseau.NBColisRetour].etatLivraison = 0;
                vaisseau.slot[NB_SLOT].NBColisSlot++;
                vaisseau.NBColisRetour++;

            }

        }

        /*Si le drone n'a plus d'autonomie et qu'il se trouve au vaisseau on recharge le drone*/
        if(drone->autonomie == 0 && drone->zone == 0){

            printf("%sRecharge du drone %d\n%s", GREEN, drone->ID_drone, INIT);
            //Sleep(100*AUTONOMIE);
            fflush(stdout);
            sleep(AUTONOMIE/10);
            drone->autonomie = AUTONOMIE;
            printf("%sFin du rechargement du drone %d\n%s", GREEN, drone->ID_drone, INIT);
            drone->status = 0;

        }

        /*Si le drone est perdu, on stop ce thread*/
        if(drone->status == -2 && drone->zone == -1){
            //mort du drone
            drone->status = -2;
        }

        drone->status = 0;

        if(drone->NBColisLivre == NB_COLIS){
            drone->status = -2;
        }

        pthread_mutex_unlock(&drone->mDrone);
    }

    pthread_cond_signal(&vaisseau.cVaisseau);

    pthread_exit(NULL);
}
