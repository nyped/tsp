/*
 * PEDERSEN Ny Aina
 * license: Unlicense
 *
 * Header for binary heap used by Prim's algorithm
 */

#ifndef _PHEAP_H_
#define _PHEAP_H_

#include "common.h"

//
typedef struct
{
    Distance value;
    City index, prec;
} Pnode;

//
typedef struct
{
    size_t size, capacity;
    Pnode *restrict pheap;
    int *restrict position;
} Pheap;

//
Pheap pheap_create (const size_t capacity);

//
void pheap_reset (Pheap *restrict pheap);

//
void pheap_push (Pheap *restrict pheap, const Pnode *restrict node);

//
Pnode pheap_pop (Pheap *restrict pheap);

//
void pheap_update (Pheap *restrict pheap, const Pnode *restrict node);

//
bool pheap_empty (const Pheap *restrict pheap);

//
void pheap_print (const Pheap *restrict pheap);

//
void pheap_free (Pheap *restrict pheap);

#endif /* _PHEAP_H_ */
