#include <memory>
#include <random>
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

const int MAX_DEPTH = 500;
const double ROULETTE = 0.9;

Vec3 radiance(const Ray& init_ray, const Aggregate& aggregate, const Sky& sky) {
  Vec3 col;
  Vec3 throughput(1);
  Ray ray = init_ray;

  for (int depth = 0; depth < MAX_DEPTH; depth++) {
    Hit res;
    if(aggregate.intersect(ray, res)) {
      Vec3 n = res.hitNormal;
      Vec3 s, t;
      orthonormalBasis(n, s, t);
      Vec3 wo_local = worldToLocal(-ray.direction, s, n, t);

      auto hitMaterial = res.hitSphere->material;
      auto hitLight = res.hitSphere->light;

      col += throughput*hitLight->Le();

      Vec3 brdf;
      Vec3 wi_local;
      double pdf;
      brdf = hitMaterial->sample(wo_local, wi_local, pdf);

      double cos = absCosTheta(wi_local);

      Vec3 wi = localToWorld(wi_local, s, n, t);

      throughput *= brdf*cos/pdf;

      ray = Ray(res.hitPos, wi);
    }

    else {
      col += throughput*sky.getRadiance(ray);
      break;
    }

    if(rnd() >= ROULETTE) break;
    else throughput /= ROULETTE;
  }
  return col;
}

int main() {
  const int N = 100;

  Image img(512, 512);
  ThinLensCamera cam(Vec3(0, 0, 1), Vec3(0, 0, -1), Vec3(0, 0, -3), 1, 0.1);

  auto mat1 = std::make_shared<Diffuse>(Vec3(0.9));
  auto mat2 = std::make_shared<Glass>(1.5);
  auto mat3 = std::make_shared<Mirror>();
  auto mat4 = std::make_shared<Diffuse>(Vec3(0.2, 0.2, 0.8));

  auto light1 = std::make_shared<Light>(Vec3(0));

  Aggregate aggregate;
  aggregate.add(std::make_shared<Sphere>(Vec3(0, -10001, 0), 10000, mat1, light1));
  aggregate.add(std::make_shared<Sphere>(Vec3(0, 1+std::sqrt(2)/2, -3), 1, mat4 ,light1));
  aggregate.add(std::make_shared<Sphere>(Vec3(1, 0, -3), 1, mat2, light1));
  aggregate.add(std::make_shared<Sphere>(Vec3(-1, 0, -3), 1, mat3, light1));

  IBL sky("PaperMill_E_3k.hdr");

#pragma omp parallel for schedule(dynamic, 1)
  for (int i = 0; i < img.width; i++) {
    for (int j = 0; j < img.height; j++) {
      for (int k = 0; k < N; k++) {
        double u = (2.0*(i + rnd()) - img.width)/img.width;
        double v = (2.0*(j + rnd()) - img.height)/img.height;

        Ray ray = cam.getRay(-u, -v);

        Vec3 col = radiance(ray, aggregate, sky);

        img.addPixel(i, j, col);
      }

      if (omp_get_thread_num() == 0) {
        std::cout << double(j + i*img.height)/(img.width*img.height) * 100 << "\r" << std::flush;
      }
    }
  }

  img.divide(N);

  img.gammma_correction();

  img.ppm_output("far.ppm");

  return 0;
}
