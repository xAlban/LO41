#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "../headers/fonctions.h"

/*Fonction qui retourne un nombre aleatoire entre 1 et max*/
int get_random(int max, int n){
    return (rand()% max+n);
}

/*affiche en rouge*/
void ROUGE(char *chaine)
{
	printf("%s%s%s", RED, chaine, INIT);
}

/*affiche en bleu*/
void BLEU(char *chaine)
{
	printf("%s%s%s", BLUE, chaine, INIT);
}

/*affiche en vert*/
void VERT(char *chaine)
{
	printf("%s%s%s", GREEN, chaine, INIT);
}

/*affiche en jaune*/
void JAUNE(char *chaine)
{
	printf("%s%s%s", YELLOW, chaine, INIT);
}
