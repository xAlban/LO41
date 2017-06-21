#ifndef FONCTIONS_H_INCLUDED
#define FONCTIONS_H_INCLUDED

#define BLACK "\x1B[1;30m"
#define RED "\x1B[1;31m"
#define GREEN "\x1B[1;32m"
#define YELLOW "\x1B[1;33m"
#define BLUE "\x1B[1;34m"
#define MAGENTA "\x1B[1;35m"
#define CYAN "\x1B[1;36m"
#define BLANC "\x1B[1;37m"
#define INIT "\x1B[0m"

int get_random(int max, int n);
void ROUGE(char *chaine);
void BLEU(char *chaine);
void VERT(char *chaine);
void JAUNE(char *chaine);

#endif // FONCTIONS_H_INCLUDED
