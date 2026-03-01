#include "SimulationDriver.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <ctime>

// --- Constructor & Destructor ---

SimulationDriver::SimulationDriver(double w, double h, double r, double phi) 
    : width(w), height(h), radius(r), target_phi(phi) {
    calculate_particle_count();
    particles = new std::vector<Particle>();
    dsu = new DisjointSet(num_particles + 2);
    box = new SimulationBox(width, height, num_particles);
}

SimulationDriver::~SimulationDriver() {
    // RAII principle: The class cleans up its own heap memory when destroyed.
    // This prevents memory leaks even if the program exits early.
    delete dsu;
    delete box;
    delete particles;
}

// --- Internal Helpers ---

void SimulationDriver::calculate_particle_count() {
    double particle_area = M_PI * std::pow(radius, 2);
    double box_area = width * height;
    num_particles = static_cast<int>((target_phi * box_area) / particle_area);
}

// --- Core Pipeline Methods ---

void SimulationDriver::initialize_and_relax(int iterations, double step) {
    std::cout << "--- Simulation Config ---" << std::endl;
    std::cout << "Target Phi: " << target_phi << " | Particles: " << num_particles << std::endl;

    std::srand(std::time(0));
    particles->reserve(num_particles);
    
    // Generate Random Configuration
    for (int i = 0; i < num_particles; ++i) {
        double rx = (static_cast<double>(std::rand()) / RAND_MAX) * width;
        double ry = (static_cast<double>(std::rand()) / RAND_MAX) * height;
        particles->push_back(Particle(i, rx, ry, radius));
    }

    // Relax particles to resolve physical overlaps
    std::cout << "Relaxing particles to remove overlaps..." << std::endl;
    box->relax_particles(*particles, iterations, step);
}

void SimulationDriver::perform_connectivity_scan(double force_factor) {
    dsu->reset();

    // Fix floating point math
    const double tolerance = 0.1;

    for (int i = 0; i < num_particles; ++i) {
        Particle& p_i = (*particles)[i];

        if (box->is_touching_top(p_i)) dsu->unite(p_i.get_id(), box->get_top_id());
        if (box->is_touching_bottom(p_i)) dsu->unite(p_i.get_id(), box->get_bottom_id());

        for (int j = i + 1; j < num_particles; ++j) {
            Particle& p_j = (*particles)[j];
            double dist = box->get_distance(p_i, p_j);

            // THE FIX: Factor multiplies the radius, but we still add the absolute tolerance buffer
            double threshold = ((p_i.get_radius() + p_j.get_radius()) * force_factor) + tolerance;

            if (dist <= threshold) {
                dsu->unite(p_i.get_id(), p_j.get_id());
            }
        }
    }
}

void SimulationDriver::print_results() const {
    std::cout << "\n--- Results ---" << std::endl;
    std::cout << "Max Cluster Size: " << dsu->get_max_cluster_size() << std::endl;

    if (dsu->connected(box->get_top_id(), box->get_bottom_id())) {
        std::cout << "Status: [JAMMED] - System percolates!" << std::endl;
    } else {
        std::cout << "Status: [UNJAMMED] - System is dilute." << std::endl;
    }
}

// --- File I/O ---

void SimulationDriver::save_particles_to_csv(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << filename << " for writing.\n";
        return;
    }

    // 1. Add 'is_spanning' to the CSV header
    file << "id,x,y,radius,is_spanning\n";

    // 2. Determine if the system is jammed, and get the root ID of the top plate
    int top_root = dsu->find(box->get_top_id());
    int bottom_root = dsu->find(box->get_bottom_id());
    bool is_jammed = (top_root == bottom_root);

    // 3. Write data
    for (const auto& p : *particles) {
        int p_root = dsu->find(p.get_id());

        // If system is jammed AND this particle shares the same root as the top plate, it's in the chain!
        int is_spanning = (is_jammed && p_root == top_root) ? 1 : 0;

        file << p.get_id() << ","
             << p.get_x() << ","
             << p.get_y() << ","
             << p.get_radius() << ","
             << is_spanning << "\n";
    }
    file.close();
    std::cout << "Saved particle configuration to " << filename << "\n";
}

bool SimulationDriver::load_particles_from_csv(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    // Clear existing particles if any
    particles->clear();

    std::string line;
    std::getline(file, line); // Skip header line

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string token;
        int id;
        double x, y, r;

        std::getline(ss, token, ','); id = std::stoi(token);
        std::getline(ss, token, ','); x = std::stod(token);
        std::getline(ss, token, ','); y = std::stod(token);
        std::getline(ss, token, ','); r = std::stod(token);

        particles->push_back(Particle(id, x, y, r));
    }
    file.close();

    // Update simulation state based on the loaded file
    num_particles = particles->size();
    
    // We must re-allocate the DSU and Box to account for the new particle count
    delete dsu;
    delete box;
    dsu = new DisjointSet(num_particles + 2);
    box = new SimulationBox(width, height, num_particles);

    std::cout << "Successfully loaded " << num_particles << " particles from " << filename << ".\n";
    return true;
}