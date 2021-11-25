#ifndef RAY_H
#define RAY_H

#include "glm-0.9.7.1/glm/glm.hpp"

// P(t) = A + tb
class Ray {
    public:
        glm::vec3 ori;  // A
        glm::vec3 dir;  // b

        Ray() {}
        Ray(glm::vec3 origin, glm::vec3 direction)
            : ori(origin), dir(direction)
        {}

        glm::vec3 origin() { return ori; }
        glm::vec3 direction() { return dir; }

        glm::vec3 at(float t) {
            return ori + t*dir;
        }
};

#endif