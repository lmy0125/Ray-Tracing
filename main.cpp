#include <iostream>
#include "glm-0.9.7.1/glm/glm.hpp"

#include "Ray.h"
#include "Camera.cpp"

void write_color(std::ostream &out, glm::vec3 pixel_color) {
    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(255.999 * pixel_color.x) << ' '
        << static_cast<int>(255.999 * pixel_color.y) << ' '
        << static_cast<int>(255.999 * pixel_color.z) << '\n';
}

glm::vec3 ray_color(Ray& r) {
    glm::vec3 unit_direction = glm::normalize(r.direction());
    float t = 0.5f * (unit_direction.y + 1.0f);
    return (1.0f-t)*glm::vec3(1.0, 1.0, 1.0) + t*glm::vec3(0.5, 0.7, 1.0);
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

int main() {

  // Image
  const float aspect_ratio = 16.0f / 9.0f;
  const int image_width = 400;
  const int image_height = static_cast<int>(image_width / aspect_ratio);

  // Camera
  Camera* camera = new Camera;
  camera->eye_default = glm::vec3(0.0f, 0.0f, 0.0f);
  camera->aspect_default = aspect_ratio;
  camera->reset();

  float viewport_height = 2.0f;
  float viewport_width = aspect_ratio * viewport_height;
  float focal_length = 1.0f;
  glm::vec3 horizontal = glm::vec3(viewport_width, 0, 0);
  glm::vec3 vertical = glm::vec3(0, viewport_height, 0);
  glm::vec3 lower_left_corner = camera->eye - horizontal/2.0f - vertical/2.0f - glm::vec3(0, 0, focal_length);

  // Scene

  // Render
  std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

  // for each pixel in the scene
  for (int j = image_height-1; j >= 0; --j) {
    std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
      for (int i = 0; i < image_width; ++i) {
        float u = float(i) / (image_width-1);
        float v = float(j) / (image_height-1);
        Ray r(camera->eye, (lower_left_corner + u*horizontal + v*vertical - camera->eye));
        glm::vec3 pixel_color = ray_color(r);
        // glm::vec3 pixel_color = glm::vec3 (double(i)/(image_width-1), double(j)/(image_height-1), 0.25);
        write_color(std::cout, pixel_color);
      }
  }
  std::cerr << "\nDone.\n";
}