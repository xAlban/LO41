#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/*Fonction qui retourne un nombre aleatoire entre 0 et max*/
int get_random(int max){
    return (rand()% max+1);
}
