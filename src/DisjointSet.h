//
// Created by 黃少榆 on 2026/2/26.
//

#ifndef FORCECHAINSCANNER_DISJOINTSET_H
#define FORCECHAINSCANNER_DISJOINTSET_H

#pragma once

#include "Vector.h"

class DisjointSet {
private:
    Vector* parent; // Pointer to a Vector object allocated on the heap
    Vector* size;   // Pointer to a Vector object allocated on the heap
    int num_elements;
    int num_clusters;
    int max_cluster_size;

public:
    explicit DisjointSet(int n);
    // Function: DisjointSet (Constructor)
    // Purpose: Creates and initializes a disjoint set data structure for n particles.
    // Pre: n > 0.
    // Post: Dynamically allocates Vector objects for parent and size on the heap. Sets each element to be its own root, sizes to 1, num_clusters to n, and max_cluster_size to 1.

    ~DisjointSet();
    // Function: ~DisjointSet (Destructor)
    // Purpose: Frees the dynamically allocated Vector objects.
    // Pre: None.
    // Post: The memory pointed to by 'parent' and 'size' is released back to the OS.

    int find(int p);
    // Function: find
    // Purpose: Finds the root identifier of the cluster containing particle p, applying path compression.
    // Pre: 0 <= p < num_elements.
    // Post: Returns the integer index of the root of p. The path from p to the root is compressed.

    void unite(int p, int q);
    // Function: unite
    // Purpose: Merges the two clusters containing particle p and particle q using the union-by-size optimization.
    // Pre: 0 <= p < num_elements and 0 <= q < num_elements.
    // Post: If p and q were in different clusters, they are merged. Updates size, decrements num_clusters, and potentially updates max_cluster_size.

    bool connected(int p, int q);
    // Function: connected
    // Purpose: Determines if particle p and particle q are part of the same cluster.
    // Pre: 0 <= p < num_elements and 0 <= q < num_elements.
    // Post: Returns true if p and q share the same root, false otherwise.

    int get_cluster_size(int p);
    // Function: get_cluster_size
    // Purpose: Retrieves the total number of particles in the cluster that contains particle p.
    // Pre: 0 <= p < num_elements.
    // Post: Returns the size of the root's cluster containing p.

    int get_max_cluster_size() const;
    // Function: get_max_cluster_size
    // Purpose: Retrieves the size of the largest cluster currently existing in the system.
    // Pre: None.
    // Post: Returns the integer value of max_cluster_size.

    int get_num_clusters() const;
    // Function: get_num_clusters
    // Purpose: Retrieves the total number of distinct, disconnected clusters in the system.
    // Pre: None.
    // Post: Returns the integer value of num_clusters.

    void reset();
    // Function: reset
    // Purpose: Resets the disjoint set to its initial state using the existing Vector memory.
    // Pre: The object must have been successfully constructed.
    // Post: All parent pointers point to themselves, all sizes are reset to 1. Memory remains intact.
};

#endif //FORCECHAINSCANNER_DISJOINTSET_H