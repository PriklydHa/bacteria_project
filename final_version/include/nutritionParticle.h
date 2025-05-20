#ifndef NUTRITION_PARTICLE_H
#define NUTRITION_PARTICLE_H

#include "particle.h"
#include <cmath>       // Для математических функций
#include <stdexcept>

class NutritionParticle : public Particle {
private:
    double nutrientValue;

public:
    NutritionParticle(double x, double y, double size, double nutrientValue)
        : Particle(x, y, size), nutrientValue(nutrientValue) {}

    // Исправленный метод move
    void move(double orientation, double speed) override {
        x += speed * std::cos(orientation);
        y += speed * std::sin(orientation);
    }

    NutritionParticle* clone() const override {
        return new NutritionParticle(x, y, size, nutrientValue);
    }

    double getNutrientValue() const { return nutrientValue; }
    void setNutrientValue(double value) { nutrientValue = value; }

    friend std::ostream& operator<<(std::ostream& os, const NutritionParticle& particle);
};

#endif