#ifndef VAISSEAU_H_INCLUDED
#define VAISSEAU_H_INCLUDED

void* fonction_vaisseau(void* arg);
Slot_t initAllColis(Vaisseau_t *vaisseau, int i);
Slot_t triColis(Slot_t slot);
int totalColis(int nb);
Colis_t Init_colis(int i, int j, Colis_t colis);

#endif // VAISSEAU_H_INCLUDED
