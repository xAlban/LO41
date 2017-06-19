#ifndef DRONE_H_INCLUDED
#define DRONE_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include "structure.h"
#include "fonctions.h"

void* fonction_drone(void* arg);
Drone_t Init_drone(int i, Drone_t drone);

#endif // DRONE_H_INCLUDED
