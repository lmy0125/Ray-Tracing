#ifndef SPHERE_H
#define SPHERE_H

#include <iostream>
#include <limits>
#include "glm-0.9.7.1/glm/glm.hpp"
#include "Intersection.h"

struct Material {
  glm::vec4 ambient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
  glm::vec4 diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
  glm::vec4 specular = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
  glm::vec4 emision = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
  float shininess = 10.0f;
};

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
  // Intersection intersection;
  // glm::vec3 pc = ray->ori - center;   // p - c
  // float b = glm::dot(ray->dir, pc);
  // float c = pow(glm::length(pc), 2) - pow(radius,2);
  // float discriminant = pow(b,2) - c;
  
  // // no intersection
  // if (discriminant < 0) {
  //   // intersection.t = -1*std::numeric_limits<double>::infinity();
  //   return intersection;
  // }
  // float t = -b + sqrt(discriminant);
  // glm::vec3 hitPoint = ray->ori + t * ray->dir;
  // intersection.pos = hitPoint;
  // intersection.normal = glm::normalize(center-hitPoint);
  // intersection.t = t;
  // std::cerr << glm::to_string(intersection.normal) << "\n";
  // return intersection;


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
    intersection.normal = glm::vec3(0,0,0);
    return intersection;
  } else {  // return nearest intersection
    float t = -b - sqrt(disc);
    if (t < 0) {
      Intersection intersection;
      intersection.normal = glm::vec3(0,0,0);
      return intersection;
    }
    glm::vec3 intersectPt = ray->ori + t * ray->dir;

    //naive normal using center - intersectionPoint
    glm::vec3 normal = center - intersectPt;
    normal = glm::normalize(normal);

    Intersection intersection;
    intersection.pos = intersectPt;
    intersection.normal = normal;
    intersection.t = t;
    // std::cerr << glm::to_string(intersection.normal) << "\n";
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