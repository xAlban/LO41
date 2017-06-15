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
    //int ret = 0;
    //int i;
    srand(time(NULL));
    //pthread_t t_drone[NB_DRONE], t_slot[NB_SLOT];

    /*printf("\n************************************************Initialisation des Drones !********************************************\n");
    printf("Nombre de drone a creer : %d\n", NB_DRONE);
    printf("\n****************************************Initialisation des colis dans chaque slots !***********************************\n");
    printf("Nombre de de colis a creer dans chaque slot : %d\n", NB_COLIS);*/

    initAll();
    //printf("Creation des threads drones !\n");
    /*for(i = 1; i<=NB_DRONE; ++i){
        ret = pthread_create(&t_drone[i], NULL, fonction_drone, (void*)i);
    }
    //printf("Creation des threads slots !\n");
    for(i = 1; i<=NB_SLOT; ++i){
        ret = pthread_create(&t_slot[i], NULL, fonction_vaisseau, (void*)i);
    }

    for(i = 1; i<=NB_DRONE; ++i){
        pthread_join(t_drone[i], NULL);
    }
    for(i = 1; i<=NB_SLOT; ++i){
        pthread_join(t_slot[i], NULL);
    }*/
    return EXIT_SUCCESS;
}

void initAll(){
    int i,j,k,l;
    for(i = 1; i<=NB_CLIENT; ++i){
        client[i] = initClient(i, client[i]);
    }
    for(j = 1; j<=NB_DRONE; ++j){
        drone[j] = Init_drone(j, drone[j]);
    }
    for(k = 1; k<=NB_SLOT-1; ++k){
        printf("Slot %d\n", k);
        vaisseau.slot[k] = initAllColis(k);
    }
    for(l = 1; l<=NB_SLOT-1; ++l){
        printf("Slot %d\n", l);
        vaisseau.slot[l] = triColis(vaisseau.slot[l]);
    }
}

