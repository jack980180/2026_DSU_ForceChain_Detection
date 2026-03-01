#include "DisjointSet.h"

#include <stdexcept>

DisjointSet::DisjointSet(int n) {
    // Safety check
    if (n <= 0) {
        throw std::invalid_argument("Number of particles must be greater "
                                    "than 0");
    }

    // Initialize variables
    num_elements = n;
    num_clusters = n;
    max_cluster_size = 1;

    // Allocate and initialize the vectors
    parent = new Vector(n, 0);
    size = new Vector(n, 1);

    // Initialize every particle as root
    for (int i = 0; i < n; ++i) {
        (*parent)[i] = i;
    }
}

DisjointSet::~DisjointSet() {
    delete parent;
    delete size;
}


int DisjointSet::find(int p) {
    // Safety check
    if (p < 0 || p >= num_elements) {
        throw std::invalid_argument("index of particles must be positive and "
                                    "smaller than number of particles");
    }

    // Find the root and update the path
    if ((*parent)[p] == p) {
        return p;
    }

    (*parent)[p] = find((*parent)[p]);
    return (*parent)[p];
}

void DisjointSet::unite(int p, int q) {
    // Safety check
    if (p < 0 || p >= num_elements || q < 0 || q >= num_elements) {
        throw std::invalid_argument("Particle indices out of bounds.");
    }

    // Find the roots
    int root_p = find(p);
    int root_q = find(q);

    // Already in the same cluster
    if (root_p == root_q) {
        return;
    }

    // Since they are different clusters merging, the total number of clusters drops by 1
    num_clusters--;

    // 3 & 4. Union by Size and updating trackers
    if ((*size)[root_p] < (*size)[root_q]) {
        // root_p is smaller. Attach it to root_q.
        (*parent)[root_p] = root_q;
        (*size)[root_q] += (*size)[root_p];
        // Update max size
        if ((*size)[root_q] > max_cluster_size) {
            max_cluster_size = (*size)[root_q];
        }

    } else {
        // root_q is smaller (or they are equal). Attach it to root_p.
        (*parent)[root_q] = root_p;
        (*size)[root_p] += (*size)[root_q];
        // Update max size
        if ((*size)[root_p] > max_cluster_size) {
            max_cluster_size = (*size)[root_p];
        }
    }
}

bool DisjointSet::connected(int p, int q) {
    // Safety check
    if (p < 0 || p >= num_elements || q < 0 || q >= num_elements) {
        throw std::invalid_argument("Particle indices out of bounds.");
    }

    // Find the roots
    int root_p = find(p);
    int root_q = find(q);

    // Return true if p and q are in the same cluster
    return root_p == root_q;
}

int DisjointSet::get_cluster_size(int p) {
    if (p < 0 || p >= num_elements) {
        throw std::invalid_argument("index of particles must be positive and "
                                    "smaller than number of particles");
    }

    // Find the root and get the cluster size in size vector
    int root = find(p);

    return (*size)[root];
}

int DisjointSet::get_max_cluster_size() const {
    return max_cluster_size;
}

int DisjointSet::get_num_clusters() const {
    return num_clusters;
}

void DisjointSet::reset() {
    // Reset every particle as root
    max_cluster_size = 1;
    num_clusters = num_elements;

    // Reset the arrays for every single particle
    for (int i = 0; i < num_elements; ++i) {
        (*parent)[i] = i; // every particle is its own root again
        (*size)[i] = 1;   // every cluster size drops back down to 1
    }
}