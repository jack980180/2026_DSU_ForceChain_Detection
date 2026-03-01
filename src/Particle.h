//
// Created by 黃少榆 on 2026/2/26.
//

#ifndef FORCECHAINSCANNER_PARTICLE_H
#define FORCECHAINSCANNER_PARTICLE_H

#pragma once

class Particle {
private:
    int id;           // Unique ID corresponding to DSU index
    double x, y;      // Coordinates
    double radius;    // Particle size
    double force;     // Total force magnitude (for force chain thresholding)

public:
    Particle();
    // Function: Particle (Default Constructor)
    // Purpose: Initializes a particle with zeroed values.
    // Pre: None.
    // Post: id is -1, x, y, radius, and force are 0.0.

    Particle(int id, double x, double y, double r);
    // Function: Particle (Overloaded Constructor)
    // Purpose: Initializes a particle with specific geometry and ID.
    // Pre: id >= 0, r > 0.
    // Post: Particle is created with given spatial and identity info.

    // Getters and Setters
    int get_id() const;
    double get_x() const;
    double get_y() const;
    double get_radius() const;

    void set_force(double f);
    void set_pos(double new_x, double new_y);
    double get_force() const;

    // Physics Helpers
    double distance_to(const Particle& other) const;
    // Function: distance_to
    // Purpose: Calculates Euclidean distance between this particle and another.
    // Pre: other is a valid Particle.
    // Post: Returns the distance between centers.
};

#endif //FORCECHAINSCANNER_PARTICLE_H