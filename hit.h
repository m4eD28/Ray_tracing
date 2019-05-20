#ifndef HIT_H
#define HIT_H
#include "vec3.h"

class Figure;

class Hit {
  public:
    double t;
    Vec3 hitPos;
    Vec3 hitNormal;
    const Figure* hitShape;

    Hit() {
      t = 10000;
    };
};
#endif
