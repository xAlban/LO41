#include "../headers/client.h"

/*Fonction qui initialise les clients*/
Client_t initClient(int i, Client_t client){
    client.ID = i;
    client.zone = get_random(ZONE,1);
    client.etat = get_random(2,1);
    client.NBColisRecu = 0;
    client.NBColisAttente = 0;
    printf("Client ID est de %d, et sa zone est %d, et il est dans un %d\n", client.ID, client.zone, client.etat);
    return client;
}

void ClientprendreColis(Client_t *client){
    //int k = client->NBColisAttente;
    //int i,j,l,m;
    //BOOL boolean = FALSE;
    pthread_mutex_lock(&client->mClient);
    pthread_cond_wait(&client->cClient, &client->mClient);
    /*while(boolean){
        for(i = 0; i<k; ++i){
            for(j = 0; j<NB_DRONE; ++j){
                if(client->ID==drone[j].colis.ID_client && client->colis[i].poids == drone[j].colis.poids){
                    if(drone[j].colis.etatLivraison == 2){
                        l = j;
                        m = i;
                        break;
                        boolean = TRUE;
                    }
                }
            }
        }
    }
    if(drone[l].colis.etat==1){
        printf("Client %d a pris son colis\n", drone[l].colis.ID_client);
        client->NBColisAttente--;
        client->NBColisRecu++;
        client->colis[m].etatLivraison = 4;
    }else{
        printf("Client %d refuse le colis car mauvais etat\n", drone[l].colis.ID_client);
    }*/
    /*drone[l].status = 3;
    drone[l].colis.etatLivraison = 3;
    drone[l].autonomie = drone[l].autonomie - (drone[l].colis.temps/2);*/
    pthread_cond_signal(&client->cClient);
    pthread_mutex_unlock(&client->mClient);
}

void* fonction_client(void* arg){
    Client_t *client = (Client_t*) arg;
    int idClient = client->ID;
    printf("Client thread %d creee\n", idClient);
    Sleep(2000);
    if(client->NBColisAttente==0){
        printf("Le client %d ne recoit pas ces colis car aucun des colis lui appartient!\n", client->ID);
    }else{
        while(client->NBColisAttente >0){
            ClientprendreColis(client);
        }
        printf("Client %d, n'a plus de colis en attente ! \n", client->ID);
    }

    pthread_exit(NULL);
}
