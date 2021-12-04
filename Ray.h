#ifndef RAY_H
#define RAY_H

#include "glm-0.9.7.1/glm/glm.hpp"

// P(t) = A + tb
class Ray {
    public:
        glm::vec3 ori;  // A
        glm::vec3 dir;  // b

        Ray() {}
        Ray(const glm::vec3 origin, const glm::vec3 direction)
            : ori(origin), dir(glm::normalize(direction))
        {}

        // glm::vec3 origin() { return ori; }
        // glm::vec3 direction() { return dir; }

        glm::vec3 at(float t) const {
            return ori + t*dir;
        }
};

#endif