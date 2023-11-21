/*
 * PEDERSEN Ny Aina
 * license: Unlicense
 *
 * Variants of Prim's algorithm implementation
 */

#include "prim.h"
#include "prim_heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIST(a, b) distances[(a)*n + (b)]
#define MAX(a, b) (((b) < (a)) ? (a) : (b))

//
static inline Distance
prim_mst (Pheap *restrict pheap, const Distance *restrict distances,
          City *restrict degree, size_t n, bool update_degree)
{
    Distance mst_weight = 0;
    pheap_reset (pheap);

    // Adding the non reached nodes
    for (size_t city = 0; city < n; ++city)
        if (degree[city] != 2)
            {
                const Pnode node
                    = { .index = city, .value = INFINITY, .prec = -1 };

                pheap_push (pheap, &node);
            }

    // All reached
    if (pheap->size == 0)
        return 0;

    // Adding the root to the tree
    {
        const Pnode next_node = { .index = 0, .value = 0, .prec = -1 };

        pheap_update (pheap, &next_node);
    }

    //
    while (!pheap_empty (pheap))
        {
            const Pnode node = pheap_pop (pheap);

            //
            mst_weight += node.value;

            // Updating the degrees
            if (update_degree && node.prec != -1)
                {
                    degree[node.prec]++;
                    degree[node.index]++;
                }

            /*
             * Finding all the edges that go from the node
             * and updating the value of the pheap, with the
             * min.
             */
            for (size_t city = 0; city < n; ++city)
                {
                    const Pnode next_node = { .index = city,
                                              .value = DIST (node.index, city),
                                              .prec = node.index };

                    pheap_update (pheap, &next_node);
                }
        }

    //
    return mst_weight;
}

//
Distance
prim_bound_1tree_opt (Pheap *restrict pheap,
                      const Distance *restrict _distances,
                      const City *restrict _degree, size_t n,
                      Distance *restrict distances, City *restrict degree)
{
    const size_t dist_size = n * n * sizeof (Distance);
    const size_t deg_size = n * sizeof (City);
    Distance mst_weight = 0;

    // Copying the distances matrix, and the degree matrix
    memcpy (distances, _distances, dist_size);
    memcpy (degree, _degree, deg_size);

    /*
     * Getting the best 1tree.
     * For this we add weights to the nodes.
     * We give negative weights to nodes with one incident edge,
     * and positive weights to nodes with more than 2.
     * We repeat this process multiple times.
     * It should converge to a stable value.
     */
    {
#define MAX_IT 25

        double extra_weight, weight_factor = 1;

        // Getting the degrees on the initial mst
        mst_weight = prim_mst (pheap, distances, degree, n, true);

        // Setting the weight_factor as the mean edge weight
        weight_factor = MAX (weight_factor, mst_weight / n);

        // Running prim one updated graph
        for (size_t i = 0; i < MAX_IT; ++i)
            {
                extra_weight = 0;

                // Add weights
                for (City city = 0; city < n; ++city)
                    {
                        const double sup_weight
                            = weight_factor * (degree[city] - 2);

                        // Updating the cost of the city
                        for (City j = 0; j < n; ++j)
                            DIST (city, j) += sup_weight;
                        for (City j = 0; j < n; ++j)
                            DIST (j, city) += sup_weight;

                        // Updating the tree extra cost
                        extra_weight += 2 * sup_weight;
                    }

                // Getting the weight
                memcpy (degree, _degree, deg_size);
                mst_weight = prim_mst (pheap, distances, degree, n, true);
                weight_factor *= 0.9;
            }

        // Removing the extra weight
        mst_weight += extra_weight;
    }

    //
    return mst_weight;
}

//
Distance
prim_bound_mst (Pheap *restrict pheap, const Distance *restrict distances,
                const City *restrict reached, size_t n)
{
    /*
     * Safe cast. The last arguments tells that it
     * should not be updated.
     */
    return prim_mst (pheap, distances, (City *restrict)reached, n, false);
}

/* vim: set ts=8 sts=4 sw=4 et : */
