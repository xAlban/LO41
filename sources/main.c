#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "../headers/structure.h"
#include "../headers/drone.h"
#include "../headers/vaisseau.h"
#include "../headers/client.h"
#include "../headers/fonctions.h"


void initAll();
void DestroyAll();

int main(int argc, char* argv[]){
    int ret = 0;
    int i;
    srand(time(NULL));

    initAll();
  
    JAUNE("Creation d'un thread vaisseau !\n");
    ret = pthread_create(&t_vaisseau, NULL, fonction_vaisseau, &vaisseau);
    printf("Vaisseau thread creee\n");
    if(ret){
        exit(EXIT_FAILURE);
    }
    //Sleep(2000);
    sleep(2);
  
    BLEU("Creation des threads client !\n");
    for(i = 0; i<NB_CLIENT; ++i){
        ret = pthread_create(&t_client[i], NULL, fonction_client, &client[i]);
        printf("Client thread %d creee\n", i);
        if(ret){
            exit(EXIT_FAILURE);
        }
    }
    //Sleep(2000);

    fflush(stdout);

    sleep(2);

    VERT("Creation des threads drones !\n");
    for(i = 0; i<NB_DRONE; ++i){
        ret = pthread_create(&t_drone[i], NULL, fonction_drone, &drone[i]);
        printf("Drone thread %d creee\n", i);
        if(ret){
            exit(EXIT_FAILURE);
        }
    }
    //Sleep(2000);

    fflush(stdout);
    sleep(2);

    JAUNE("Creation d'un thread vaisseau !\n");
    ret = pthread_create(&t_vaisseau, NULL, fonction_vaisseau, &vaisseau);
    printf("Vaisseau thread creee\n");
    if(ret){
        exit(EXIT_FAILURE);
    }
    //Sleep(2000);
    fflush(stdout);

    sleep(2);

    for(i = 0; i<NB_CLIENT; ++i){
        pthread_join(t_client[i], NULL);
    }
    for(i = 0; i<NB_DRONE; ++i){
        pthread_join(t_drone[i], NULL);
    }
    pthread_join(t_vaisseau, NULL);

    DestroyAll();

    return EXIT_SUCCESS;
}

void initAll(){
    int i,j,k,l;

    BLEU("Initialisation des clients\n");
    for(i = 0; i<NB_CLIENT; ++i){
        client[i] = initClient(i, client[i]);
    }
    printf("\n\n");
    sleep(2);

    VERT("Initialisation des drones\n");
    for(j = 0; j<NB_DRONE; ++j){
        drone[j] = Init_drone(j, drone[j]);
    }
    printf("\n\n");
    sleep(2);
  
    JAUNE("Initialisation du vaisseau mere\n");
    vaisseau.NBColis = 0;
    vaisseau.NBDroneAttente = NB_DRONE;
    vaisseau.NBDronePerdu = 0;
    vaisseau.NBDroneRepos = 0;
    vaisseau.NBDroneTravail = 0;
    vaisseau.Status = 0;
    vaisseau.NBColisRetour = 0;
    printf("\n");
    sleep(2);

    ROUGE("Initialisation des slots et des colis\n");
    for(k = 0; k<NB_SLOT-1; ++k){
        printf("Slot %d\n", k);
        vaisseau.slot[k] = initAllColis(&vaisseau, k);
    }
    vaisseau.slot[NB_SLOT].NBColisSlot = 0;
    //vaisseau.slot[NB_SLOT].colis[NBColisMax];
    printf("\n\n");
    sleep(2);


    ROUGE("Tri des colis\n");
    for(l = 0; l<NB_SLOT-1; ++l){
        printf("Slot %d\n", l);
        vaisseau.slot[l] = triColis(vaisseau.slot[l]);
    }
    printf("\n\n");
    sleep(2);

    BLEU("Connaitre chaque client attends combien de colis\n");
    for(i = 0; i<NB_CLIENT; ++i){
        printf("Client %d attends %d colis\n", client[i].ID, client[i].NBColisAttente);
    }
    printf("Nombre total de colis a livrer est de %d\n", vaisseau.NBColis);
    printf("\n\n");
    sleep(2);

    ROUGE("Chargement des colis dans les slots\n\n");
    sleep(2);

    ROUGE("Initialisation des mutex\n");
    sleep(2);

    for(i = 0; i<NB_CLIENT; ++i){
        pthread_mutex_init(&client[i].mClient, NULL);
        pthread_cond_init(&client[i].cClient, NULL);
    }

    for(j = 0; j<NB_DRONE; ++j){
        pthread_mutex_init(&drone[j].mDrone, NULL);
        pthread_cond_init(&drone[j].cDrone, NULL);
    }

    pthread_mutex_init(&vaisseau.mVaisseau, NULL);
    pthread_cond_init(&vaisseau.cVaisseau, NULL);
    printf("\n\n");
}

void DestroyAll(){
    int i;
    ROUGE("Destroy All Mutex And Condition\n");

    for(i = 0; i<NB_CLIENT; ++i){
        pthread_mutex_destroy(&client[i].mClient);
        pthread_cond_destroy(&client[i].cClient);
    }

    for(i = 0; i<NB_DRONE; ++i){
        pthread_mutex_destroy(&drone[i].mDrone);
        pthread_cond_destroy(&drone[i].cDrone);
    }

    pthread_mutex_destroy(&vaisseau.mVaisseau);
    pthread_cond_destroy(&vaisseau.cVaisseau);
}

