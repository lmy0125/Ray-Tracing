#include <iostream>
#include "glm-0.9.7.1/glm/glm.hpp"
#include "glm-0.9.7.1/glm/gtx/string_cast.hpp"
#include <numeric>

#include "Ray.h"
#include "Sphere.h"
#include "Camera.cpp"
#include "Scene.h"
#include "helper.cpp"

inline float clamp(float x, float min, float max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

void write_color(std::ostream &out, glm::vec3 pixel_color) {
    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(256 * clamp(pixel_color.x, 0.0, 0.999)) << ' '
      << static_cast<int>(256 * clamp(pixel_color.y, 0.0, 0.999)) << ' '
      << static_cast<int>(256 * clamp(pixel_color.z, 0.0, 0.999)) << '\n';
        // out << static_cast<int>(255.999 * pixel_color.x) << ' '
        // << static_cast<int>(255.999 * pixel_color.y) << ' '
        // << static_cast<int>(255.999 * pixel_color.z) << '\n';
  }

// shade the light color seen by the in-coming ray
glm::vec3 findColor(Intersection hit) {
  // float t = hit.t;
  glm::vec3 normal = hit.normal;

  glm::vec3 pixelColor;
  if (normal != glm::vec3(0.0f,0.0f,0.0f)) {
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
std::vector<Ray*> rayThruPixel(Camera* cam, int i, int j, int width, int height, int sample_per_pixel) {
  std::vector<Ray*> rays;
  glm::vec3 w = glm::normalize(cam->eye - cam->target);
  glm::vec3 u = glm::normalize(glm::cross(cam->up, w));
  glm::vec3 v = glm::cross(w, u);
  v = glm::normalize(v);

  for (int k = 0; k < sample_per_pixel; k++) {
    float m = rand() / (RAND_MAX + 1.0f);
    float n = rand() / (RAND_MAX + 1.0f);
    float x = 2 * (i + m) / width - 1;
    float y = 2 * (j + n) / height - 1;
    glm::vec3 pos = cam->eye;
    glm::vec3 dir = glm::normalize(x * u +  y * v / cam->aspect - w); 

    Ray* ray = new Ray(pos, dir);
    rays.push_back(ray);
  }

  return rays;
}

int main() {

  // Image
  const float aspect_ratio = 16.0f / 9.0f;
  const int image_width = 400;
  const int image_height = static_cast<int>(image_width / aspect_ratio);
  // for antialiasing
  const int sample_per_pixel = 10;
  // for recursive ray tracing
  const int recur_depth = 10;

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
  material2 -> emision = glm::vec3(0.0f, 0.0f, 0.0f);

  Sphere* sphere1 = new Sphere(glm::vec3(0.0f, 0.0f, -2.0f), 0.5f, material1);
  Sphere* sphere2 = new Sphere(glm::vec3(0.0f, -100.5f, -3.0f), 100.0f, material2);
  world.add_obj(sphere2);
  world.add_obj(sphere1);

  Light* light1 = new Light;
  light1->color = glm::vec3(0.5f, 0.7f, 1.0f);
  light1->position = glm::vec3(-0.5f, 1.2f, -2.0f);
  world.add_light(light1);
  // Light* light2 = new Light;
  // light2->position = glm::vec3(0.5f, 1.2f, -2.0f);
  // world.add_light(light2);


  // Render
  std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

  // for each pixel in the scene
  for (int j = image_height-1; j >= 0; --j) {
    std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
      for (int i = 0; i < image_width; ++i) {
        // antialiasing, average color for each pixel
        glm::vec3 pixel_color;
        std::vector<glm::vec3> ray_color;
        std::vector<Ray*> myRay = rayThruPixel(camera, i, j, image_width, image_height, sample_per_pixel);
        //  myRay = new Ray(camera->eye, lower_left_corner + u*horizontal + v*vertical);
        for (Ray* ray : myRay) {
          Intersection hitPoint = world.getIntersection(ray, 0.001f, std::numeric_limits<double>::infinity());
          // ray_color.push_back(findColor(hitPoint));

          //set color using FindColor
          ray_color.push_back(world.findColor(&hitPoint, recur_depth));
        }
        pixel_color = std::accumulate(ray_color.begin(), ray_color.end(), decltype(ray_color)::value_type(0));
        pixel_color = pixel_color * (1.0f / sample_per_pixel) ;

        // std::cerr << pixel_color.x << " "<< pixel_color.y << " " << pixel_color.z << "\n";
        write_color(std::cout, pixel_color);
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