#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "glm-0.9.7.1/glm/glm.hpp"
#include "Ray.h"
#include "Material.h"
#include <limits>

// Intersection of a ray and an object, hittable object
struct Intersection {
    glm::vec3 pos;      // position of the intersection
    glm::vec3 normal = glm::vec3(0.0f, 0.0f, 0.0f);   // surface normal
    float t = std::numeric_limits<double>::infinity();      // distance to the source of object, t
    glm::vec3 dir = glm::vec3(0.0f, 0.0f, 0.0f);      // direction to the in-coming ray
    Material* material = new Material;  // pointers to material 
    bool isFront;       // is interesction inside or outside 
    bool frontOnly = false;     // is there intersection in the front
    bool onHit = false;   // if the ray hits an object

    inline void setFrontNormal(Intersection* intersection, glm::vec3 outNormal) {
      isFront = glm::dot(intersection->dir, outNormal) < 0;
      normal = isFront ? outNormal : -outNormal;
    }
};

class Object {
  public:
    virtual Intersection getIntersection(Ray* ray, float t_min, float t_max) = 0;
};

#endif