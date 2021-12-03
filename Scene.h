#ifndef SCENE_H
#define SCENE_H

#include "Intersection.h"

#include <vector>
#include <limits>

class Scene: public Intersect {
  public:
    std::vector<Intersect*> objects;

    Scene() {}
    Scene(Intersect* object) { objects.push_back(object); }

    void add(Intersect* object) { objects.push_back(object); };
    void clear() { objects.clear(); }

    virtual Intersection getIntersection(Ray* ray, float t_min, float t_max) override;
};

// searches over all geometries in the scene and returns the closest hit
Intersection Scene::getIntersection(Ray* ray, float t_min, float t_max) {
  float minDist = std::numeric_limits<double>::infinity();
  Intersection hit;
  for (Intersect* object : objects) {
    // std::cerr << object.t;
    Intersection hit_temp = object->getIntersection(ray, 0.0, 0.0);
    if (hit_temp.t < minDist) {
      minDist = hit_temp.t;
      hit = hit_temp;
    }
  }
  return hit;
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