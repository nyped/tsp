/*
 * PEDERSEN Ny Aina
 * license: Unlicense
 *
 * Common header
 */

#ifndef _COMMON_H_
#define _COMMON_H_

#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>

/* Check infinity from math.h */
#ifndef INFINITY
#error "Missing INFINITY macro from math.h"
#endif

//
typedef double Distance;

//
typedef int City;

//
typedef struct
{
    /*
     * reached:  list of reached city, such that:
     *            - 0 means not reached
     *            - n means reached at step n
     * position: position of the travelling salesman
     */
    City *reached, position;

    /*
     * value: value of the bound
     * tour:  distance from the start to the current position
     */
    Distance value, tour;

    //
    int depth;
} Node;

#endif /* _COMMON_H_ */

/* vim: set ts=8 sts=4 sw=4 et : */
