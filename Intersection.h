#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "glm-0.9.7.1/glm/glm.hpp"
#include "Ray.h"
#include <limits>

// Material of the object
struct Material {
  glm::vec3 ambient = glm::vec3(0.5f, 0.5f, 0.5f);
  glm::vec3 diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 specular = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 emision = glm::vec3(0.5f, 0.5f, 0.5f);
  float shininess = 10.0f;
};

// Intersection of a ray and an object, hittable object
struct Intersection {
    glm::vec3 pos;      // position of the intersection
    glm::vec3 normal;   // surface normal
    // distance to the source of object, t
    // INITIALIZE TO INFINITY
    float t = std::numeric_limits<double>::infinity();
    glm::vec3 dir;      // direction to the in-coming ray
    Material* material = new Material;  // pointers to material 
    bool isFront;       // is interesction inside or outside 
    bool frontOnly = false;     // is there intersection in the front
};

class Object {
  public:

    virtual Intersection getIntersection(Ray* ray, float t_min, float t_max) = 0;

};

#endif