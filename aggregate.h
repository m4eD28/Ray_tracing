#ifndef AGGREGATE_H
#define AGGREGATE_H
#include <memory>
#include <vector>
#include "ray.h"
#include "hit.h"
#include "figure.h"

class Aggregate {
  public:
    std::vector<std::shared_ptr<Figure>> spheres; //物体

    Aggregate() {};
    Aggregate(const std::vector<std::shared_ptr<Figure>>& _spheres) :
    spheres(_spheres) {};

    void add(const std::shared_ptr<Figure>& s) {
      spheres.push_back(s);
    };

    //衝突計算(衝突したらresに情報を格納return true)
    bool intersect(const Ray& ray, Hit& res) const {
      bool hit = false;
      for (auto s : spheres) {
        Hit res_temp;
        if(s->intersect(ray, res_temp)) {
          //衝突した物体がさらに手前にあればtrue
          if(res_temp.t < res.t) {
            hit = true;
            res = res_temp;
          }
        }
      }
      return hit;
    };
};
#endif
