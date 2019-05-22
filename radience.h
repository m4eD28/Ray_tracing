#ifndef RADIENCE_H
#define RADIENCE_H
#include "sky.h"
#include "aggregate.h"
const int MAX_DEPTH = 500;
const double ROULETTE = 0.9;

Vec3 radience(const Ray& init_ray, const Aggregate& aggregate, const Sky& sky) {
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

      auto hitMaterial = res.hitShape->material;
      auto hitLight = res.hitShape->light;

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
      col += throughput*sky.getRadience(ray);
      break;
    }

    if(rnd() >= ROULETTE) break;
    else throughput /= ROULETTE;
  }
  return col;
}

#endif
