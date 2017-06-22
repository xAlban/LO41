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
    drone.charge = get_random(15, CHARGEMAXI-15);
    drone.zone = 0;
    drone.NBColisLivre = 0;
    drone.NBColisRetour = 0;
    printf("Drone %d prend le slot %d et a pour charge maximal %d kg\n", drone.ID_drone, drone.slot, drone.charge);
    return drone;
}

void* fonction_drone(void* arg){
    
    Drone_t *drone = (Drone_t*) arg;

    //int idDrone = drone->ID_drone;
    
    int i = 0; // pointer sur le colis du slot
    int idClient;

    while(vaisseau.slot[drone->slot].NBColisSlot>0 && drone->status!=-2 && appuyer == TRUE){
      
        /*Si il n'y a plus de colis on envoie un signal au vaisseau pour dire qu'on rentre*/
        if(vaisseau.NBColis == 0){
            pthread_cond_signal(&vaisseau.cVaisseau);
        }

        /*Si le drone a pour status 0 et qui est au vaisseau mere on cherche le colis*/
        if(drone->status == 0 && drone->zone == 0){

            if(drone->charge>=vaisseau.slot[drone->slot].colis[i].poids){

                /*Si l'autonomie du drone est inferieur a celui du colis on va se recharger*/
                if(drone->autonomie<vaisseau.slot[drone->slot].colis[i].temps){

                    printf("%sRecharge du drone %d\n%s", MAGENTA, drone->ID_drone, INIT);
                  
                    /*Change de status pour dire que le drone est en charge*/
                    drone->status = -1;
                  
                    sleep(AUTONOMIE/10);
                    drone->autonomie = AUTONOMIE;
                    printf("%sFin du rechargement du drone %d\n%s", MAGENTA, drone->ID_drone, INIT);
                  
                    /*Rechange le status pour dire que le drone est pret pour prendre un colis*/
                    drone->status = 0;

                }

                /*Prendre le colis*/
                drone->colis = vaisseau.slot[drone->slot].colis[i];
                idClient = drone->colis.ID_client;
              
                /*Change le status du drone pour dire que il a pris un colis*/
                drone->status = 1;
                
                /*Change le etat de livraison du colis pour dire que le colis est sur le drone*/
                drone->colis.etatLivraison = 1;
                printf("%sDrone %d a pris le colis pour le client %d\n%s", GREEN, drone->ID_drone, idClient, INIT);

                /*on incremente i pour prendre le prochain colis du slot*/
                i++;

                /*on decremente le nombre de colis*/
                vaisseau.NBColis--;
                vaisseau.slot[drone->slot].NBColisSlot--;

            }
          
            /*Si le couloir pour aller vers le client est occupe alors on attends un signal*/
            while(client[idClient].couloir[0] == 1){
                pthread_mutex_lock(&client[idClient].mClient);
                printf("%sCouloir utilise pour aller au client %d, j'attends\n%s", MAGENTA, idClient, INIT);
                pthread_cond_wait(&client[idClient].cClient, &client[idClient].mClient);
                printf("%sCouloir libere pour aller au client %d\n%s", MAGENTA, idClient, INIT);
                pthread_mutex_unlock(&client[idClient].mClient);

            }

            /*J'emprunte le couloir pour aller a la zone*/
            printf("%sCouloir vide pour aller au client %d, j'y vais\n%s", MAGENTA, idClient, INIT);
            client[idClient].couloir[0] = 1;
            //pthread_cond_signal(&client[idClient].cClient);
            
            /*On incremente le nbre de drone qui travail c'est a dire qu'ils sont en livraison*/
            vaisseau.NBDroneTravail++;
          
            sleep((drone->colis.temps/2)/10);
            drone->autonomie = drone->autonomie - (drone->colis.temps/2);
          
            /*On change le zone du drone pour dire qu'il est dans la zone du client*/
            drone->zone = drone->colis.zone;
            printf("%sLe drone %d arrive a la zone %d\n%s", GREEN, drone->ID_drone, drone->zone, INIT);

            /*Si le client est chez lui on livre son colis sinon on rentre*/
            if(client[idClient].etat == 1){

                printf("%sLe client %d est chez lui, drone %d descends\n%s", GREEN, idClient, drone->ID_drone, INIT);
                sleep(2);
              
                /*On change l'etat de livraison du colis pour dire que le colis est en attente pour donner au client*/
                drone->colis.etatLivraison = 2;

                /*on envoie le signal au client pour lui dire qu'on est la*/
                pthread_cond_signal(&client[idClient].cClient);
              
                /*on attends le signal pour continuer*/
                pthread_mutex_lock(&client[idClient].mClient);
                pthread_cond_wait(&client[idClient].cClient, &client[idClient].mClient);
                pthread_mutex_unlock(&client[idClient].mClient);
                //printf("%sDrone %d, colis etat livraison %d\n%s", GREEN, drone->ID_drone, drone->colis.etatLivraison, INIT);

            }else{

                printf("%sLe client %d n'est pas chez lui, drone %d rentre\n%s", GREEN, idClient, drone->ID_drone, INIT);
              
                drone->colis.etat = 1;

            }

            drone->NBColisLivre++;
            //printf("%sDrone %d a livre %d colis\n%s", GREEN, drone->ID_drone, drone->NBColisLivre, INIT);

            /*Si le couloir pour rentrer est occupe on attends*/
             while(client[idClient].couloir[1] == 1){
              
                pthread_mutex_lock(&client[idClient].mClient);
                printf("%sDrone %d attends que le couloir pour rentrer soit vide\n%s", GREEN, drone->ID_drone, INIT);
                pthread_cond_wait(&client[idClient].cClient, &client[idClient].mClient);
                printf("%sCouloir libere pour rentrer au vaisseau pour le drone %d\n%s", GREEN, drone->ID_drone, INIT);
                pthread_mutex_unlock(&client[idClient].mClient);

            }

            /*J'emprunte le couloir pour rentrer*/
            printf("%sDrone %d prend le couloir retour\n%s", GREEN, drone->ID_drone, INIT);
            client[idClient].couloir[1] = 1;
            //pthread_cond_signal(&client[idClient].cClient);
          
            /*on libere le couloir apres avoir emprunte le couloir pour rentrer*/
            client[idClient].couloir[0] = 0;
            printf("%sDrone %d libere le couloir aller du client %d\n%s", GREEN, drone->ID_drone, drone->colis.ID_client, INIT);
            pthread_cond_signal(&client[idClient].cClient);
            
            /*Change le status du drone pour dire qu'il est en train de rentrer*/
            drone->status = 4;
            sleep(drone->colis.temps/2);
            drone->autonomie = drone->autonomie - (drone->colis.temps/2);
          
            printf("%sDrone %d arrive au vaisseau mere\n%s", GREEN, drone->ID_drone, INIT);
            printf("%sDrone %d lui reste %d min de batterie\n%s", MAGENTA, drone->ID_drone, drone->autonomie, INIT);
            vaisseau.NBDroneTravail--;
          
            /*On met le zone du drone a 0 pour dire qu'il est bien au vaisseau mere*/
            drone->zone = 0;

            /*On libere le couloir pour rentrer apres arriver au vaisseau*/
            client[idClient].couloir[1] = 0;
            printf("%sDrone %d libere le couloir retour du client %d\n%s", GREEN, drone->ID_drone, drone->colis.ID_client, INIT);
            pthread_cond_signal(&client[idClient].cClient);
          
            /*On teste si le l'etat du colis est mauvais ou le client n'est pas la alors on le met dans le dernier slot du vaisseau*/
            if(drone->colis.etat==2 || client[idClient].etat == 2){

                ROUGE("Colis mavais etat ou le client n'est pas chez lui, on place le colis dans le dernier slot\n");
                vaisseau.slot[data.nbSlot-1].colis[vaisseau.NBColisRetour] = drone->colis;
                vaisseau.slot[data.nbSlot-1].colis[vaisseau.NBColisRetour].etatLivraison = 0;
                vaisseau.slot[data.nbSlot-1].NBColisSlot++;
                vaisseau.NBColisRetour++;
                drone->NBColisRetour++;

            }

        }

        /*Si le drone n'a plus d'autonomie et qu'il se trouve au vaisseau on recharge le drone*/
        if(drone->autonomie == 0 && drone->zone == 0){

            printf("%sRecharge du drone %d\n%s", MAGENTA, drone->ID_drone, INIT);
            sleep(AUTONOMIE/10);
            drone->autonomie = AUTONOMIE;
            printf("%sFin du rechargement du drone %d\n%s", MAGENTA, drone->ID_drone, INIT);
            drone->status = 0;

        }

        /*Si le drone est perdu, on stop ce thread*/
        if(drone->status == -2 && drone->zone == -1){
            //mort du drone
            drone->status = -2;
        }
        
        /*Changement du status du drone pour dire qu'il est pret pour prendre un colis*/
        drone->status = 0;

        if(drone->NBColisLivre == data.nbColis){
            drone->status = -2;
        }

    }
  
    printf("%sDrone %d a fini de livrer ces colis %s", RED, drone->ID_drone, INIT);
    if(drone->NBColisRetour>0){
      
      printf("%sMais il y a retourner %d colis%s", RED, drone->NBColisRetour, INIT);
      
    }
    printf("\n");
  
    pthread_cond_signal(&vaisseau.cVaisseau);

    pthread_exit(NULL);
}
