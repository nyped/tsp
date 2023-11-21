/*
 * PEDERSEN Ny Aina
 * license: Unlicense
 *
 * Binary heap module's header
 */

#ifndef _HEAP_H_
#define _HEAP_H_

#include "common.h"

//
typedef struct
{
    size_t size, capacity;
    Node *restrict heap;
} Heap;

//
Heap heap_create (const size_t capacity);

//
void heap_push (Heap *restrict heap, const Node *restrict node);

//
Node heap_pop (Heap *restrict heap);

//
bool heap_empty (const Heap *restrict heap);

//
void heap_print (const Heap *restrict heap);

//
void heap_free (Heap *restrict heap);

#endif /* _HEAP_H_ */
