//
// Created by 黃少榆 on 2026/2/26.
//

#ifndef FORCECHAINSCANNER_SIMULATIONBOX_H
#define FORCECHAINSCANNER_SIMULATIONBOX_H

#pragma once

#include <vector>

#include "Vector.h"
#include "Particle.h"
#include "DisjointSet.h"

class SimulationBox {
private:
    double width;  // x-range [0, width]
    double height; // y-range [0, height]

    // IDs for virtual "particles" representing the plates
    int top_boundary_id;
    int bottom_boundary_id;

public:
    SimulationBox(double w, double h, int num_particles);
    // Function: SimulationBox
    // Purpose: Initializes the environment dimensions and defines boundary IDs.
    // Pre: w > 0, h > 0, num_particles > 0.
    // Post: Sets width/height. Plate IDs are set to num_particles and num_particles + 1.

    double get_periodic_dx(double x1, double x2) const;
    // Function: get_periodic_dx
    // Purpose: Calculates the shortest horizontal distance considering circulation.
    // Pre: None.
    // Post: Returns dx, adjusted so that |dx| <= width / 2.

    double get_distance(const Particle& p1, const Particle& p2) const;
    // Function: get_distance
    // Purpose: Calculates Euclidean distance using PBC in x and standard distance in y.
    // Pre: None.
    // Post: Returns the shortest distance between particle centers.

    void relax_particles(std::vector<Particle>& particles, int iterations, double step_size);
    // Function: relax_particles
    // Purpose: Reduces overlaps by moving overlapping particles apart.
    // Pre: particles vector is populated. iterations > 0, step_size > 0.
    // Post: Particle coordinates are updated to reduce potential energy/overlap.

    bool is_touching_top(const Particle& p) const;
    bool is_touching_bottom(const Particle& p) const;
    // Purpose: Checks if a particle is physically in contact with the top or bottom wall.

    // Getters
    int get_top_id() const { return top_boundary_id; }
    int get_bottom_id() const { return bottom_boundary_id; }
};

#endif //FORCECHAINSCANNER_SIMULATIONBOX_H