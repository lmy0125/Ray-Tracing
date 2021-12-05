#ifndef SCENE_H
#define SCENE_H

#include "Intersection.h"
#include "Light.h"

#include <iostream>
#include <vector>
#include <limits>
// #include <algorithm>

class Scene: public Object {
  public:
    std::vector<Object*> objects;
    std::vector<Light*> lights;

    Scene() {}
    Scene(Object* object, Light* light) { 
      objects.push_back(object); 
      lights.push_back(light);
    }

    void add_obj(Object* object) { objects.push_back(object); };
    void add_light(Light* light) { lights.push_back(light); };
    void clear() { objects.clear(); }

    virtual Intersection getIntersection(Ray* ray, float t_min, float t_max) override;
    glm::vec3 findColor(Intersection* intersection, int depth);
};

// searches over all geometries in the scene and returns the closest hit
Intersection Scene::getIntersection(Ray* ray, float t_min, float t_max) {
  float minDist = std::numeric_limits<double>::infinity();
  Intersection hit;
  hit.dir = ray->dir;  // merely for background color
  bool frontOnly = false;

  for (Object* object : objects) {
    Intersection hit_temp = object->getIntersection(ray, t_min, t_max);
    if (hit_temp.frontOnly){ frontOnly = true; };
    if (hit_temp.t < minDist) {
      minDist = hit_temp.t;
      hit = hit_temp;
    }
  }

  if (frontOnly){ hit.frontOnly = true;};

  return hit;
}


// helper functions for findColor
inline float random_float(float min, float max) {
    // Returns a random real in [min,max).
    float random01 = rand() / (RAND_MAX + 1.0f);
    return min + (max-min) * random01;
}
glm::vec3 random_vec(float min, float max) {
  // Returns a random vector.
  return glm::vec3(random_float(min,max), random_float(min,max), random_float(min,max));
}
glm::vec3 random_in_unit_sphere() {
    while (true) {
        glm::vec3 p = random_vec(-1.0f,1.0f);
        if (glm::length(p) >= 1) continue;
        return p;
    }
}

// given intersection, find the color from all light sources in scene
glm::vec3 Scene::findColor(Intersection* intersection, int depth){
  // reach the end of the recursion
  if (depth <= 0) {
    return glm::vec3(0,0,0);
  }

  // if not hit, generate background color
  if (intersection->onHit == false) {
    float t = 0.5f * (intersection->dir.y + 1.0f);
    glm::vec3 color = (1.0f-t)*glm::vec3(1.0f, 1.0f, 1.0f) + t*glm::vec3(0.5f, 0.7f, 1.0f);
    return color;
  }

  // color on object hit
  glm::vec3 target = intersection->pos + intersection->normal + random_in_unit_sphere();
  Ray* ray2 = new Ray(intersection->pos, target - intersection->pos);
  Intersection intersection2 = this->getIntersection(ray2, 0.001f, std::numeric_limits<double>::infinity());
  return 0.5f * this->findColor(&intersection2, depth-1);

  glm::vec3 color = intersection->material->emision;

  //iterate through all light sources
  for (Light* light : lights){
    //find the shadow ray -- ray from intersection point to light
    glm::vec3 ori = intersection->pos;
    glm::vec3 dir = glm::normalize(light->position - ori);
    Ray * shadowRay = new Ray(ori, dir);

    //decide if shadow ray intersects with other objects in scene
    Intersection shadowInt = getIntersection(shadowRay, 0, 0);

    //if yes, skip this light source
    // in shadow
    if ((shadowInt.normal != glm::vec3(0.0f, 0.0f, 0.0f) || shadowInt.frontOnly)) { continue; }

    //otherwise, apply the shading formula and add to color
    else{
      //implement the formula
      glm::vec3 lj = glm::normalize(intersection->pos - light->position);
      glm::vec3 n = glm::normalize(intersection->normal);
      glm::vec3 hj = glm::normalize(lj + glm::normalize(intersection->dir));

      glm::vec3 C = intersection->material->ambient;
      C += intersection->material->diffuse * ((0<dot(n, lj))?dot(n, lj):0);
      if (dot(n, hj) > 0) {
          C += intersection->material->specular * pow(dot(n, hj), intersection->material->shininess);
      }
      
      color += C * light->color;

      // glm::vec3 target = intersection->pos + intersection->normal + random_in_unit_sphere();
      // Ray* ray2 = new Ray(intersection->pos, target - intersection->pos);
      // Intersection intersection2 = this->getIntersection(ray2, 0, 0);
      // color += this->findColor(&intersection2, depth-1);
    }
  }

  // return color;
}

// Color FindColor( hit ){
// Generate secondary rays to all lights
//   color = Visible? ShadingModel : 0;
//   ray2 = Generate mirror-reflected ray 
//   hit2 = Intersect( ray2, scene );
//   color += FindColor( hit2 );
// return color 
// }

#endif