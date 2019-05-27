#ifndef RAY_H
#define RAY_H
#include <iostream>
#include "vec3.h"
#include "hit.h"
#include "bvh.h"

class Ray {
  public:
    Vec3 origin;
    Vec3 direction;
    double tmin = 0.001;
    double org[3];
    double dir[3];

    Ray(const Vec3& _origin, const Vec3& _direction)
      : origin(_origin) , direction(_direction) {
        org[0] = _origin.x;
        org[1] = _origin.y;
        org[2] = _origin.z;
        dir[0] = _direction.x;
        dir[1] = _direction.y;
        dir[2] = _direction.z;
      };

    Vec3 operator()(double t) const {
      return origin + t*direction;
    };
};

/* std::shared_ptr<Figure> Intersect(BVH_node *nodes, int index, const Ray &ray, Hit& hit) { */
/*   if (IntersectAABBvsRay(nodes[index].bbox, const Ray& ray)) { */

/*     if (nodes[index].children[0] != -1) { */
/*       std::shared_ptr<Figure> childResult = -1; */
/*       std::shared_ptr<Figure> result; */
/*       for (int i=0; i<2; i++) { */
/*         result = intersect(ray, hit, nodes[index].children[i], node); */
/*         if (result != nullptr) { */
/*           childResult = result; */
/*         } */
/*       } */
/*       if (childResult != nullptr) return childResult; */
/*     } else { */
/*       std::shared_ptr<Figure> result = nullptr; */
/*       for (auto p : node[index].polygons) { */
/*         if (p->Intersect(ray, distance, normal)) { */
/*           if (distance < info.distance) { */
/*             result = tri; */
/*             info.distance = distance; */
/*             for (int i=0; i<3; i++) info.normal[i] = normal[i]; */
/*           } */
/*         } */
/*       } */
/*       if (result != nullptr) return result; */
/*     } */
/*   } else { */
/*   } */
/*   return nullptr; */
/* }; */

std::ostream& operator << (std::ostream& stream, const Ray& ray) {
  stream << "origin: " << ray.origin << " direction: " << ray.direction;
  return stream;
}

#endif
