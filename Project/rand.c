#include "rand.h"

static uint32_t next = 1;

uint16_t rand(void) // RAND_MAX assumed to be 32767
{
    next = next * 214013  + 2531011;
    return (uint16_t)(next >> 16) & 0x7fff;
}

void srand(uint16_t seed)
{
    next = seed;
}