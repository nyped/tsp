/*
 * PEDERSEN Ny Aina
 * license: Unlicense
 *
 * Header for Prim based algorithms
 */

#ifndef _PRIM_H_
#define _PRIM_H_

#include "common.h"
#include "prim_heap.h"

//
Distance prim_bound_mst (Pheap *restrict pheap, const Distance *restrict,
                         const City *restrict, size_t n);

Distance prim_bound_1tree_opt (Pheap *restrict pheap, const Distance *restrict,
                               const City *restrict, size_t n,
                               Distance *restrict tmp1, City *restrict tmp2);

#endif /* _PRIM_H_ */

/* vim: set ts=8 sts=4 sw=4 et : */
