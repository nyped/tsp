/*
 * PEDERSEN Ny Aina
 * license: Unlicense
 *
 * Implementation of a TSP distance matrix reader
 */

#include "reader.h"
#include <stdio.h>
#include <stdlib.h>

//
Distance *restrict reader (const char *restrict file, size_t *n)
{
    Distance *distances = NULL;
    FILE *f = NULL;

    //
    f = fopen (file, "r");
    if (!f)
        perror ("fopen"), exit (255);

    // Scanning the header
    fscanf (f, " %lu", n);

    // Memory allocation
    distances = malloc (*n * *n * sizeof (Distance));
    if (!distances)
        perror ("malloc"), exit (254);

    // Scanning the distance matrix
    for (size_t i = 0; i < *n * *n; ++i)
        fscanf (f, " %lf", distances + i);

    // Adding infinity to diagonals
    for (size_t i = 0; i < *n; ++i)
        distances[i * *n + i] = INFINITY;

    //
    fclose (f);

    //
    return distances;
}

/* vim: set ts=8 sts=4 sw=4 et : */
