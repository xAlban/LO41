#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include "structure.h"
#include "drone.h"
#include "vaisseau.h"
#include "client.h"

void initAll();

int main(int argc, char* argv[]){
    int ret = 0;
    int i;
    srand(time(NULL));

    initAll();

    printf("Creation des threads drones !\n");
    for(i = 0; i<NB_DRONE; ++i){
        ret = pthread_create(&t_drone[i], NULL, fonction_drone, &drone[i]);
        if(ret){
            exit(EXIT_FAILURE);
        }
    }

    printf("Creation d'un thread vaisseau !\n");
    ret = pthread_create(&t_vaisseau, NULL, fonction_vaisseau, &vaisseau);
    if(ret){
        exit(EXIT_FAILURE);
    }

    printf("Creation des threads client !\n");
    for(i = 0; i<NB_CLIENT; ++i){
        ret = pthread_create(&t_client[i], NULL, fonction_client, &client[i]);
        if(ret){
            exit(EXIT_FAILURE);
        }
    }

    for(i = 0; i<NB_CLIENT; ++i){
        pthread_join(t_client[i], NULL);
    }
    for(i = 0; i<NB_DRONE; ++i){
        pthread_join(t_drone[i], NULL);
    }
    pthread_join(t_vaisseau, NULL);

    return EXIT_SUCCESS;
}

void initAll(){
    int i,j,k,l;

    printf("Initialisation des clients\n");
    for(i = 0; i<NB_CLIENT; ++i){
        client[i] = initClient(i, client[i]);
    }
    printf("\n\n");

    printf("Initialisation des drones\n");
    for(j = 0; j<NB_DRONE; ++j){
        drone[j] = Init_drone(j, drone[j]);
    }
    printf("\n\n");

    printf("Initialisation du vaisseau mere\n");
    vaisseau.NBColis = 0;
    vaisseau.NBDroneAttente = NB_DRONE;
    vaisseau.NBDronePerdu = 0;
    vaisseau.NBDroneRepos = 0;
    vaisseau.NBDroneTravail = 0;
    vaisseau.Status = 0;
    printf("\n\n");

    printf("Initialisation des slots et des colis\n");
    for(k = 0; k<NB_SLOT-1; ++k){
        printf("Slot %d\n", k);
        vaisseau.slot[k] = initAllColis(k);
    }
    printf("\n\n");

    printf("Tri des colis\n");
    for(l = 0; l<NB_SLOT-1; ++l){
        printf("Slot %d\n", l);
        vaisseau.slot[l] = triColis(vaisseau.slot[l]);
    }
    printf("\n\n");

    printf("Connaitre chaque client attends combien de colis\n");
    for(i = 0; i<NB_CLIENT; ++i){
        printf("Client %d attends %d colis\n", client[i].ID, client[i].NBColis);
    }
    printf("\n\n");

    printf("Initialisation des mutex\n");
    pthread_mutex_init(&mClient, NULL);
    pthread_mutex_init(&mVaisseau, NULL);
    pthread_mutex_init(&mColis, NULL);
    pthread_mutex_init(&mDrone, NULL);

    /*mClient = PTHREAD_MUTEX_INITIALIZER;
    mColis = PTHREAD_MUTEX_INITIALIZER;
    mDrone = PTHREAD_MUTEX_INITIALIZER;
    mVaisseau = PTHREAD_MUTEX_INITIALIZER;*/

    cVaisseau = PTHREAD_COND_INITIALIZER;
    cColis = PTHREAD_COND_INITIALIZER;
    cDrone = PTHREAD_COND_INITIALIZER;
    cClient = PTHREAD_COND_INITIALIZER;
    printf("\n\n");
}

