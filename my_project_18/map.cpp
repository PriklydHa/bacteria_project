#include "map.h"
#include <cmath>
#include <iostream>

/*Map::Map() {
}

Map::Map(const Map& other) {
    // Копирующий конструктор
    for (auto obj : other.objects) {
        objects.push_back(new Particle(*obj)); // Создаем новые объекты Particle
    }
}

Map& Map::operator=(const Map& other) {
    if (this != &other) {
        // Очищаем текущий вектор объектов
        for (auto obj : objects) {
            delete obj;
        }
        objects.clear();

        // Копируем объекты из другого вектора
        for (auto obj : other.objects) {
            objects.push_back(new Particle(*obj)); // Создаем новые объекты Particle
        }
    }
    return *this;
}

Map::~Map() {
    // Деструктор
    for (auto obj : objects) {
        delete obj;
    }
}
*/


Map::Map(const Map& other) {// Копирующий конструктор
    for (auto obj : other.objects) {
        objects.push_back(new Particle(*obj)); // Создаем новые объекты Particle
    }
}

Map& Map::operator=(const Map& other) {
    if (this != &other) {
        // Очищаем текущий вектор объектов
        for (auto obj : objects) {
            delete obj;
        }
        objects.clear();

        // Копируем объекты из другого вектора
        for (auto obj : other.objects) {
            objects.push_back(new Particle(*obj)); // Создаем новые объекты Particle
        }
    }
    return *this;
}


void Map::addObject(Particle* obj) {
    objects.push_back(obj);
}

std::vector<Particle*> Map::getObjectsNear(double x, double y, double radius) const {
    std::vector<Particle*> nearbyObjects;
    for (auto obj : objects) {
        double dx = obj->getX() - x;
        double dy = obj->getY() - y;
        if (sqrt(dx * dx + dy * dy) <= radius) {
            nearbyObjects.push_back(obj);
        }
    }
    return nearbyObjects;
}

void Map::display() const {
    for (auto obj : objects) {
        std::cout << "Object at (" << obj->getX() << ", " << obj->getY() << ")\n";
    }
}
