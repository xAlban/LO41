#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include "structure.h"
#include "drone.h"
#include "vaisseau.h"
#include "client.h"

void initAll();

pthread_t t_drone[NB_DRONE], t_slot[NB_SLOT], t_client[NB_CLIENT];

int main(int argc, char* argv[]){
    int ret = 0;
    int i;
    srand(time(NULL));


    /*printf("\n************************************************Initialisation des Drones !********************************************\n");
    printf("Nombre de drone a creer : %d\n", NB_DRONE);
    printf("\n****************************************Initialisation des colis dans chaque slots !***********************************\n");
    printf("Nombre de de colis a creer dans chaque slot : %d\n", NB_COLIS);*/

    initAll();
    printf("Creation des threads drones !\n");
    for(i = 0; i<NB_DRONE; ++i){
        ret = pthread_create(&t_drone[i], NULL, fonction_drone, (void*)i);
        if(ret){
            exit(EXIT_FAILURE);
        }
    }
    printf("Creation des threads slots !\n");
    for(i = 0; i<NB_SLOT; ++i){
        ret = pthread_create(&t_slot[i], NULL, fonction_vaisseau, (void*)i);
        if(ret){
            exit(EXIT_FAILURE);
        }
    }

    printf("Creation des threads client !\n");
    for(i = 0; i<NB_CLIENT; ++i){
        ret = pthread_create(&t_client[i], NULL, fonction_client, (void*)i);
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
    for(i = 0; i<NB_SLOT; ++i){
        pthread_join(t_slot[i], NULL);
    }
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
    printf("Initialisation des slots et des colis\n");
    vaisseau.NBColis = 0;
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

