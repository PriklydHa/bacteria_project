#ifndef MATRIX_PARTICLE_H
#define MATRIX_PARTICLE_H

#include "particle.h"

class MatrixParticle : public Particle {
public:
    MatrixParticle(double x, double y, double size) 
        : Particle(x, y, size) {}

    void move(double orientation, double speed) override { std::cout << "MatrixParticle does not move.\n"; }
};

#endif
