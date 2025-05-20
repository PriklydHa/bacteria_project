#ifndef MATRIX_PARTICLE_H
#define MATRIX_PARTICLE_H

#include "particle.h"
#include <stdexcept>  // Для std::runtime_error
#include <iostream>   // Для std::cout

class MatrixParticle : public Particle {
public:
    MatrixParticle(double x, double y, double size) 
        : Particle(x, y, size) {}

    // Реализация move (вывод сообщения)
    void move(double /*orientation*/, double /*speed*/) override {
        std::cout << "DEBUG: MatrixParticle::move called!" << std::endl;
        std::cout << "MatrixParticle does not move."<< std::endl;
    }

    MatrixParticle* clone() const override {
        return new MatrixParticle(x, y, size);
    }
};

#endif