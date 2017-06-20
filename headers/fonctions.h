#ifndef FONCTIONS_H_INCLUDED
#define FONCTIONS_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define RED "\x1B[1;31m"
#define BLUE "\x1B[1;34m"
#define GREEN "\x1B[1;32m"
#define YELLOW "\x1B[1;33m"
#define INIT "\x1B[0m"

int get_random(int max, int n);
void ROUGE(char *chaine);
void BLEU(char *chaine);
void VERT(char *chaine);
void JAUNE(char *chaine);

#endif // FONCTIONS_H_INCLUDED
