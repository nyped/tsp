# TSP

The Travelling Salesman Problem is an optimization problem where the goal is to
find the shortest possible path that visits a given set of cities and returns
to the starting city. It can be seen as the computation of the shortest 
Hamiltonian cycle of a graph.

## Branch and bound

This repository provides a solver for the TSP using an operational
research algorithm (branch and bound). A branch and bound algorithm
is an algorithm that breaks a problem into sub-problems and follows
the best paths according to a bound estimation.

For that, multiple bounding algorithms are implemented:
- Minimum weight edges bounding: bounding using the sum of the minimum edges
  that go from one unvisited node to another one. The algorithm does not give a
  valid solution, since the edges are not always connected.
- Minimum spanning tree (MST): bounding using the tree with minimal weight. The
  approximation may not be valid since the solution is not always a cycle.
- Held-Karp (HK) approximation: bounding using 1-trees. A 1-tree is essentially
  a MST built on $n - 1$ nodes, to which we add an edge. It is a minimally
  weighted graph with a cycle. HK showed that we can assign weights to nodes
  with a degree greater than 2 to create a larger cycle. This leads to an
  iterative algorithm.

## Usage

The binary takes a file that contains the number of cities and a distance
matrix. You can find some good material on
[TSPLIB](http://comopt.ifi.uni-heidelberg.de/software/TSPLIB95/).

```bash
# compilation
make

# creating a distance matrix
cat > file.txt <<EOF
5
0.0 3.0 4.0 2.0 7.0
3.0 0.0 4.0 6.0 3.0
4.0 4.0 0.0 5.0 8.0
2.0 6.0 5.0 0.0 6.0
7.0 3.0 8.0 6.0 0.0
EOF

# running with 8 threads
OMP_PROC_BIND=spread OMP_NUM_THREADS=8 ./tsp file.txt
```

## References

<!-- ltex: enabled=false -->
- Benchimol, Pascal, Jean-Charles Régin, Louis-Martin Rousseau, Michel Rueher,
  et Willem-Jan van Hoeve. 2010. "Improving the Held and Karp Approach with
  Constraint Programming". In, 40-44.
  [doi.org](https://doi.org/10.1007/978-3-642-13520-0_6.)
- Panangadan, Anand, et Prof Korf. 2001. "Incremental Evaluation of Minimum
  Spanning Trees for the Traveling Salesman Problem.", mars.
<!-- ltex: enabled=true -->

<!-- vim: set spelllang=en: -->
