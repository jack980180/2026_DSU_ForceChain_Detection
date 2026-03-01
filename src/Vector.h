//
// Created by 黃少榆 on 2026/2/26.
//

#ifndef FORCECHAINSCANNER_VECTOR_H
#define FORCECHAINSCANNER_VECTOR_H


#pragma once

class Vector {
private:
    int* data;
    int current_size;

    void resize(int new_size);
    // Function: resize
    // Purpose: Reallocates the internal array to a new size, preserving existing elements up to the new size.
    // Pre: new_size >= 0.
    // Post: The internal array is reallocated to new_size. Existing elements are copied over. Any new elements are initialized to 0. Old memory is freed. current_size is updated.

public:
    Vector();
    // Function: Vector (Default Constructor)
    // Purpose: Initializes an empty vector.
    // Pre: None.
    // Post: data is set to nullptr and current_size is 0.

    Vector(int size, int initial_value = 0);
    // Function: Vector (Overloaded Constructor)
    // Purpose: Initializes a vector of a specific size with a default value.
    // Pre: size >= 0.
    // Post: Allocates memory for 'size' integers and sets them all to 'initial_value'.

    ~Vector();
    // Function: ~Vector (Destructor)
    // Purpose: Frees the dynamically allocated heap memory.
    // Pre: None.
    // Post: The memory pointed to by 'data' is released back to the OS.

    int& operator[](int index);
    // Function: operator[]
    // Purpose: Allows read/write access to elements using bracket notation.
    // Pre: 0 <= index < current_size.
    // Post: Returns a reference to the element at the specified index.

    int size() const;
    // Function: size
    // Purpose: Returns the number of elements in the vector.
    // Pre: None.
    // Post: Returns current_size.

    // Prevent copying to avoid double-free errors
    Vector(const Vector&) = delete;
    Vector& operator=(const Vector&) = delete;
};

#endif //FORCECHAINSCANNER_VECTOR_H