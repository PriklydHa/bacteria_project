#ifndef BACTERIA_H
#define BACTERIA_H

#include "particle.h"
#include "nutritionParticle.h"
#include "matrixParticle.h"
#include "map.h"

class Map;

class Bacteria : public Particle {
private:
    double orientation;  // Угол движения (в радианах)
    double speed;        // Скорость движения
    double nutrition;    // Запас питательных веществ

public:
    Bacteria(double x, double y, double size, double orientation, double speed, double nutrition)
        : Particle(x, y, size), orientation(orientation), speed(speed), nutrition(nutrition) {}

    void move(double orientation, double speed) override;
    void eat(double nutrientValue);
    void excrete(double matrixSize, Map& map);

    double getOrientation() const { return orientation; }
    void setOrientation(double newOrientation) { orientation = newOrientation; }
    double getSpeed() const { return speed; }
    void setSpeed(double newSpeed) { speed = newSpeed; }
    double getNutrition() const { return nutrition; }
    void setNutrition(double newNutrition) { nutrition = newNutrition; }

    virtual Bacteria* clone() const { return new Bacteria(*this);};
};

#endif
