#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdexcept>
#include "../src/Vector.h"

// Tiny helper to print headers
void print_test_header(const char* name) {
    printf("\n================ %s ================\n", name);
}

// ============================================================
// CUSTOM VECTOR TESTS
// ============================================================
void test_vector() {
    print_test_header("TESTING VECTOR CLASS");

    // Test 1: Default Constructor
    Vector v1;
    assert(v1.size() == 0);
    printf("[PASS] Default Constructor (Size 0)\n");

    // Test 2: Overloaded Constructor Initialization
    Vector v2(10, 5); // Size 10, all initialized to 5
    assert(v2.size() == 10);
    for (int i = 0; i < 10; ++i) {
        assert(v2[i] == 5);
    }
    printf("[PASS] Overloaded Constructor (Allocation & Initialization)\n");

    // Test 3: Element Read/Write
    v2[0] = 42;
    v2[9] = 99;
    assert(v2[0] == 42);
    assert(v2[9] == 99);
    assert(v2[1] == 5); // Unchanged element
    printf("[PASS] Array Bracket Operator Read/Write\n");

    // Test 4: Out of Bounds Exception (Negative Index)
    bool caught_negative = false;
    try {
        v2[-1] = 10;
    } catch (const std::out_of_range& e) {
        caught_negative = true;
    }
    assert(caught_negative);
    printf("[PASS] Exception thrown on negative index access\n");

    // Test 5: Out of Bounds Exception (Index >= Size)
    bool caught_overflow = false;
    try {
        v2[10] = 10;
    } catch (const std::out_of_range& e) {
        caught_overflow = true;
    }
    assert(caught_overflow);
    printf("[PASS] Exception thrown on out-of-bounds index access\n");

    printf("Vector Tests Complete!\n");
}

int main() {
    test_vector();

    printf("\n\nAll Vector Tests Passed! Your custom memory management is solid.\n");
    return 0;
}