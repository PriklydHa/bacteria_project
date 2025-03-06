#ifndef NUTRITION_PARTICLE_H
#define NUTRITION_PARTICLE_H

#include "particle.h"

class NutritionParticle : public Particle {
private:
    double nutrientValue;

public:
    NutritionParticle(double x, double y, double size, double nutrientValue)
        : Particle(x, y, size), nutrientValue(nutrientValue) {}

    double getNutrientValue() const { return nutrientValue; }
    void setNutrientValue(double value) { nutrientValue = value; }

    friend std::ostream& operator<<(std::ostream& os, const NutritionParticle& particle);
};

#endif
