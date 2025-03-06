#ifndef PARTICLE_H
#define PARTICLE_H

#include <iostream>
#include <cmath>

class Particle {
protected:
    double x, y;
    double size;

public:
    Particle(double x, double y, double size) : x(x), y(y), size(size) {}
    virtual ~Particle() = default;

    double getX() const {return x;}
    double getY() const {return y;}
    void setX(double newX) {x = newX;}
    void setY(double newY) {y = newY;}
    double getSize() const {return size;}
    void setSize(double newSize) {size = newSize;}

    virtual void move(double orientation, double speed) = 0;
    virtual Particle* clone() const = 0;

    friend std::ostream& operator<<(std::ostream& os, const Particle& particle) {
        os << "Particle at (" << particle.x << ", " << particle.y << ") with size " << particle.size;
        return os;
    }
};

#endif
