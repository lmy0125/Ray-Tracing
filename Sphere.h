#ifndef SPHERE_H
#define SPHERE_H

#include <iostream>
#include "glm-0.9.7.1/glm/glm.hpp"
#include "Intersection.h"

class Sphere: public Object {
  public: 
    float radius;
    glm::vec3 center;
    Material* material;

    // constructor
    Sphere() {}
    Sphere(const glm::vec3 center, float radius, Material* material) 
      : center(center), radius(radius), material(material) {}

    virtual Intersection getIntersection(Ray* ray, float t_min, float t_max) override;
};

Intersection Sphere::getIntersection(Ray* ray, float t_min, float t_max) {
  //invalid input
  if (radius < 0){ 
    std::cerr << "Exception *** Sphere's raduis < 0.";
    return Intersection();
  }

  glm::vec3 diff = ray->ori - center;
  float b = dot(ray->dir, diff);

  //calculate discriminant
  float disc = b*b - pow(length(diff),2) + radius*radius;

  if (disc < 0){  // no intersection
    Intersection intersection;
    intersection.normal = glm::vec3(0.0f, 0.0f, 0.0f);
    return intersection;
  } else {  // return nearest intersection
    float t = -b - sqrt(disc);
    glm::vec3 intersectPt = ray->ori + t * ray->dir;

    //naive normal using center - intersectionPoint
    glm::vec3 normal = center - intersectPt;
    normal = glm::normalize(normal);

    Intersection intersection;
    intersection.pos = intersectPt;
    intersection.normal = normal;
    intersection.t = t;
    intersection.material = material;
    return intersection;
  }
}

#endif

// glm::vec3 IntersectSeph(Ray* ray, glm::vec3 c, float r){
//   //invalid input
//   if (r < 0){ 
//     std::cerr << "Exception *** Sphere's raduis < 0.";
//     return glm::vec3(0, 0, 0);
//   }

//   glm::vec3 diff = ray->ori - c;
//   float b = dot(ray->dir, diff);

//   //calculate discriminant
//   float disc = b*b - pow(length(diff),2) + r*r;

//   if (disc < 0){  // no intersection
//     return glm::vec3(0, 0, 0);
//   } else {  // return nearest intersection
//     float t = -b - sqrt(disc);
//     glm::vec3 intersectPt = ray->ori + t * ray->dir;

//     //naive normal using center - intersectionPoint
//     glm::vec3 normal = c - intersectPt;
//     normal = glm::normalize(normal);

//     return normal;
//   }
// }