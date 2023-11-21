/*
 * PEDERSEN Ny Aina
 * license: Unlicense
 *
 * Binary heap implementation for Prim's algorithm
 */

#include "prim_heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
Pheap
pheap_create (const size_t capacity)
{
    Pheap pheap;

    //
    if (!capacity)
        fprintf (stderr, "non positive pheap capacity\n"), exit (255);

    //
    pheap.capacity = capacity;
    pheap.size = 0;

    //
    pheap.pheap = calloc (capacity, sizeof (Pnode));
    pheap.position = calloc (capacity, sizeof (int));
    if (!pheap.pheap || !pheap.position)
        perror ("calloc"), exit (254);

    //
    memset (pheap.position, -1, capacity * sizeof (int));

    //
    return pheap;
}

//
void
pheap_reset (Pheap *restrict pheap)
{
    pheap->size = 0;
    memset (pheap->position, -1, pheap->capacity * sizeof (int));
}

// Helpful macros
#define ROOT 0
#define PARENT(i) ((i - 1) / 2)
#define LEFT_CHILD(i) (2 * i + 1)
#define RIGHT_CHILD(i) (2 * i + 2)
#define EXISTS(i, pheap) (i < pheap->size)
#define SWAP(a, b)                                                            \
    {                                                                         \
        int pmt = pheap->position[(a).index];                                 \
        pheap->position[(a).index] = pheap->position[(b).index];              \
        pheap->position[(b).index] = pmt;                                     \
        Pnode tmp = a;                                                        \
        a = b;                                                                \
        b = tmp;                                                              \
    }

// pheapify bottom up
static inline void
pheapify_bu (Pheap *restrict pheap, const size_t start)
{
    for (size_t current = start; current != ROOT; current = PARENT (current))
        {
            // Comparing current and its parent
            if (pheap->pheap[current].value
                >= pheap->pheap[PARENT (current)].value)
                break;

            // Not OK, swapping them
            SWAP (pheap->pheap[current], pheap->pheap[PARENT (current)]);
        }
}

// pheapify top down
static inline void
pheapify_td (Pheap *restrict pheap, const size_t start)
{
    for (size_t current = start, next = current; current < pheap->size;
         current = next)
        {
            // Getting the child with the greatest value
            if (EXISTS (LEFT_CHILD (current), pheap))
                next = LEFT_CHILD (current);
            if (EXISTS (RIGHT_CHILD (current), pheap)
                && pheap->pheap[next].value
                       > pheap->pheap[RIGHT_CHILD (current)].value)
                next = RIGHT_CHILD (current);

            // Testing the current node against its child
            if (pheap->pheap[current].value > pheap->pheap[next].value)
                SWAP (pheap->pheap[current], pheap->pheap[next])
            else
                break;
        }
}

//
void
pheap_push (Pheap *restrict pheap, const Pnode *restrict node)
{
    /* WARNING: fixed size pheap */

    // Add the node to the last place
    pheap->position[node->index] = pheap->size;
    pheap->pheap[pheap->size] = *node;
    pheap->size++;

    /*
     * Restoring the min pheap property (bottom up).
     * In the worst case, we will finish on the root node.
     * So the worst complexity is O(log n), since it is a
     * complete binary tree.
     */
    pheapify_bu (pheap, pheap->size - 1);
}

//
Pnode
pheap_pop (Pheap *restrict pheap)
{
    //
    if (pheap_empty (pheap))
        fprintf (stderr, "popping an empty pheap\n"), exit (252);

    // Getting the min
    const Pnode node = pheap->pheap[ROOT];

    /*
     * Moving the last node to the root and doing a top-down
     * to update the tree.
     * Still in O(log n) in the worst case since we might go
     * straight to a leaf of the binary pheap.
     */
    pheap->size--;
    pheap->pheap[ROOT] = pheap->pheap[pheap->size];
    pheap->position[pheap->pheap[ROOT].index] = ROOT;
    pheapify_td (pheap, ROOT);

    // Updating the node's position
    pheap->position[node.index] = -1;

    //
    return node;
}

//
void
pheap_update (Pheap *restrict pheap, const Pnode *restrict node)
{
    const int position = pheap->position[node->index];

    // Check if we can update the node
    if (position < ROOT || pheap->pheap[position].value <= node->value)
        return;
    pheap->pheap[position] = *node;

    // Updating the tree
    pheapify_bu (pheap, position);
    pheapify_td (pheap, position);
}

//
inline bool
pheap_empty (const Pheap *restrict pheap)
{
    return !pheap->size;
}

// For debugging purposes
void
pheap_print (const Pheap *restrict pheap)
{
    printf ("%ld: ", pheap->size);
    for (size_t i = ROOT; i < pheap->size; ++i)
        printf ("%3d(%6.1lf)[%3d] ", pheap->pheap[i].index,
                pheap->pheap[i].value, pheap->position[pheap->pheap[i].index]);
    printf ("\n");
}

//
void
pheap_free (Pheap *restrict pheap)
{
    // Free the container
    free (pheap->pheap);
    free (pheap->position);

    // Safety
    pheap->capacity = pheap->size = 0;
}
