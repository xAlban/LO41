#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>


#define N_DRONE 2 //nbre de drone
#define N_COLIS 2 //nbre de colis
#define AUTONOMIE 30

typedef struct Colis{
    int priority;
    int temps; //temps necessaire pour arriver a destination
    int poids; //poids du colis
}Colis_t;

typedef struct Drone{
    int slot;
    int status;
    int charge; // poids total qui peut transporte
    int autonomie;
    Colis_t colis; // pour donner un colis au drone
}Drone_t;

typedef struct Slot{
    int NBColisSlot;
    Colis_t colis[N_COLIS];
}Slot_t;

typedef struct Vaisseau{
    Slot_t slot[N_DRONE];
    int NBColis;
    int NBDroneAttente;
    int NBSlot;
    int NBDroneTravail;
    int NBDronePerdu;
	int Status;
}Vaisseau_t;

void* fonction_drone(void* arg);
void* fonction_slot(void* arg);
Colis_t Init_colis(int i, int j, Colis_t colis);
Drone_t Init_drone(int i, Drone_t drone);
int get_random(int max);
Vaisseau_t vaisseau;
Drone_t drone[N_DRONE];

int main(int argc, char* argv[]){
    srand(time(NULL));
    int N_SLOT = N_DRONE+1;
    int ret = 0;
    int i, j;
    pthread_t t_drone[N_DRONE], t_slot[N_SLOT];

    printf("Initialisation des Drones !\n");
    for(i = 1; i<=N_DRONE; ++i){
        drone[i] = Init_drone(i, drone[i]);
    }
    printf("Initialisation des colis dans chaque slots !\n");
    for(j = 1; j<=N_SLOT-1; ++j){
        vaisseau.slot[j].NBColisSlot = 0;
        for(i = 1; i<=N_COLIS; ++i){
            vaisseau.slot[j].colis[i] = Init_colis(i, j, vaisseau.slot[i].colis[i]);
            vaisseau.slot[j].NBColisSlot++;
        }
        printf("Nombre de colis dans le slot %d a %d\n", j, vaisseau.slot[j].NBColisSlot);
    }
    printf("Creation des threads drones !\n");
    for(i = 1; i<=N_DRONE; ++i){
        ret = pthread_create(&t_drone[i], NULL, fonction_drone, (void*)i);
    }
    printf("Creation des threads slots !\n");
    for(i = 1; i<=N_SLOT; ++i){
        ret = pthread_create(&t_slot[i], NULL, fonction_slot, (void*)i);
    }

    for(i = 1; i<=N_DRONE; ++i){
        pthread_join(t_drone[i], NULL);
    }
    for(i = 1; i<=N_SLOT; ++i){
        pthread_join(t_slot[i], NULL);
    }
    return EXIT_SUCCESS;
}

void* fonction_slot(void* arg){
    int i = (int) arg;
    while(1){

    }
    pthread_exit(NULL);
}

void* fonction_drone(void* arg){
    int i = (int)arg;
    while(1){
        if(drone[i].autonomie!=0){
            if(vaisseau.slot[drone[i].slot].colis[i].poids <= drone[i].charge){
                    if(vaisseau.slot[drone[i].slot].colis[i].temps<=drone[i].autonomie){
                        drone[i].colis = vaisseau.slot[drone[i].slot].colis[i];
                        printf("Drone %d a pris le colis %d qui va prendre une duree de %d min\n", i, i, vaisseau.slot[drone[i].slot].colis[i].temps);
                        vaisseau.slot[drone[i].slot].NBColisSlot--;
                        drone[i].autonomie = drone[i].autonomie-vaisseau.slot[drone[i].slot].colis[i].temps;
                    }
            }
        }
    }
    pthread_exit(NULL);
}

/*Fonction qui retourne un nombre al¨¦atoire entre 0 et max*/
int get_random(int max){
    return (rand()% max+1);
}

/*Fonction qui initialise les colis*/
Drone_t Init_drone(int i, Drone_t drone){
    drone.slot = i;
    drone.charge = get_random(50);
    printf("Drone %d prend le slot %d et a pour charge maximal %d kg\n", i, drone.slot, drone.charge);
    return drone;
}

/*Fonction qui initialise les drones*/
//i pour le numero de colis et j pour le numero du slot
Colis_t Init_colis(int i, int j, Colis_t colis){
    colis.priority =  get_random(3);
    colis.poids = get_random(drone[j].charge);
    colis.temps = get_random(AUTONOMIE);
    printf("Colis %d a pour priorite %d, pour un temps maxi %d min, et pese %d\n", i, colis.priority, colis.temps, colis.poids);
    return colis;
}
