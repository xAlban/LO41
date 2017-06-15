#ifndef VAISSEAU_H_INCLUDED
#define VAISSEAU_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include "structure.h"

void* fonction_vaisseau(void* arg);
Slot_t initAllColis(int i);
Slot_t triColis(Slot_t slot);

#endif // VAISSEAU_H_INCLUDED