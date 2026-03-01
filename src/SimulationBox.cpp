#include "SimulationBox.h"
#include <cmath>

SimulationBox::SimulationBox(double w, double h, int num_particles) 
    : width(w), height(h) {
    // We reserve the last two indices in our DSU for the plates
    top_boundary_id = num_particles; 
    bottom_boundary_id = num_particles + 1;
}

double SimulationBox::get_periodic_dx(double x1, double x2) const {
    double dx = x1 - x2;
    // Circulation logic: if distance is more than half the width, 
    // it's shorter to go the "other way" around the circle.
    if (dx > width * 0.5) dx -= width;
    if (dx < -width * 0.5) dx += width;
    return dx;
}

double SimulationBox::get_distance(const Particle& p1, const Particle& p2) const {
    double dx = get_periodic_dx(p1.get_x(), p2.get_x());
    double dy = p1.get_y() - p2.get_y();
    return std::sqrt(dx * dx + dy * dy);
}

bool SimulationBox::is_touching_top(const Particle& p) const {
    // Particle touches top if y + radius >= height
    return (p.get_y() + p.get_radius()) >= height;
}

bool SimulationBox::is_touching_bottom(const Particle& p) const {
    // Particle touches bottom if y - radius <= 0
    return (p.get_y() - p.get_radius()) <= 0;
}

void SimulationBox::relax_particles(std::vector<Particle>& particles, int iterations, double step_size) {
    int n = particles.size();

    // The Iteration Loop:
    // Pushing two particles apart might push them into a third particle.
    // Therefore, we have to repeat this entire process many times (e.g., 1000)
    // until the whole system settles into a stable state.
    for (int iter = 0; iter < iterations; ++iter) {

        // The O(N^2) Pairwise Scan:
        // Check every unique pair of particles (i and j) exactly once per iteration.
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {

                // 1. Calculate Distance
                // dx uses the periodic boundary function (e.g., if one particle is at x=1
                // and the other at x=199 in a 200-wide box, dx is 2, not 198).
                double dx = get_periodic_dx(particles[i].get_x(), particles[j].get_x());
                double dy = particles[i].get_y() - particles[j].get_y();
                double dist = std::sqrt(dx * dx + dy * dy);

                // The mathematical threshold for touching
                double sum_r = particles[i].get_radius() + particles[j].get_radius();

                // 2. Collision Detection
                // If dist < sum_r, they are overlapping.
                // We also check dist > 0 to prevent a divide-by-zero crash if two
                // particles spawned at the exact same coordinate.
                if (dist < sum_r && dist > 0) {

                    // 3. Normalization (Finding the Direction)
                    // nx and ny create a "Unit Vector" (length of 1) that points
                    // directly from particle j to particle i.
                    double nx = dx / dist;
                    double ny = dy / dist;

                    // 4. Calculate Displacement Vector
                    // We decide how far to push them along that line.
                    // We multiply by 0.5 because both particles share the work:
                    // i moves half the distance away from j, and j moves half the distance away from i.
                    double move_x = nx * step_size * 0.5;
                    double move_y = ny * step_size * 0.5;



                    // 5. Apply X Movement with Periodic Wrap-Around
                    // If moving the particle pushes it past the right wall (width),
                    // it teleports to the left side (new_ix -= width).
                    // If it goes past the left wall (0), it teleports to the right.
                    double new_ix = particles[i].get_x() + move_x;
                    if (new_ix < 0) new_ix += width;
                    if (new_ix >= width) new_ix -= width;

                    double new_jx = particles[j].get_x() - move_x;
                    if (new_jx < 0) new_jx += width;
                    if (new_jx >= width) new_jx -= width;

                    // 6. Apply Y Movement with Hard Wall Constraints
                    // The Y-axis represents the solid top and bottom plates.
                    // std::min and std::max act like a vice: they clamp the particle's
                    // position so its center can never get closer to a wall than its own radius.
                    double new_iy = std::max(particles[i].get_radius(),
                                    std::min(height - particles[i].get_radius(), particles[i].get_y() + move_y));

                    double new_jy = std::max(particles[j].get_radius(),
                                    std::min(height - particles[j].get_radius(), particles[j].get_y() - move_y));

                    // 7. Update the System
                    // Actually overwrite the old positions with the new physics-corrected ones.
                    particles[i].set_pos(new_ix, new_iy);
                    particles[j].set_pos(new_jx, new_jy);
                }
            }
        }
    }
}