

#ifndef FORCECHAINSCANNER_SIMULATIONDRIVER_H
#define FORCECHAINSCANNER_SIMULATIONDRIVER_H

#pragma once

#include <vector>
#include <string>
#include "SimulationBox.h"
#include "Particle.h"
#include "DisjointSet.h"

class SimulationDriver {
private:
    // Simulation Parameters
    double width;
    double height;
    double radius;
    double target_phi;
    int num_particles;

    // Core Components (Managed on the Heap)
    DisjointSet* dsu;
    SimulationBox* box;
    std::vector<Particle>* particles;

    // Helper method
    void calculate_particle_count();

public:
    // Constructor & Destructor
    SimulationDriver(double w, double h, double r, double phi);
    ~SimulationDriver(); // Handles all the 'delete' cleanup!

    // Core Pipeline Methods
    void initialize_and_relax(int iterations = 1000, double step = 0.2);
    void perform_connectivity_scan(double force_factor = 1.0);
    void print_results() const;

    // File I/O
    void save_particles_to_csv(const std::string& filename) const;
    bool load_particles_from_csv(const std::string& filename);
};


#endif //FORCECHAINSCANNER_SIMULATIONDRIVER_H