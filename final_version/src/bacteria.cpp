#include "bacteria.h"
#include "nutritionParticle.h"
#include "map.h"
#include <memory>

void Bacteria::move(double orientation, double speed) {
    x += speed * cos(orientation) / size;
    y += speed * sin(orientation) / size;
}

void Bacteria::eat(double nutrient_value) {
    nutrition += nutrient_value;
}

void Bacteria::excrete(double matrixSize, Map& map) {
    if (nutrition >= matrixSize) {
        nutrition -= matrixSize;

        auto matrix = std::make_shared<MatrixParticle>(x, y, size);
        map.addObject(matrix);  

    } else {
        std::cerr << "Insufficient nutrition to excrete matrixParticle\n";
    }
}