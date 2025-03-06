#include <iostream>
#include "bacteria.h"
#include "matrixParticle.h"
#include "map.h"

int main() {
    try {
        // Создаем объект карты с необходимыми параметрами (например, размеры карты)
        Map simulationMap();
      //  Map simulationMap(100.0, 100.0); // 100x100 - размеры карты
        std::cout << "Simulation map created.\n";

        // Создаем бактерию
        Bacteria bacteria(10.0, 10.0, 1.0, 0.0, 1.0, 10.0);  // x, y, size, orientation, speed, nutrition
        std::cout << "Initial bacteria state:\n";
        std::cout << "Position: (" << bacteria.getX() << ", " << bacteria.getY() << ")\n";
        std::cout << "Nutrition: " << bacteria.getNutrition() << "\n";

        // Двигаем бактерию
        std::cout << "\nMoving bacteria...\n";
        bacteria.move(0.785398, 1.0);  // Move at 45 degrees (0.785398 radians), speed = 1.0
        std::cout << "Bacteria position after move: (" << bacteria.getX() << ", " << bacteria.getY() << ")\n";

        // Проверяем выделение матрикса
        std::cout << "\nExcreting matrix particle...\n";
        bacteria.excrete(5.0, simulationMap);  // Выделение матрикса размером 5.0
        std::cout << "Nutrition after excretion: " << bacteria.getNutrition() << "\n";

        // Проверяем объекты рядом с бактерией
        std::cout << "\nObjects near the bacteria:\n";
        auto objectsNearby = simulationMap.getObjectsNear(bacteria.getX(), bacteria.getY(), 10.0); // Радиус поиска = 10
        for (Particle* obj : objectsNearby) {
            std::cout << "Object at position (" << obj->getX() << ", " << obj->getY() << "), size: " << obj->getSize() << "\n";
        }

        // Пробуем выделить больше матриксов, чем есть ресурсов
        std::cout << "\nAttempting to excrete with insufficient nutrition...\n";
        bacteria.excrete(10.0, simulationMap);

        // Проверяем объекты вокруг бактерии снова
        std::cout << "\nFinal objects near the bacteria:\n";
        auto finalObjectsNearby = simulationMap.getObjectsNear(bacteria.getX(), bacteria.getY(), 20.0); // Радиус поиска = 20
        for (Particle* obj : finalObjectsNearby) {
            std::cout << "Object at position (" << obj->getX() << ", " << obj->getY() << "), size: " << obj->getSize() << "\n";
        }

        
    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << "\n";
    }

    return 0;
}
