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

void ClientprendreColis(Client_t *client, Drone_t drone){
    pthread_mutex_lock(&client->mClient);
    pthread_cond_wait(&client->cClient, &drone.mDrone);
    if(drone.colis.etat==1){
        printf("Client %d a pris son colis\n", drone.colis.ID_client);
        client->NBColisAttente--;
        client->NBColisRecu++;
        drone.autonomie = drone.autonomie - drone.colis.temps;
        drone.status = 3;
    }else{
        printf("Client %d refuse le colis car mauvais etat\n", drone.colis.ID_client);
    }
    pthread_cond_signal(&drone.cDrone);
    pthread_mutex_unlock(&client->mClient);
}

void* fonction_client(void* arg){
    Client_t *client = (Client_t*) arg;
    int idClient = pthread_self() - 1;
    while(client->NBColisAttente >0){
        //printf("Thread client %d\n", idClient);
        //ClientprendreColis(&client)
    }

    pthread_exit(NULL);
}
