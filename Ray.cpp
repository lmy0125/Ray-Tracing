#include "Ray.h"

Ray::ray(glm::vec3 &origin, glm::vec3 &direction)
    : ori(origin), dir(direction)
{}

glm::vec3 at(float t) {
    return ori + t*dir;
}