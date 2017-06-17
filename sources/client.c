#include "client.h"

/*Fonction qui initialise les clients*/
Client_t initClient(int i, Client_t client){
    client.ID = i;
    client.zone = get_random(ZONE,1);
    client.etat = get_random(2,1);
    client.NBColis = 0;
    printf("Client ID est de %d, et sa zone est %d, et il est dans un %d\n", client.ID, client.zone, client.etat);
    return client;
}

void ClientprendreColis(Drone_t drone){
    pthread_mutex_lock(&mClient);
    pthread_cond_wait(&cClient, & mClient);
    if(drone.colis.etat==1){
        printf("Client %d a pris son colis\n", drone.colis.ID_client);
        drone.autonomie = drone.autonomie - drone.colis.temps;
        drone.status = 3;
    }else{
        printf("Client %d refuse le colis car mauvais etat\n", drone.colis.ID_client);
    }
    pthread_cond_signal(&cDrone);
    pthread_mutex_unlock(&mClient);
}

void* fonction_client(void* arg){
    Client_t *client = (Client_t*) arg;

    while(client->NBColis >0){
        //ClientprendreColis()
    }

    pthread_exit(NULL);
}
