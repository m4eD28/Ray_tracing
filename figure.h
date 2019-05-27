#ifndef FIGURE_H
#define FIGURE_H
#include <cmath>
#include <memory>
#include <iostream>
#include "vec3.h"
#include "ray.h"
#include "hit.h"
#include "material.h"
#include "light.h"

const double kEpsilon = 1e-6;

class Figure {
  public:
    std::shared_ptr<Material> material;
    std::shared_ptr<Light> light;
    double bbox[2][3];
    Figure() {};
    Figure(const std::shared_ptr<Material> _material, const std::shared_ptr<Light> _light) : material(_material), light(_light){};
    virtual bool intersect(const Ray& ray, Hit& hit) const = 0;
};

class Sphere : public Figure {
  public:
    Vec3 center;
    double radius;

    Sphere(const Vec3& _center, double _radius, const std::shared_ptr<Material>& _material, const std::shared_ptr<Light>& _light) : center(_center), radius(_radius), Figure(_material, _light) {
      bbox[0][0] = center.x - radius;
      bbox[1][0] = center.x + radius;
      bbox[0][1] = center.y - radius;
      bbox[1][1] = center.y + radius;
      bbox[0][2] = center.z - radius;
      bbox[1][2] = center.z + radius;
    };

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

    Plane(const Vec3& _center, const Vec3& _normal, const std::shared_ptr<Material> _material, const std::shared_ptr<Light>& _light) : center(_center), normal(_normal), Figure(_material, _light) {};
    virtual bool intersect(const Ray& ray, Hit& res ) const {
      double t;
      double denominator = dot(normal, ray.direction);
      if (fabs(denominator) < kEpsilon) return false;

      Vec3 difference = center - ray.origin;
      t = dot(difference, normal) / denominator;

      if (t <= 0) return false;

      res.t = t;
      res.hitPos = ray(t);
      res.hitNormal = normal;
      res.hitShape = this;

      return true;
    }
};

class Disk : public Figure {
  public:
    Vec3 center;
    Vec3 normal;
    double radius;

    Disk(const Vec3& _center, const Vec3& _normal, double _radius, const std::shared_ptr<Material> _material, const std::shared_ptr<Light> _light) : center(_center), normal(_normal), radius(_radius), Figure(_material, _light) {};
    double t = 0;
    virtual bool intersect(const Ray& ray, Hit& res ) const {
      double t;
      double denominator = dot(normal, ray.direction);
      if (fabs(denominator) < kEpsilon) return false;

      Vec3 difference = center - ray.origin;
      t = dot(difference, normal) / denominator;

      if (t <= 0) return false;

      Vec3 temp= ray.origin + t*ray.direction - center;
      double radius_val = temp.length();
      if (radius_val > radius) return false;
      res.t = t;
      res.hitPos = ray(t);
      res.hitNormal = normal;
      res.hitShape = this;

      return true;
    }
};

class Triangle: public Figure {
  public:
    Vec3 normal;
    Vec3 a;
    Vec3 b;
    Vec3 c;

    Triangle(const Vec3& _normal, const Vec3& _a, const Vec3& _b, const Vec3& _c, const std::shared_ptr<Material> _material, const std::shared_ptr<Light>& _light) : normal(_normal), a(_a), b(_b), c(_c), Figure(_material, _light) {
      bbox[1][0] = std::max(a.x, std::max(b.x, c.x));
      bbox[0][0] = std::min(a.x, std::min(b.x, c.x));
      bbox[1][1] = std::max(a.y, std::max(b.y, c.y));
      bbox[0][1] = std::min(a.y, std::min(b.y, c.y));
      bbox[1][2] = std::max(a.z, std::max(b.z, c.z));
      bbox[0][2] = std::min(a.z, std::min(b.z, c.z));
    };
    virtual bool intersect(const Ray& ray, Hit& res ) const {
      Vec3 e1 = b - a;
      Vec3 e2 = c - a;

      Vec3 alpha = cross(ray.direction, e2);
      double det = dot(e1, alpha);
      /* std::cout << "b1" << normal << a << b << c << std::endl; */
      if (-kEpsilon < det && det < kEpsilon) return false;
      /* std::cout << "b2" << normal << a << b << c << std::endl; */
      double invDet = 1.0 / det;
      Vec3 r = ray.origin - a;

      double u = dot(alpha, r) * invDet;
      if(u < 0 || u > 1.0) return false;
      /* std::cout << "b3" << normal << a << b << c << std::endl; */

      Vec3 beta = cross(r, e1);

      double v = dot(ray.direction, beta) * invDet;
      if (v < 0 || v + u > 1.0) return false;
      /* std::cout << "b4" << normal << a << b << c << std::endl; */

      double t = dot(e2, beta) * invDet;
      if (t < 0) return false;
      /* std::cout << "b5" << normal << a << b << c << std::endl; */

      res.t = t;
      res.hitPos = ray(t);
      res.hitNormal = normal;
      res.hitShape = this;

      return true;
    }
};


class Box {
  public:

};
#endif
