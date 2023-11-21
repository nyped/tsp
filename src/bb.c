/*
 * PEDERSEN Ny Aina
 * license: Unlicense
 *
 * Branch and bound implementation for the TSP
 */

#include "bb.h"
#include "heap.h"
#include "prim.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Useful macros
#define START 0
#define TARGET START
#define DIST(a, b) (distances[(a)*n + (b)])
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) < (b)) ? (b) : (a))

/*
 * Branch the node for a new city i.
 */
static inline void
bb_branch (Heap *restrict heap, Pheap *restrict pheap,
           const Distance *restrict distances, const size_t n,
           Distance *best_tour, const Node *restrict node,
           const City next_city, Distance *restrict _tmp1,
           City *restrict _tmp2)
{
    Node next_node = *node;

    /*
     * Early stop if we step into the initial node
     * without doing a tour.
     */
    if (next_city == START && node->depth != n - 1)
        return;

    // Allocating the reached array
    {
        const size_t size = n * sizeof (City);
        next_node.reached = malloc (size);
        if (!next_node.reached)
            perror ("malloc"), exit (254);
        memcpy (next_node.reached, node->reached, size);
    }

    // Updating the node's state
    next_node.depth++;
    next_node.position = next_city;
    next_node.tour += DIST (node->position, next_city);

    // Update the degrees
    next_node.reached[node->position]++;
    next_node.reached[next_city]++;

#if 0
    next_node.value
        = next_node.tour + prim_bound_mst (pheap, distances, next_node.reached, n);
#elif 1
    next_node.value
        = next_node.tour
          + prim_bound_1tree_opt (pheap, distances, next_node.reached, n,
                                  _tmp1, _tmp2);
#else
    if (next_node.depth < n / 2)
        next_node.value
            = next_node.tour
              + prim_bound_1tree_opt (pheap, distances, next_node.reached, n,
                                      _tmp1, _tmp2);
    else
        next_node.value
            = next_node.tour
              + prim_bound_mst (pheap, distances, next_node.reached, n);
#endif

    // Is it a solution ?
    if (next_node.value < *best_tour)
        {
            if (next_node.depth == n)
                {
                    // free the previous solution
#pragma omp critical
                    *best_tour = next_node.tour;
                    free (next_node.reached);
                }
            else
#pragma omp critical
                heap_push (heap, &next_node);
        }
    else
        {
            free (next_node.reached);
        }
}

//
Distance
bb_solve (const Distance *restrict distances, const size_t n)
{
    Distance best_tour = INFINITY;
    Heap heap = heap_create (10000);
    const long iter_max = 1e9;
    long iter = 0;
    int nb_worker = 0;

    // First node
    {
        const Node start = { .position = START,
                             .tour = 0.0,
                             .depth = 0,
                             .value = INFINITY,
                             .reached = calloc (n, sizeof (City)) };

        if (!start.reached)
            perror ("calloc"), exit (254);

        heap_push (&heap, &start);
    }

#pragma omp parallel
    {
        Pheap _pheap = pheap_create (n);
        City *restrict _reached = NULL;
        Distance *restrict _distances = NULL;
        bool stop = false;

        // Allocating temporary arrays
        _distances = malloc (n * n * sizeof (Distance));
        _reached = malloc (n * sizeof (City));
        if (!_distances || !_reached)
            perror ("malloc"), exit (254);

        //
        while (true)
            {
                Node current;

                // Getting the best node
#pragma omp critical
                {
                    if (!stop && heap.size)
                        {
                            nb_worker++;
                            current = heap_pop (&heap);
                        }
                    else
                        stop = true;
                }

                //
                if (stop)
                    {
                        if (nb_worker > 0)
                            {
                                stop = false;
                                continue;
                            }
                        else
                            break;
                    }

                // Check for terminaison
                if (best_tour < current.value)
                    {
#pragma omp critical
                        nb_worker -= 1;
                        free (current.reached);
                        break;
                    }

#if 0
#pragma omp critical
                {
                    printf ("iter: %10ld depth: %3d position: %2d tour: %5.1f "
                            "bound: "
                            "%5.1f best: %5.1f \n",
                            iter, current.depth, current.position,
                            current.tour, current.value, best_tour);
                }
#endif

                // Branching on the node
                for (size_t i = 0; i < n; ++i)
                    if (current.reached[i] != 2 && current.position != i)
                        bb_branch (&heap, &_pheap, distances, n, &best_tour,
                                   &current, i, _distances, _reached);

                        // Finished the job
#pragma omp critical
                nb_worker -= 1;

                // Free the node
                free (current.reached);

#pragma omp critical
                iter += 1;
                if (iter == iter_max)
                    break;
            }

        //
        free (_distances);
        free (_reached);
        pheap_free (&_pheap);
    }

    //
    if (iter == iter_max)
        fprintf (stderr, "/!\\ reached max iteration\n");

    //
    printf ("iterations: %ld\n", iter);

    //
    heap_free (&heap);

    //
    return best_tour;
}

/* vim: set ts=8 sts=4 sw=4 et : */
