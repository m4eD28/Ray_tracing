#ifndef FIGURE_H
#define FIGURE_H
#include <cmath>
#include <memory>
#include "vec3.h"
#include "ray.h"
#include "hit.h"
#include "material.h"
#include "light.h"


class Figure {
  public:
    std::shared_ptr<Material> material;
    std::shared_ptr<Light> light;
    Figure() {};
    Figure(const std::shared_ptr<Material> _material, const std::shared_ptr<Light> _light) : material(_material), light(_light){};
    virtual bool intersect(const Ray& ray, Hit& hit) const = 0;
};

class Sphere : public Figure {
  public:
    Vec3 center;
    double radius;

    Sphere(const Vec3& _center, double _radius, const std::shared_ptr<Material>& _material, const std::shared_ptr<Light>& _light) : center(_center), radius(_radius), Figure(_material, _light) {};

    virtual bool intersect(const Ray& ray, Hit& res) const {
      double b = dot(ray.direction, ray.origin - center);
      double c = (ray.origin - center).length2() - radius*radius;
      double D = b*b - c;

      double t1 = -b - std::sqrt(D);
      double t2 = -b + std::sqrt(D);

      if(t1 > 10000 | t2 < ray.tmin) return false;
      double t = t1;
      if(t < ray.tmin) {
        t = t2;
        if(t2 > 10000) return false;
      }

      res.t = t;
      res.hitPos = ray(t);
      res.hitNormal = normalize(res.hitPos - center);
      res.hitShape = this;

      return true;
    }
};

class Plane : public Figure {
  public:
    Vec3 center;
    Vec3 normal;

    Plane(const Vec3& _center, const Vec3& _normal, const std::shared_ptr<Material> _material, const std::shared_ptr<Light>& _light) : center(_center), Figure(_material, _light) {};
    virtual bool intersect(const Ray& ray, Hit& res ) const {
      double t;
      double denominator = dot(normal, ray.direction);
      if (fabs(denominator) > 1e-6) {
        Vec3 defference = center - ray.origin;
        t = dot(defference, normal) / denominator;
        if(t >= 0) {
          res.t = t;
          res.hitPos = ray(t);
          res.hitNormal = normalize(res.hitPos - center);
          res.hitShape = this;

          return true;
        }
      }

      return false;

    }
};

class Disk : public Figure {
  public:
    Vec3 center;
    Vec3 normal;
    double radius;

    Disk(const Vec3& _center, const Vec3& _normal, double _radius, const std::shared_ptr<Material> _material, const std::shared_ptr<Light> _light) : center(_center), normal(_normal), radius(_radius), Figure(_material, _light) {};
    double t = 0;
};

class Triangle {
  public:

};

class Box {
  public:

};
#endif
