#ifndef TEXTURE_H
#define TEXTURE_H
#include "vec3.h"

class texture {
  public:
    virtual Vec3 value(double u, double v, const Vec3& p) const = 0;
};

class constant_texture : public texture {
  public:
    constant_texture(Vec3 _c) : color(_c) {};
    virtual Vec3 value(double u, double v, const Vec3& p) const {
      return color;
    }
    Vec3 color;
};
#endif
