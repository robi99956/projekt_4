#include "ramie.h"

ramie::ramie(uint16_t l1, uint16_t l2, QPoint poczatek)
{
    k = new kinematyka(l1, l2, poczatek);
}

ramie::~ramie()
{
    delete k;
}
