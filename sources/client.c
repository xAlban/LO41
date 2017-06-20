#include <stdio.h>
#include <stdlib.h>
#include "../headers/structure.h"
#include "../headers/fonctions.h"
#include "../headers/client.h"

/*Fonction qui initialise les clients*/
Client_t initClient(int i, Client_t client){
    client.ID = i;
    client.zone = get_random(ZONE,1);
    client.etat = get_random(2,1);
    client.NBColisRecu = 0;
    client.NBColisAttente = 0;
    client.couloir[0] = 0;
    client.couloir[1] = 0;
    printf("Client ID est de %d, et sa zone est %d, et il est dans un %d\n", client.ID, client.zone, client.etat);
    return client;
}

void* fonction_client(void* arg){

    Client_t *client = (Client_t*) arg;
    int idClient = client->ID;
    //Sleep(2000);
    //sleep(2000);
    int k = client->NBColisAttente;
    int i,j,l,m;
    BOOL boolean = FALSE;

    /*Si le nbre de colis que le client attend est nulle on arrete ce threads*/
    if(client->NBColisAttente==0){

        printf("Le client %d n'a aucun colis en attente ! \n", idClient);

    }else{

        while(client->NBColisAttente >0){

            pthread_mutex_lock(&client->mClient);

            pthread_cond_wait(&client->cClient, &client->mClient);
            printf("Client %d appelle\n", idClient);

            while(boolean){
                printf("TEST\n");
                for(i = 0; i<k; ++i){
                    printf("TEST\n");
                    for(j = 0; j<NB_DRONE; ++j){
                        printf("TEST\n");
                        if(idClient == drone[j].colis.ID_client && client->colis[i].poids == drone[j].colis.poids && client->colis[j].temps == drone[j].colis.temps){
                            printf("TEST\n");
                            if(drone[j].colis.etatLivraison == 2){

                                l = j;
                                m = i;
                                printf("TROUVER\n");
                                boolean = TRUE;

                            }

                        }

                    }

                }

            }

            boolean = FALSE;

            if(drone[l].colis.etat==1){
                printf("Client %d a pris son colis\n", idClient);
                client->NBColisAttente--;
                client->NBColisRecu++;
                client->colis[m].etatLivraison = 4;
            }else{
                printf("Client %d refuse le colis car mauvais etat\n", idClient);
            }
            drone[l].status = 3;

            pthread_cond_signal(&client->cClient);

            pthread_mutex_unlock(&client->mClient);

        }

        printf("Client %d, n'a plus de colis en attente ! \n", idClient);

    }

    pthread_exit(NULL);
}
