#ifndef SKY_H
#define SKY_H
#include "vec3.h"
#include "ray.h"

class Sky {
  public:
    virtual Vec3 getRadiance(const Ray& ray) const = 0;
};

class UniformSky : public Sky {
  public:
    Vec3 color;

    UniformSky(const Vec3& _color) : color(_color) {};

    Vec3 getRadiance(const Ray& ray) const {
      return color;
    };
};

class SimpleSky : public Sky {
  public:
    SimpleSky() {};

    Vec3 getRadiance(const Ray& ray) const {
      double t = (ray.direction.y + 1)/2;
      return (1 - t)*Vec3(1) + t*Vec3(0.7, 0.8, 1);
    };
};
#endif
