/*
 * PEDERSEN Ny Aina
 * license: Unlicense
 *
 * TSP solver top-level program
 */

#include "bb.h"
#include "reader.h"
#include <stdio.h>
#include <stdlib.h>

//
int
main (int argc, char *argv[])
{
    Distance *distances;
    size_t n;

    //
    if (argc != 2)
        return fprintf (stderr, "usage: %s file\n", *argv), 255;

    //
    distances = reader (argv[1], &n);

    //
    printf ("tour: %ld\n", (long)bb_solve (distances, n));

    //
    free (distances);

    //
    return 0;
}

/* vim: set ts=8 sts=4 sw=4 et : */
