#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "../headers/structure.h"
#include "../headers/fonctions.h"
#include "../headers/vaisseau.h"


/*Fonction qui initialise les drones*/
//i pour le numero de colis et j pour le numero du slot
Colis_t Init_colis(int i, int j, Colis_t colis){
  
    colis.ID_client = get_random(data.nbClient,0);
    colis.priorite =  get_random(3,1);
    colis.poids = get_random(10, drone[j].charge-10);//donne un poid entre (la charge du drone) et (la charge du drone + 10)
    colis.temps = get_random(25, AUTONOMIE-25); // donne un temps que dois faire le drone pour un aller retour entre 5 min et 30 min
    colis.zone = client[colis.ID_client].zone;
    colis.etat = 1; //get_random(2,1); etat du colis qui veut dire 1 pour correct et 2 pour mauvais etat du colis, ici je l'ai mis a 1
    colis.etatLivraison = 0;
    client[colis.ID_client].colis[client[colis.ID_client].NBColisAttente] = colis;
    client[colis.ID_client].NBColisAttente++;
    printf("Colis pour client %d a pour priorite %d, pour un temps maxi %d min, et pese %d kg a pour destination %d, et l'etat du colis est %d\n", colis.ID_client, colis.priorite, colis.temps, colis.poids, colis.zone, colis.etat);
    return colis;
  
}

/*Calcule du nbre total de colis dans le vaisseau*/
int totalColis(int nb){
  
    return vaisseau.NBColis + nb;
  
}

/*Initialisation de tout les colis des slots*/
Slot_t initAllColis(Vaisseau_t *vaisseau, int i){
  
    vaisseau->slot[i].NBColisSlot = 0;
    int j;
  
    for(j = 0; j<data.nbSlot-1; ++j){
 
        vaisseau->slot[i].colis[j] = Init_colis(i, j, vaisseau->slot[i].colis[j]);
        vaisseau->slot[i].NBColisSlot++;
          
    }
  
    vaisseau->NBColis = totalColis(vaisseau->slot[i].NBColisSlot);
  
    return vaisseau->slot[i];
  
}

/*Fonction qui tri les colis*/
Slot_t triColis(Slot_t slot){
  
    Colis_t tmp; //variable temporaire
    int i, j;
    int l = 0;
    int k = data.nbColis;

    /*Tri par priorite*/
    for(i =0 ; i<k; ++i){
      
        for(j = i+1; j<k; ++j){
          
            if(slot.colis[l].priorite>slot.colis[j].priorite){
              
                tmp = slot.colis[l];
                slot.colis[l] = slot.colis[j];
                slot.colis[j] = tmp;
              
            }
          
        }
      
        l++;
      
    }

    l = 0;
  
    /*Tri en fonction par priorite et par le temps de livraison croissant*/
    for(i = 0; i<k; ++i){
      
        for(j = i+1; j<k; ++j){
          
            if(slot.colis[l].priorite==slot.colis[j].priorite){
              
                if(slot.colis[l].temps>slot.colis[j].temps){
                  
                    tmp = slot.colis[l];
                    slot.colis[l] = slot.colis[j];
                    slot.colis[j] = tmp;
                  
                }
              
            }
          
        }
      
        l++;
      
    }
  
    for(i = 0; i<k; ++i){
      
            printf("Colis pour client %d, priorite %d, temps %d\n", slot.colis[i].ID_client, slot.colis[i].priorite, slot.colis[i].temps);
      
    }
  
    return slot;
}


void* fonction_vaisseau(void* arg){

    Vaisseau_t *vaisseau = (Vaisseau_t*) arg;
  
    JAUNE("Decollage du vaisseau mere\n");
    sleep(2);
  
    vaisseau->Status = 1;
    JAUNE("Vaisseau mere en l'air, commencement des livraisons\n");
    sleep(2);
  
    /*Tant que le nbre total de colis est superieur a 0 et que le status du vaisseau est en l'air, on continue a livrer les colis*/
    while(vaisseau->NBColis != 0 && vaisseau->Status == 1){

        while(vaisseau->NBColis!=0){
          
            pthread_mutex_lock(&vaisseau->mVaisseau);
            pthread_cond_wait(&vaisseau->cVaisseau, &vaisseau->mVaisseau);
            pthread_mutex_unlock(&vaisseau->mVaisseau);
          
        }
        
        pthread_mutex_lock(&vaisseau->mVaisseau);
      
        JAUNE("PLUS DE COLIS A LIVRER\n");
      
        vaisseau->Status = 0;

        pthread_mutex_unlock(&vaisseau->mVaisseau);
      
    }

    JAUNE("ATTERRISAGE DU VAISSEAU MERE A L'ENTREPROT\n");

    pthread_exit(NULL);
}
