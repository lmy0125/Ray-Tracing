#include <iostream>
#include "glm-0.9.7.1/glm/glm.hpp"

#include "Ray.h"
#include "Sphere.h"
#include "Camera.cpp"
#include "Scene.h"
#include "helper.cpp"

void write_color(std::ostream &out, glm::vec3 pixel_color, int sample_per_pixel) {
    // Write the translated [0,255] value of each color component.
    float r = pixel_color.x;
    float g = pixel_color.y;
    float b = pixel_color.z;

    out << static_cast<int>(255.999 * pixel_color.x) << ' '
        << static_cast<int>(255.999 * pixel_color.y) << ' '
        << static_cast<int>(255.999 * pixel_color.z) << '\n';
}

// shade the light color seen by the in-coming ray
glm::vec3 findColor(Intersection hit) {
  float t = hit.t;
  glm::vec3 normal = hit.normal;

  glm::vec3 pixelColor;
  if (normal != glm::vec3(0,0,0)) {
    pixelColor = 0.5f * (normal + glm::vec3(1.0f,1.0f,1.0f));
  }
  else {
    float t = 0.5f * (hit.dir.y + 1.0f);
    pixelColor = (1.0f-t)*glm::vec3(1.0f, 1.0f, 1.0f) + t*glm::vec3(0.5f, 0.7f, 1.0f);
  }

  return pixelColor;
}

glm::vec3 ray_color(Ray& r) {
    glm::vec3 unit_direction = glm::normalize(r.dir);
    float t = 0.5f * (unit_direction.y + 1.0f);
    return (1.0f-t)*glm::vec3(1.0, 1.0, 1.0) + t*glm::vec3(0.5, 0.7, 1.0);
}

// generates a ray originated from the camera position, through the center of the (i,j) pixel into the world
Ray* rayThruPixel(Camera* cam, int i, int j, int width, int height) {
  glm::vec3 w = glm::normalize(cam->eye - cam->target);
  glm::vec3 u = glm::normalize(glm::cross(cam->up, w));
  glm::vec3 v = glm::cross(w, u);
  v = glm::normalize(v);

  float x = 2 * (i + 0.5f) / width - 1;
  float y = 2 * (j + 0.5f) / height - 1;

  glm::vec3 pos = cam->eye;
  glm::vec3 dir = glm::normalize(x * u +  y * v / cam->aspect - w);
  // glm::vec3 dir = glm::normalize(glm::vec3(x,y/ cam->aspect,-1.0f) - pos); 

  Ray* ray = new Ray(pos, dir);

  return ray;
}

int main() {

  // Image
  const float aspect_ratio = 16.0f / 9.0f;
  const int image_width = 400;
  const int image_height = static_cast<int>(image_width / aspect_ratio);
  // for antialiasing
  const int sample_per_pixel = 100;

  // Camera
  Camera* camera = new Camera;
  camera->eye_default = glm::vec3(0.0f, 0.0f, 0.0f);
  camera->target_default = glm::vec3(0.0f, 0.0f, -1.0f);
  camera->aspect_default = aspect_ratio;
  camera->reset();

  // Scene
  Scene world;

  Material* material1 = new Material;
  Material* material2 = new Material;
  material2 -> ambient = glm::vec3(1.0f, 0.0f, 0.0f);

  Sphere* sphere1 = new Sphere(glm::vec3(0.0f, 0.0f, -1.0f), 0.5f, material2);
  world.add(sphere1);
  Sphere* sphere2 = new Sphere(glm::vec3(0.0f, -100.5f, -1.0f), 100.0f, material2);
  world.add(sphere2);

  // Render
  std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

  float viewport_height = 2.0f;
  float viewport_width = aspect_ratio * viewport_height;
  float focal_length = 1.0f;

  glm::vec3 horizontal = glm::vec3(viewport_width, 0, 0);
  glm::vec3 vertical = glm::vec3(0, viewport_height, 0);
  glm::vec3 lower_left_corner = camera->eye - horizontal/2.0f - vertical/2.0f - glm::vec3(0, 0, focal_length);

  // for each pixel in the scene
  for (int j = image_height-1; j >= 0; --j) {
    std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
      for (int i = 0; i < image_width; ++i) {
        // antialiasing, average color for each pixel
        glm::vec3 pixel_color;
        // for (int k = 0; k < sample_per_pixel; k++) {
          float u = float(i) / (image_width-1);
          float v = float(j) / (image_height-1);
          // Ray* myRay = rayThruPixel(camera, i, j, image_width, image_height);
          Ray* myRay = new Ray(camera->eye, lower_left_corner + u*horizontal + v*vertical);

          Intersection hitPoint = world.getIntersection(myRay, 0.0, 0.0);

          pixel_color = findColor(hitPoint);
        // }


        write_color(std::cout, pixel_color, sample_per_pixel);
      }
  }
  std::cerr << "\nDone.\n";
}

// sudo code in slide
// Image Raytrace(Camera cam, Scene scene, int width, int height){
//   Image image = new Image(width,height);
//   for (int j=0; j<height; j++)
//     for (int i=0; i<width; i++){
//       Ray ray = RayThruPixel( cam, i, j );
//       Intersection hit = Intersect( ray, scene );
//       image[i][j] = FindColor( hit );
//   }
//   return image;
// }