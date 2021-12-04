#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "glm-0.9.7.1/glm/glm.hpp"
#include "Ray.h"

// Material of the object
struct Material {
  glm::vec3 ambient = glm::vec3(0.5f, 0.0f, 0.0f);
  glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
  glm::vec3 specular = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 emision = glm::vec3(0.0f, 0.0f, 0.0f);
  float shininess = 10.0f;
};

// Intersection of a ray and an object, hittable object
struct Intersection {
    glm::vec3 pos;      // position of the intersection
    glm::vec3 normal;   // surface normal
    float t;            // distance to the source of object, t
    glm::vec3 dir;      // direction to the in-coming ray
    Material* material = new Material;  // pointers to material 
    bool isFront;       // is interesction inside of outside 
};

class Object {
  public:
    // constructor
    // Intersection() {}
    // Intersection(glm::vec3 pos, glm::vec3 normal, float t)
    //   : pos(pos), normal(normal), t(t) {}

    virtual Intersection getIntersection(Ray* ray, float t_min, float t_max) = 0;

    // searches over all geometries in the scene and returns the closest hit
    // Intersection IntersectScene(Ray* ray, float t_min, float t_max);
};

#endif