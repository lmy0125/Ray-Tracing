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
    glm::vec3 lightIntersectColor (Light* light, Intersection* intersection, const int sample_per_light);
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

glm::vec3 random_in_unit_sphere() {
    while (true) {
        float min = -1.0f;
        float max = 1.0f;
        glm::vec3 p = glm::vec3(random_float(min,max), random_float(min,max), random_float(min,max));
        if (glm::length(p) >= 1) continue;
        return p;
    }
}

// get the color of a light source hitting an intersection
glm::vec3 Scene::lightIntersectColor (Light* light, Intersection* intersection, const int sample_per_light){
  // initialize color from this light source
  glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f);

    //soft shadow
    //generate random samples in light area and average
    for (int sample = 0; sample < sample_per_light; sample++){

      // generate random point light in area light
      glm::vec3 lightPos = light->position;
      float x_adjust = random_float(-light->x_pad, light->x_pad);
      float y_adjust = random_float(-light->y_pad, light->y_pad);
      lightPos = lightPos + glm::vec3(x_adjust, y_adjust, 0);

      //find the shadow ray -- ray from intersection point to light
      glm::vec3 ori = intersection->pos;
      glm::vec3 dir = glm::normalize(lightPos - ori);
      Ray * shadowRay = new Ray(ori, dir);

      //decide if shadow ray intersects with other objects in scene
      Intersection shadowInt = getIntersection(shadowRay, 0, 0);

      delete shadowRay;

      //if not in shadow, compute color from light
      if (!shadowInt.onHit && !shadowInt.frontOnly) { 
        glm::vec3 lj = glm::normalize(-intersection->pos + lightPos);
        glm::vec3 n = glm::normalize(intersection->normal);
        glm::vec3 hj = glm::normalize(lj + glm::normalize(intersection->dir));

        glm::vec3 C = intersection->material->ambient;

        C += intersection->material->diffuse * ((0<dot(n, lj))?dot(n, lj):0);

        if (dot(n, hj) > 0) {
          C += intersection->material->specular * pow(dot(n, hj), intersection->material->shininess);
        }
        
        color += C * light->color;
      }
    }

  //average
  return (1/float(sample_per_light)) * color;
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
    // glm::vec3 color = (1.0f-t)*glm::vec3(1.0f, 1.0f, 1.0f) + t*glm::vec3(0.5f, 0.7f, 1.0f);
    glm::vec3 color = (1.0f-t)*glm::vec3(0.2f, 0.2, 0.2f);
    return color;
  }

  glm::vec3 color = intersection->material->emision;

  const int sample_per_light = 20;

  // get color from all light sources
  for (Light* light : lights){
    color += lightIntersectColor(light, intersection, sample_per_light);
  }

  // recursive color
  glm::vec3 target = intersection->pos + intersection->normal + random_in_unit_sphere();
  Ray* ray2 = new Ray(intersection->pos, target - intersection->pos);
  Intersection intersection2 = this->getIntersection(ray2, 0, 0);
  color += 0.5f * this->findColor(&intersection2, depth-1);
  delete ray2;

  return color;
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