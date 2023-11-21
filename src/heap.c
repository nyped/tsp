/*
 * PEDERSEN Ny Aina
 * license: Unlicense
 *
 * Binary heap implementation
 */

#include "heap.h"
#include <stdio.h>
#include <stdlib.h>

//
Heap
heap_create (const size_t capacity)
{
    Heap heap;

    //
    if (!capacity)
        fprintf (stderr, "non positive heap capacity\n"), exit (255);

    //
    heap.capacity = capacity;
    heap.size = 0;

    //
    heap.heap = calloc (capacity, sizeof (Node));
    if (!heap.heap)
        perror ("calloc"), exit (254);

    //
    return heap;
}

// Helpful macros
#define ROOT 0
#define PARENT(i) ((i - 1) / 2)
#define LEFT_CHILD(i) (2 * i + 1)
#define RIGHT_CHILD(i) (2 * i + 2)
#define EXISTS(i, heap) (i < heap->size)
#define SWAP(a, b)                                                            \
    {                                                                         \
        Node tmp = a;                                                         \
        a = b;                                                                \
        b = tmp;                                                              \
    }

//
void
heap_push (Heap *restrict heap, const Node *restrict node)
{
    // Dynamic expansion if full
    if (heap->size == heap->capacity)
        {
            heap->capacity *= 2;
            heap->heap
                = reallocarray (heap->heap, heap->capacity, sizeof (Node));
            if (!heap->heap)
                fprintf (stderr, "failed to realloc the heap\n"), exit (253);
        }

    // Add the node to the last place
    heap->heap[heap->size] = *node;
    heap->size++;

    /*
     * Restoring the max heap property (bottom up).
     * In the worst case, we will finish on the root node.
     * So the worst complexity is O(log n), since it is a
     * complete binary tree.
     */
    for (size_t current = heap->size - 1; current != ROOT;
         current = PARENT (current))
        {
            // Comparing current and its parent
            if (heap->heap[current].value
                >= heap->heap[PARENT (current)].value)
                break;

            // Not OK, swapping them
            SWAP (heap->heap[current], heap->heap[PARENT (current)]);
        }
}

//
Node
heap_pop (Heap *restrict heap)
{
    Node node;

    //
    if (heap_empty (heap))
        fprintf (stderr, "popping an empty heap\n"), exit (252);

    // Getting the max
    node = heap->heap[ROOT];

    /*
     * Moving the last node to the root and doing a top-down
     * to update the tree.
     * Still in O(log n) in the worst case since we might go
     * straight to a leaf of the binary heap.
     */
    heap->size--;
    heap->heap[ROOT] = heap->heap[heap->size];
    for (size_t current = ROOT, next = current; current < heap->size;
         current = next)
        {
            // Getting the child with the greatest value
            if (EXISTS (LEFT_CHILD (current), heap))
                next = LEFT_CHILD (current);
            if (EXISTS (RIGHT_CHILD (current), heap)
                && heap->heap[next].value
                       > heap->heap[RIGHT_CHILD (current)].value)
                next = RIGHT_CHILD (current);

            // Testing the current node against its child
            if (heap->heap[current].value > heap->heap[next].value)
                SWAP (heap->heap[current], heap->heap[next])
            else
                break;
        }

    //
    return node;
}

//
inline bool
heap_empty (const Heap *restrict heap)
{
    return !heap->size;
}

// For debugging purposes
void
heap_print (const Heap *restrict heap)
{
    printf ("%ld: ", heap->size);
    for (size_t i = 0; i < heap->size; ++i)
        printf ("%lf ", heap->heap[i].value);
    printf ("\n");
}

//
void
heap_free (Heap *restrict heap)
{
    // Freeing the items of the container
    for (size_t i = 0; i < heap->size; ++i)
        free (heap->heap[i].reached);

    // Free the container
    free (heap->heap);

    // Safety
    heap->capacity = heap->size = 0;
}
