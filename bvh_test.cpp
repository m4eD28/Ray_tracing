#include <memory>
#include <random>
#include <chrono>
#include <omp.h>
#include "vec3.h"
#include "ray.h"
#include "hit.h"
#include "figure.h"
#include "camera.h"
#include "image.h"
#include "aggregate.h"
#include "material.h"
#include "light.h"
#include "random.h"
#include "sky.h"
#include "radience.h"
#include "bvh.h"

int main() {
  std::chrono::system_clock::time_point start, end;
  start = std::chrono::system_clock::now();
  const int N = 10;

  Image img(1024, 512);
  double screen_height = 2.0;
  ThinLensCamera cam(Vec3(0, 0, 1), Vec3(0, 0, -1), Vec3(0, 0, -3), 1, 0.1);

  auto mat1 = std::make_shared<Diffuse>(Vec3(0.9));
  auto mat2 = std::make_shared<Glass>(1.5);
  auto mat3 = std::make_shared<Mirror>();
  auto mat4 = std::make_shared<Diffuse>(Vec3(0.2, 0.2, 0.8));
  auto mat5 = std::make_shared<Diffuse>(Vec3(0.2, 0.8, 0.2));
  auto mat6 = std::make_shared<Diffuse>(Vec3(0.8, 0.2, 0.2));

  auto light1 = std::make_shared<Light>(Vec3(0));

  Aggregate aggregate;
  aggregate.add(std::make_shared<Disk>(Vec3(0, -1, -4), Vec3(0, 1, 0), 3, mat5, light1));
  aggregate.add(std::make_shared<Plane>(Vec3(0, -1.1, 0), Vec3(0, 1, 0), mat4, light1));
  aggregate.add(std::make_shared<Sphere>(Vec3(0, 1+std::sqrt(2)/2, -3), 1, mat1 ,light1));
  aggregate.add(std::make_shared<Sphere>(Vec3(1, 0, -3), 1, mat2, light1));
  aggregate.add(std::make_shared<Sphere>(Vec3(-1, 0, -3), 1, mat3, light1));
  aggregate.add(std::make_shared<Triangle>(Vec3(0, 0, 1), Vec3(2, -1.001, -3), Vec3(4, 3, -3), Vec3(5, -1.089, -3), mat6, light1));

  //box
  aggregate.add(std::make_shared<Triangle>(Vec3(0, 0, 1), Vec3(-5, -1, -3), Vec3(-5, 3, -3), Vec3(-2.5, -1, -3), mat1, light1));
  aggregate.add(std::make_shared<Triangle>(Vec3(0, 0, 1), Vec3(-5, 3, -3), Vec3(-2.5, 3, -3), Vec3(-2.5, 3, -3), mat1, light1));
  /* aggregate.add(std::make_shared<Triangle>(Vec3(1, 0, 0), Vec3(), Vec3(), Vec3(), mat1, light1)); */
  /* aggregate.add(std::make_shared<Triangle>(Vec3(1, 0, 0), Vec3(), Vec3(), Vec3(), mat1, light1)); */
  /* aggregate.add(std::make_shared<Triangle>(Vec3(-1, 0, 0), Vec3(), Vec3(), Vec3(), mat1, light1)); */
  /* aggregate.add(std::make_shared<Triangle>(Vec3(-1, 0, 0), Vec3(), Vec3(), Vec3(), mat1, light1)); */
  /* aggregate.add(std::make_shared<Triangle>(Vec3(0, 1, 0), Vec3(), Vec3(), Vec3(), mat1, light1)); */
  /* aggregate.add(std::make_shared<Triangle>(Vec3(0, 1, 0), Vec3(), Vec3(), Vec3(), mat1, light1)); */

  constructBVH(aggregate.shapes);
  IBL sky("PaperMill_E_3k.hdr");

#pragma omp parallel for schedule(dynamic, 1)
  for (int i = 0; i < img.width; i++) {
    for (int j = 0; j < img.height; j++) {
      for (int k = 0; k < N; k++) {
        double u = screen_height*(2*i - img.width) / (2 * img.height) + screen_height*rnd()/img.height;
        double v = screen_height*(2*j - img.height)/(2*img.height) + screen_height*rnd()/img.height;

        Ray ray = cam.getRay(-u, -v);

        Vec3 col = radience(ray, aggregate, sky);

        img.addPixel(i, j, col);
      }

      if (omp_get_thread_num() == 0) {
        std::cout << double(j + i*img.height)/(img.width*img.height) * 100 << "\r" << std::flush;
      }
    }
  }

  img.divide(N);

  img.gammma_correction();

  img.ppm_output("bvh_test.ppm");

  end = std::chrono::system_clock::now();
  double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
  std:: cout << elapsed << std::endl;
  return 0;
}
