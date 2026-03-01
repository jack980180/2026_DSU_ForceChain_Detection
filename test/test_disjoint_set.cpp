#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdexcept>
#include "../src/DisjointSet.h"

// Tiny helper to print headers
void print_test_header(const char* name) {
    printf("\n================ %s ================\n", name);
}

// ============================================================
// DISJOINT SET / FORCE CHAIN TESTS
// ============================================================
void test_disjoint_set() {
    print_test_header("TESTING DISJOINT SET (DSU)");

    int N = 10;
    DisjointSet dsu(N);

    // Test 1: Initial State
    assert(dsu.get_num_clusters() == N);
    assert(dsu.get_max_cluster_size() == 1);
    for (int i = 0; i < N; ++i) {
        assert(dsu.get_cluster_size(i) == 1);
        assert(dsu.connected(i, i) == true);
    }
    printf("[PASS] Initial State (All particles isolated)\n");

    // Test 2: Basic Union & Connectivity
    dsu.unite(0, 1);
    assert(dsu.connected(0, 1) == true);
    assert(dsu.connected(0, 2) == false);
    assert(dsu.get_num_clusters() == N - 1);
    assert(dsu.get_cluster_size(0) == 2);
    assert(dsu.get_max_cluster_size() == 2);
    printf("[PASS] Basic Unite (Particles 0 and 1 merged)\n");

    // Test 3: Union by Size (Building a larger force chain)
    dsu.unite(2, 3);
    dsu.unite(0, 2); // Merging cluster {0,1} with {2,3}
    assert(dsu.connected(1, 3) == true); // Transitive connection!
    assert(dsu.get_cluster_size(3) == 4);
    assert(dsu.get_max_cluster_size() == 4);
    assert(dsu.get_num_clusters() == N - 3);
    printf("[PASS] Force Chain Merging & Max Cluster Tracking\n");

    // Test 4: Redundant Union (Should do nothing)
    dsu.unite(1, 2); // Already in the same cluster
    assert(dsu.get_num_clusters() == N - 3); // Should not decrease
    assert(dsu.get_max_cluster_size() == 4); // Should not increase
    printf("[PASS] Redundant Union Ignored Correctly\n");

    // Test 5: Fast Reset for Next Simulation Frame
    dsu.reset();
    assert(dsu.get_num_clusters() == N);
    assert(dsu.get_max_cluster_size() == 1);
    assert(dsu.connected(0, 1) == false); // Should be disconnected again
    assert(dsu.get_cluster_size(0) == 1);
    printf("[PASS] DSU Reset (Ready for next frame)\n");

    // Test 6: Safety Exceptions
    bool caught_bounds = false;
    try {
        dsu.unite(0, 99); // 99 is out of bounds
    } catch (const std::invalid_argument& e) {
        caught_bounds = true;
    }
    assert(caught_bounds);
    printf("[PASS] Exception thrown on out-of-bounds particle index\n");

    printf("Disjoint Set Tests Complete!\n");
}

int main() {
    test_disjoint_set();

    printf("\n\nAll Disjoint Set Tests Passed! Your force chain scanner is ready.\n");
    return 0;
}