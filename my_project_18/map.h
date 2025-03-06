#ifndef MAP_H
#define MAP_H

#include <vector>
#include "bacteria.h"
#include "nutritionParticle.h"
#include "matrixParticle.h"

class Map {
private:
    double width, height;  // Размеры среды
    std::vector<Particle*> objects;  // Массив объектов среды

public:
    Map(double width, double height) : width(width), height(height) {}
    Map(const Map& other);            // Копирующий конструктор
    Map& operator=(const Map& other); // Оператор присваивания
    ~Map() {
    for (auto obj : objects) {
        delete obj;
        }
    }

    /*Map();
    Map(const Map& other);            // Копирующий конструктор
    Map& operator=(const Map& other); // Оператор присваивания
    ~Map();*/

    void addObject(Particle* obj);
    std::vector<Particle*> getObjectsNear(double x, double y, double radius) const;
    void display() const;
};

#endif
