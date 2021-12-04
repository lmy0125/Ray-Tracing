#include "glm-0.9.7.1/glm/glm.hpp"

inline float random_double() {
    // Returns a random real in [0,1).
    return rand() / (RAND_MAX + 1.0f);
}

inline double random_double(double min, double max) {
    // Returns a random real in [min,max).
    return min + (max-min)*random_double();
}