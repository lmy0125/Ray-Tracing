#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "glm-0.9.7.1/glm/glm.hpp"

// Material of the object
struct Material {
  // glm::vec3 ambient = glm::vec3(0.0f, 0.0f, 0.0f);
  // glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
  // glm::vec3 specular = glm::vec3(0.0f, 0.0f, 0.0f);
  // glm::vec3 emision = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 ambient = glm::vec3(0.5f, 0.5f, 0.5f);
  glm::vec3 diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 specular = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 emision = glm::vec3(0.5f, 0.5f, 0.5f);
  float shininess = 10.0f;
};

#endif