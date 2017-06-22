#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include "../headers/structure.h"
#include "../headers/drone.h"
#include "../headers/vaisseau.h"
#include "../headers/client.h"
#include "../headers/fonctions.h"

void initAll();
void DestroyAll();
void traitantSIGINT(int n);

int nbDrone = 10;
int nbColis = 5;
int nbClient = 10;

pthread_t *t_drone, t_vaisseau, *t_client;

int main(int argc, char* argv[]){
    
    /*Action sur CRTL+C*/
    signal(SIGINT, traitantSIGINT);
    
    int ret = 0;
    int i;
    appuyer = TRUE;
    srand(time(NULL));
  
    if(argc > 1){
      
          nbDrone = atoi(argv[1]);
      
          if(argc > 2){
            
              nbClient = atoi(argv[2]);
            
                  if(argc > 3){
                    
                      nbColis = atoi(argv[3]);
                    
                  }
            
          }
      
    }
    
    data.nbColis = nbColis;
    data.nbClient = nbClient;
    data.nbDrone = nbDrone;
    data.nbColisMax = nbDrone * nbColis;
    data.nbSlot = nbDrone + 1;
    
    printf("%s\nNombre de drone : %d\n%s", YELLOW, data.nbDrone, INIT);
    printf("%sNombre de client : %d\n%s", YELLOW, data.nbClient, INIT);
    printf("%sNombre de colis par slot : %d\n\n%s", YELLOW, data.nbColis, INIT);
    
    t_drone = malloc(sizeof(pthread_t) * data.nbDrone);
  
    if(t_drone == NULL){
      
        fprintf(stderr,"Allocation thread drone impossible\n");
        exit(EXIT_FAILURE);
      
    }
  
    t_client = malloc(sizeof(pthread_t) * data.nbClient);
  
    if(t_client == NULL){
      
        fprintf(stderr,"Allocation thread client impossible\n");
        exit(EXIT_FAILURE);
      
    }
  
    initAll();
  
    /*Creation des threads*/
    JAUNE("Creation d'un thread vaisseau !\n");
    ret = pthread_create(&t_vaisseau, NULL, fonction_vaisseau, &vaisseau);
    printf("Vaisseau thread creee\n");
  
    if(ret){
        
        fprintf(stderr,"Creation thread vaisseau impossible\n");
        exit(EXIT_FAILURE);
      
    }
  
    BLEU("Creation des threads client !\n");
    for(i = 0; i<data.nbClient; ++i){
      
        ret = pthread_create(&t_client[i], NULL, fonction_client, &client[i]);
        printf("Client thread %d creee\n", i);
      
        if(ret){
          
            fprintf(stderr,"Creation thread client %d impossible\n", i);
            exit(EXIT_FAILURE);
          
        }
      
    }
    sleep(2);

    VERT("Creation des threads drones !\n");
    for(i = 0; i<data.nbDrone; ++i){
      
        ret = pthread_create(&t_drone[i], NULL, fonction_drone, &drone[i]);
        printf("Drone thread %d creee\n", i);
      
        if(ret){
             
            fprintf(stderr,"Creation thread drone %d impossible\n", i);
            exit(EXIT_FAILURE);
          
        }
      
    }
    sleep(2);

    for(i = 0; i<data.nbClient; ++i){
      
        pthread_join(t_client[i], NULL);
      
    }
  
    for(i = 0; i<data.nbDrone; ++i){
      
        pthread_join(t_drone[i], NULL);
      
    }
    pthread_join(t_vaisseau, NULL);

    DestroyAll();
    
    ROUGE("FIN DU PROGRAMME\n");
    return EXIT_SUCCESS;
}

