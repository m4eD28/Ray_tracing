#ifndef MATERIAL_H
#define MATERIAL_H
#include <cmath>
#include "vec3.h"
#include "random.h"
#include "ray.h"
#include "hit.h"

double cosTheta(const Vec3& v) {
  return v.y;
}

double absCosTheta(const Vec3& v) {
  return std::abs(v.y);
}

double fresnel(const Vec3& v, const Vec3& n, double n1, double n2) {
  double f0 = std::pow((n1 - n2)/(n1 + n2), 2.0);
  double cos = absCosTheta(v);
  return f0 + (1 - f0)*std::pow(1 - cos, 5.0);
}

bool refract(const Vec3& v, Vec3& r, const Vec3& n, double n1, double n2) {
  double cos = absCosTheta(v);
  double sin = std::sqrt(std::max(1 - cos*cos, 0.0));
  double alpha = n1/n2 * sin;

  if (alpha*alpha > 1.0) return false;

  r = n1/n2 * (-v + dot(v, n)*n) - std::sqrt(1 - alpha*alpha)*n;

  return true;
}

Vec3 reflect(const Vec3& v, const Vec3& n) {
  return -v + 2*dot(v, n) * n;
}

class Material {
  public:
    virtual Vec3 sample(const Vec3& wo, Vec3& wi, double& pdf) const = 0;
    /* virtual bool scatter(const Ray& ray, const Hit& hit, Vec3& attenuation, Ray& scatterd) const = 0; */
};

class Diffuse : public Material {
  public:
    Vec3 rho;

    Diffuse(const Vec3& _rho) : rho(_rho) {};

    Vec3 sample(const Vec3& wo, Vec3& wi, double& pdf) const {
      double u = rnd();
      double v = rnd();
      double theta = 0.5 * std::acos(1 - 2*u);
      double phi = 2 * M_PI * v;

      double x = std::cos(phi)*std::sin(theta);
      double y = std::cos(theta);
      double z = std::sin(phi)*std::sin(theta);

      wi = Vec3(x, y, z);

      pdf = std::cos(theta)/M_PI;

      return rho/M_PI;
    };
};

class Mirror : public Material {
  public:
    Mirror() {};

    Vec3 sample(const Vec3& wo, Vec3& wi, double& pdf) const {
      wi = reflect(wo, Vec3(0, 1, 0));

      pdf = 1.0;
      return 1/cosTheta(wi) * Vec3(1);
    };
};

class Glass : public Material {
  public:
    double ior;

    Glass(double _ior) : ior(_ior) {};

    Vec3 sample(const Vec3& wo, Vec3& wi, double& pdf) const {
      bool isEntering = cosTheta(wo) > 0;

      double n1, n2;
      Vec3 normal;
      if(isEntering) {
        n1 = 1.0;
        n2 = ior;
        normal = Vec3(0, 1, 0);
      }
      else {
        n1 = ior;
        n2 = 1.0;
        normal = Vec3(0, -1, 0);
      }

      double fr = fresnel(wo, normal, n1, n2);

      if(rnd() < fr) {
        wi = reflect(wo, normal);
        pdf = fr;
        return fr/absCosTheta(wi) * Vec3(1);
      }
      else {
        if (refract(wo, wi, normal, n1, n2)) {
          pdf = 1 - fr;
          return std::pow(n1/n2, 2.0) * (1 - fr)/absCosTheta(wi) * Vec3(1);
        }
        else {
          wi = reflect(wo, normal);
          pdf = 1 - fr;
          return (1 - fr)/absCosTheta(wi) * Vec3(1);
        }
      }
  };
};

/* class lambertian : puclic material { */
/*   public: */
/*     lambertian(const Vec3& a) : albedo(a) {}; */
/*     virtual bool scatter(const Ray& ray, const Hit& hit, Vec3& attenuation, Ray& scattered) const { */

/*     } */
/*     Vec3 albedo; */
/* }; */
#endif
