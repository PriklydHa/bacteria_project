#ifndef MAP_H
#define MAP_H

#include <vector>
#include "bacteria.h"
#include "nutritionParticle.h"
#include "matrixParticle.h"
#include <memory> 

class Map {
private:
    double width, height;  // Размеры среды
    std::vector<std::shared_ptr<Particle>> objects; // Массив объектов среды

public:
    Map(double width, double height) : width(width), height(height) {}
    Map(const Map& other);            // Копирующий конструктор
    Map& operator=(const Map& other); // Оператор присваивания
    
    void addObject(std::shared_ptr<Particle> obj);  
    std::vector<std::shared_ptr<Particle>> getObjectsNear(double x, double y, double radius) const; // shared_ptr
    void display() const;
};

#endif
