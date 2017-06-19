#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/*Fonction qui retourne un nombre aleatoire entre 1 et max*/
int get_random(int max, int n){
    return (rand()% max+n);
}
