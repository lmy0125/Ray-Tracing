#ifndef SCENE_H
#define SCENE_H

#include "Intersection.h"

#include <vector>
#include <limits>

struct Light {
    glm::vec3 position = glm::vec3(0.0f, 70.0f, -75.0f);
    glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
};

class Scene: public Object {
  public:
    std::vector<Object*> objects;
    std::vector<Light*> lights;

    Scene() {}
    Scene(Object* object, Light* light) { 
      objects.push_back(object); 
      lights.push_back(light);
    }

    void add(Object* object) { objects.push_back(object); };
    void clear() { objects.clear(); }

    virtual Intersection getIntersection(Ray* ray, float t_min, float t_max) override;
    glm::vec3 findColor(Intersection* intersection);
};

// searches over all geometries in the scene and returns the closest hit
Intersection Scene::getIntersection(Ray* ray, float t_min, float t_max) {
  float minDist = std::numeric_limits<double>::infinity();
  Intersection hit;
  for (Object* object : objects) {
    // std::cerr << object.t;
    Intersection hit_temp = object->getIntersection(ray, 0.0, 0.0);
    if (hit_temp.t < minDist) {
      minDist = hit_temp.t;
      hit = hit_temp;
    }
  }
  return hit;
}


// given intersection, find the color from all light sources in scene
glm::vec3 Scene::findColor(Intersection* intersection){

  glm::vec3 color = intersection->material->emision;

  //iterate through all light sources
  for (Light* light : lights){
    //find the shadow ray -- ray from intersection point to light
    glm::vec3 ori = intersection->pos;
    glm::vec3 dir = glm::normalize(ori - light->position);
    Ray * shadowRay = new Ray(ori, dir);

    //decide if shadow ray intersects with other objects in scene
    Intersection shadowInt = getIntersection(shadowRay, 0, 0);

    //if yes, skip this light source
    // in shadow
    if (shadowInt.normal != glm::vec3(0.0f, 0.0f, 0.0f)){
      continue;
    }

    //otherwise, apply the shading formula and add to color
    else{
      //implement the formula
      
    }
  }

  return color;

}


#endif

// sudo code in lecture
// Intersection Intersect(Ray ray, Scene scene){
//   Distance mindist = INFINITY; 
//   Intersection hit;
//   foreach (object in scene){ // Find closest intersection; test all objects
//     Intersection hit_temp = Intersect(ray, object);
//     if (hit_temp.dist< mindist){ // closer than previous hit
//     mindist = hit_temp.dist;
//     hit = hit_temp;
//      }
//     }
//  return hit;
// }