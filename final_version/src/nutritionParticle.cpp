#include "nutritionParticle.h"

std::ostream& operator<<(std::ostream& os, const NutritionParticle& particle) {
    os << "NutritionParticle at (" 
       << particle.getX() << ", " << particle.getY()  // Используем геттеры!
       << ") with nutrientValue = " 
       << particle.getNutrientValue();  // И геттер для nutrientValue
    return os;
}