#include "Vector.h"
#include <stdexcept>

// Default constructor
Vector::Vector() : data(nullptr), current_size(0) {
}

// Overloaded constructor
Vector::Vector(int size, int initial_value) : data(nullptr), current_size(0) {
    if (size < 0) {
        throw std::invalid_argument("Vector size cannot be negative.");
    }

    // We can use our new resize function to handle the allocation!
    resize(size);

    for (int i = 0; i < current_size; ++i) {
        data[i] = initial_value;
    }
}

// Destructor
Vector::~Vector() {
    delete[] data;
}

// Private resize function
void Vector::resize(int new_size) {
    if (new_size == current_size) {
        return; // Nothing to do
    }
    if (new_size < 0) {
        throw std::invalid_argument("New size cannot be negative.");
    }

    // 1. Allocate the new block of memory
    int* new_data = nullptr;
    if (new_size > 0) {
        new_data = new int[new_size];

        // 2. Figure out how many elements we need to copy
        int elements_to_copy = (new_size < current_size) ? new_size : current_size;

        // 3. Copy old data to the new block
        for (int i = 0; i < elements_to_copy; ++i) {
            new_data[i] = data[i];
        }

        // 4. Initialize any newly added spaces to 0
        for (int i = elements_to_copy; i < new_size; ++i) {
            new_data[i] = 0;
        }
    }

    // 5. Delete the old memory block to prevent memory leaks
    delete[] data;

    // 6. Point our class variable to the new memory and update the size
    data = new_data;
    current_size = new_size;
}

// Bracket operator
int& Vector::operator[](int index) {
    if (index < 0 || index >= current_size) {
        throw std::out_of_range("Index out of bounds!");
    }
    return data[index];
}

// Size getter
int Vector::size() const {
    return current_size;
}