#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../headers/structure.h"
#include "../headers/fonctions.h"
#include "../headers/client.h"

/*Fonction qui initialise les clients*/
Client_t initClient(int i, Client_t client){
    client.ID = i;
    client.zone = get_random(ZONE,1);
    //client.etat = get_random(2,1);
    client.etat = 1;
    client.NBColisRecu = 0;
    client.NBColisAttente = 0;
    client.couloir[0] = 0;
    client.couloir[1] = 0;
    client.retourColis = 0;
    printf("Client ID est de %d, et sa zone est %d, et il est dans un %d\n", client.ID, client.zone, client.etat);
    return client;
}

void* fonction_client(void* arg){

    Client_t *client = (Client_t*) arg;
    int idClient = client->ID;
    sleep(2);
  
    int k = client->NBColisAttente;
    int i,j,l,m;
    BOOL boolean = TRUE;

    /*Si le nbre de colis que le client attend est nulle on arrete ce threads*/
    if(client->NBColisAttente==0){

        printf("%sLe client %d n'a aucun colis en attente ! \n%s", CYAN, idClient, INIT);

    }else{
      
        while(client->NBColisAttente !=0 && appuyer == TRUE){

            pthread_mutex_lock(&client->mClient);
            
            /*J'attends que le drone m'appel*/
            pthread_cond_wait(&client->cClient, &client->mClient);
            pthread_mutex_unlock(&client->mClient);
          
            printf("%sClient %d est appelle\n%s", BLUE, idClient, INIT);
            
            pthread_mutex_lock(&client->mClient);
          
            /*Cherche quel drone a son colis*/
            while(boolean){

                for(i = 0; i<k; ++i){

                    for(j = 0; j<data.nbDrone; ++j){

                        if(idClient == drone[j].colis.ID_client && client->colis[i].poids == drone[j].colis.poids && client->colis[j].temps == drone[j].colis.temps){

                            if(drone[j].colis.etatLivraison == 2){

                                l = j;
                                m = i;
                                boolean = FALSE;

                            }

                        }

                    }

                }

            }

            boolean = TRUE;

            /*Si l'etat du colis est a 1 cest a dire que le colis est correct sans dommage sinon le client refuse de le prendre*/
            if(drone[l].colis.etat == 1){
              
                printf("%sClient %d a pris son colis\n%s", BLUE, idClient, INIT);
                client->colis[m].etatLivraison = 4;
                client->NBColisRecu++;
              
            }else{
              
                printf("%sClient %d refuse le colis car mauvais etat\n%s", BLUE, idClient, INIT);
                client->retourColis++;
              
            }

            drone[l].status = 3;
            client->NBColisAttente--;
            //pthread_cond_signal(&client->cClient);
            pthread_cond_broadcast(&client->cClient);
          
            pthread_mutex_unlock(&client->mClient);

        }
         
        printf("%sClient %d, n'a plus de colis en attente !%s", CYAN, idClient, INIT);
        if(client->retourColis>0){
          
            printf("%sMais j'ai refuse %d colis car ils sont en mauvais etat\n%s", CYAN, client->retourColis, INIT);
        }
        printf("\n");

    }

    pthread_exit(NULL);
}
