#include <iostream>
#include "glm-0.9.7.1/glm/glm.hpp"

#include "Ray.h"
#include "Sphere.h"
#include "Camera.cpp"
#include "Scene.h"

void write_color(std::ostream &out, glm::vec3 pixel_color) {
    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(255.999 * pixel_color.x) << ' '
        << static_cast<int>(255.999 * pixel_color.y) << ' '
        << static_cast<int>(255.999 * pixel_color.z) << '\n';
}

// shade the light color seen by the in-coming ray
// glm::vec3 FindColor(Intersection hit) {}
glm::vec3 ray_color(Ray& r) {
    glm::vec3 unit_direction = glm::normalize(r.direction());
    float t = 0.5f * (unit_direction.y + 1.0f);
    return (1.0f-t)*glm::vec3(1.0, 1.0, 1.0) + t*glm::vec3(0.5, 0.7, 1.0);
}

// generates a ray originated from the camera position, through the center of the (i,j) pixel into the world
Ray* rayThruPixel(Camera* cam, int i, int j, int width, int height) {
  glm::vec3 w = glm::normalize(cam->eye - cam->target);
  glm::vec3 u = glm::normalize(glm::cross(cam->up, w));
  glm::vec3 v = glm::cross(w, u);
  v = glm::normalize(v);

  float x = 2 * (i + 0.5) / width - 1;
  float y = 1 - 2 * (j + 0.5) / height;

  glm::vec3 pos = cam->eye;
  glm::vec3 dir = glm::normalize(x * u +  y * v / cam->aspect - w);

  Ray* ray = new Ray(pos, dir);

  return ray;
}

// Checks whether a ray would interset with a triangle in front of camera
bool IntersectTriag(Ray* ray, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3){
  // TODO: there is still something wrong with the view/transformation

  glm::mat4 A = glm::mat4(p1[0], p1[1], p1[2], 1, 
                          p2[0], p2[1], p2[2], 1, 
                          p3[0], p3[1], p3[2], 1,
                          -ray->dir[0], -ray->dir[1], -ray->dir[2], 0);
  glm::vec4 b = glm::vec4(ray->ori, 1);

  glm::vec4 x = glm::inverse(A) * b;

  return (x[0] >= 0 &&  x[1] >= 0 && x[2] >= 0 && x[3] >= 0);
}

int main() {

  // Image
  const float aspect_ratio = 16.0f / 9.0f;
  const int image_width = 400;
  const int image_height = static_cast<int>(image_width / aspect_ratio);

  // Camera
  Camera* camera = new Camera;
  camera->eye_default = glm::vec3(0.0f, 0.0f, 0.0f);
  camera->target_default = glm::vec3(0.0f, 0.0f, -1.0f);
  camera->aspect_default = aspect_ratio;
  camera->reset();

  // Scene
  Scene world;
  Sphere* sphere1 = new Sphere(glm::vec3(0.0f, 0.0f, -1.0f), 0.5f);
  world.add(sphere1);
  Sphere* sphere2 = new Sphere(glm::vec3(0.0f, -100.5f, -1.0f), 100.0f);
  world.add(sphere2);

  // Render
  std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

  // for each pixel in the scene
  for (int j = image_height-1; j >= 0; --j) {
    std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
      for (int i = 0; i < image_width; ++i) {
        float u = float(i) / (image_width-1);
        float v = float(j) / (image_height-1);

        //test rayThruPixel
        Ray* myRay = rayThruPixel(camera, i, j, image_width, image_height);
        glm::vec3 p1 = glm::vec3(0.0f, -88.88f, -100.0f);
        glm::vec3 p2 = glm::vec3(0.0f, 0.0f, -100.0f);
        glm::vec3 p3 = glm::vec3(50.0f, 0.0f, -100.0f);
        bool triag_int = IntersectTriag(myRay, p1, p2, p3);

        //  pixel_color = glm::vec3(0.0f, 0.0f, 0.25);
        // if(triag_int){
        //   pixel_color = glm::vec3(1.0f, 1.0f, 0.25);
        // } 

        Intersection hitPoint = world.getIntersection(myRay, 0.0, 0.0);

        glm::vec3 sphe_normal = hitPoint.normal;

        
      
        float t1 = 0.5*(sphe_normal.x + 1.0);
        float t2 = 0.5*(sphe_normal.y + 1.0);
        float t3 = 0.5*(sphe_normal.z + 1.0);
        glm::vec3 pixel_color = glm::vec3(1.0-t1, 1.0-t2, 1.0-t3)+ glm::vec3(0.5*t1, 0.7*t2, 1.0*t3);
      
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