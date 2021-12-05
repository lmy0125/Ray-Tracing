#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "glm-0.9.7.1/glm/glm.hpp"

struct Light {
    glm::vec3 position = glm::vec3(0.0f, 1.0f, -1.0f);
    glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
};

#endif