/*Fonction qui initialise tout*/
void initAll(){
  
    int i;
  
    BLEU("Initialisation des clients\n");
    client = malloc(sizeof(Client_t) * data.nbClient);
  
    if(client == NULL){
      
        fprintf(stderr,"Allocation client impossible\n");
        exit(EXIT_FAILURE);
      
    }
    
    /*Allocation des colis sur le client, on prendra le nbre de colis max car on ne peut pas savoir en avance combien de colis attendra le client*/
    for(i = 0; i<data.nbClient; ++i){
      
        client[i].colis = malloc(sizeof(Colis_t) * data.nbColisMax);
      
    }
  
    /*Initialisation des clients*/
    for(i = 0; i<data.nbClient; ++i){
      
        sleep(1);
        client[i] = initClient(i, client[i]);
      
    }
    printf("\n\n");

    VERT("Initialisation des drones\n");
    /*Allocation des drones*/
    drone = malloc(sizeof(Drone_t) * data.nbDrone);
    
    if(drone == NULL){
      
        fprintf(stderr,"Allocation drone impossible\n");
        exit(EXIT_FAILURE);
      
    }
    
    /*Initialisation des drones*/
    for(i = 0; i<data.nbDrone; ++i){
      
        sleep(1);
        drone[i] = Init_drone(i, drone[i]);
      
    }
    printf("\n\n");
  
    JAUNE("Initialisation du vaisseau mere\n");
    vaisseau.NBColis = 0;
    vaisseau.NBDroneAttente = data.nbDrone;
    vaisseau.NBDronePerdu = 0;
    vaisseau.NBDroneRepos = 0;
    vaisseau.NBDroneTravail = 0;
    vaisseau.Status = 0;
    vaisseau.NBColisRetour = 0;
  
    /*Allocation des slots du vaisseau*/
    vaisseau.slot = malloc(sizeof(Slot_t) * data.nbSlot);
  
    if(vaisseau.slot == NULL){
      
        fprintf(stderr,"Allocation des slots du vaisseau impossible\n");
        exit(EXIT_FAILURE);
      
    }
  
    /*Allocation des colis du slot du vaisseau*/
    for(i = 0; i<data.nbSlot; ++i){
      
        /*Dernier slot cest pour les colis refuse ou colis que le client etait pas chez lui.
        On ne connait le nbre de colis qui peut avoir dans le dernier slot donc on attribue le maximun des colis*/
        if(i == data.nbSlot-1){
            
            vaisseau.slot[i].colis = malloc(sizeof(Colis_t) * data.nbColisMax);
          
        }else{
            
            vaisseau.slot[i].colis = malloc(sizeof(Colis_t) * data.nbColis);
          
        }
      
        if(vaisseau.slot[i].colis == NULL){
          
            fprintf(stderr,"Allocation des colis du slot %d du vaisseau impossible\n", i);
            exit(EXIT_FAILURE);
          
        }
      
    }
    
    printf("\n");
    sleep(2);
    
    ROUGE("Initialisation des slots et des colis\n");
    for(i = 0; i<data.nbSlot-1; ++i){
      
        printf("%sSlot %d\n%s", MAGENTA, i, INIT);
        sleep(1);
        vaisseau.slot[i] = initAllColis(&vaisseau, i);
      
    }
    vaisseau.slot[data.nbSlot].NBColisSlot = 0;
    printf("\n\n");

    ROUGE("Tri des colis\n");
    for(i = 0; i<data.nbSlot-1; ++i){
      
        printf("%sSlot %d\n%s",MAGENTA, i, INIT);
        sleep(1);
        vaisseau.slot[i] = triColis(vaisseau.slot[i]);
      
    }
    printf("\n\n");

    BLEU("Connaitre chaque client attends combien de colis\n");
    for(i = 0; i<data.nbClient; ++i){
      
        printf("Client %d attends %d colis\n", client[i].ID, client[i].NBColisAttente);
        sleep(1);
      
    }
  
    printf("Nombre total de colis a livrer est de %d\n", vaisseau.NBColis);
    printf("\n\n");

    ROUGE("Chargement des colis dans les slots\n\n");
    sleep(2);

    ROUGE("Initialisation des mutex\n");
    sleep(2);

    for(i = 0; i<data.nbClient; ++i){
      
        pthread_mutex_init(&client[i].mClient, NULL);
        pthread_cond_init(&client[i].cClient, NULL);
      
    }

    pthread_mutex_init(&vaisseau.mVaisseau, NULL);
    pthread_cond_init(&vaisseau.cVaisseau, NULL);
    printf("\n\n");
}

void DestroyAll(){
    int i;
    ROUGE("Destroy All\n");

    for(i = 0; i<data.nbClient; ++i){
      
        pthread_mutex_destroy(&client[i].mClient);
        pthread_cond_destroy(&client[i].cClient);
      
    }

    pthread_mutex_destroy(&vaisseau.mVaisseau);
    pthread_cond_destroy(&vaisseau.cVaisseau);
    
    for(i = 0; i<data.nbClient; ++i){
      
        free(client[i].colis);
        client[i].colis = NULL;
      
    }
    
    for(i = 0; i<data.nbSlot; ++i){
      
        free(vaisseau.slot[i].colis);
        vaisseau.slot[i].colis = NULL;
      
    }
  
    free(vaisseau.slot);
    vaisseau.slot = NULL;
  
    free(client);
    client = NULL;
  
    free(drone);
    drone = NULL;
  
}

void traitantSIGINT(int n){
  
    int i;
  
    if(n!=SIGINT){
      fprintf(stderr, "Probleme sur SIGINT\n");
    }
    
    for(i = 0; i<data.nbDrone; ++i){
        pthread_cancel(t_drone[i]);
    }
  
    for(i = 0; i<data.nbClient; ++i){
        pthread_cancel(t_client[i]);
    }
    
    appuyer = FALSE;
    ROUGE("APPUI SUR CTRL+C ARRET DU PROGRAMME! \n");
  
}
