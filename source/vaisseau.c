#include "vaisseau.h"
#include "fonctions.h"

/*Fonction qui initialise les drones*/
//i pour le numero de colis et j pour le numero du slot
Colis_t Init_colis(int i, int j, Colis_t colis){
    colis.ID_client = get_random(NB_CLIENT);
    colis.priorite =  get_random(3);
    colis.poids = get_random(drone[j].charge);
    colis.temps = get_random(AUTONOMIE);
    colis.zone = client[colis.ID_client].zone;
    printf("Colis pour client %d a pour priorite %d, pour un temps maxi %d min, et pese %d a pour destination %d\n", colis.ID_client, colis.priorite, colis.temps, colis.poids, colis.zone);
    return colis;
}

Slot_t initAllColis(int i){
    vaisseau.slot[i].NBColisSlot = 0;
    int j;
        for(j = 1; j<=NB_COLIS; ++j){
            vaisseau.slot[i].colis[j] = Init_colis(i, j, vaisseau.slot[i].colis[j]);
            vaisseau.slot[i].NBColisSlot++;
        }
    return vaisseau.slot[i];
}

Slot_t triColis(Slot_t slot){
    /*Tri par priorite*/
    Colis_t tmp;
    int i;
    int j;
    int k = NB_COLIS;
    BOOL boolean = FALSE;

    while(boolean){
        j = 1;
        for(i = 1; i<=k; ++i){
            if(slot.colis[i].priorite>slot.colis[j].priorite){
                tmp = slot.colis[i];
                slot.colis[i] = slot.colis[j];
                slot.colis[j] = tmp;
                j++;
            }
        }
        boolean = TRUE;
    }

    /*Tri en fonction du temps de livraison*/
    boolean = FALSE;
    while(boolean){
        j = 1;
        for(i = 1; i<=k; ++i){
            if(slot.colis[i].priorite==slot.colis[j].priorite){
                if(slot.colis[i].temps>slot.colis[j].temps){
                    tmp = slot.colis[i];
                    slot.colis[i] = slot.colis[j];
                    slot.colis[j] = tmp;
                    j++;
                }
            }
        }
        boolean = TRUE;
    }
    printf("TRI\n");
    for(i = 1; i<=k; ++i){
            printf("Colis pour client %d, priorite %d, temps %d\n", slot.colis[i].ID_client, slot.colis[i].priorite, slot.colis[i].temps);
    }
    return slot;
}

void* fonction_vaisseau(void* arg){
    //int i = (int) arg;
    //int j;
    /*vaisseau.slot[i].NBColisSlot = 0;
    vaisseau.Status = 0;
    if(vaisseau.Status == 0){
        for(j = 1; j<=NB_COLIS; ++j){
            if(i!=NB_SLOT){
                vaisseau.slot[i].colis[j] = Init_colis(i, j, vaisseau.slot[i].colis[j]);
                vaisseau.slot[i].NBColisSlot++;
            }else if(i==NB_SLOT){
                vaisseau.Status = 1;
                //printf("Vaisseau en l'air\n");
            }
        }
        vaisseau.slot[i] = triColis(vaisseau.slot[i]);
    }else if(vaisseau.Status == 1){

    }*/
    pthread_exit(NULL);
}


