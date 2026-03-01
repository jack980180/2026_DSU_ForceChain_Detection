//
// Created by 黃少榆 on 2026/2/26.
//

#include "Particle.h"
#include <cmath>

Particle::Particle() : id(-1), x(0.0), y(0.0), radius(0.0), force(0.0) {}

Particle::Particle(int id, double x, double y, double r)
    : id(id), x(x), y(y), radius(r), force(0.0) {}

int Particle::get_id() const { return id; }
double Particle::get_x() const { return x; }
double Particle::get_y() const { return y; }
double Particle::get_radius() const { return radius; }

void Particle::set_force(double f) { force = f; }
void Particle::set_pos(double new_x, double new_y) {
    x = new_x;
    y = new_y;
}
double Particle::get_force() const { return force; }

double Particle::distance_to(const Particle& other) const {
    double dx = x - other.x;
    double dy = y - other.y;
    return std::sqrt(dx * dx + dy * dy);
}