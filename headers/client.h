#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "structure.h"
#include "fonctions.h"

Client_t initClient(int i, Client_t client);
void* fonction_client(void* arg);

#endif // CLIENT_H_INCLUDED
