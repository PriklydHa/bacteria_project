#include "nutritionParticle.h"


std::ostream& operator<<(std::ostream& os, const NutritionParticle& particle) {
    os << "nutritionParticle at (" << particle.x << ", " << particle.y << ") with nutrientValue = " << particle.nutrientValue;
    return os;
}
