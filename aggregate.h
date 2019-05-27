#ifndef AGGREGATE_H
#define AGGREGATE_H
#include <memory>
#include <vector>
#include <deque>
#include "ray.h"
#include "hit.h"
#include "figure.h"

class Aggregate {
  public:
    std::deque<std::shared_ptr<Figure>> shapes; //物体

    Aggregate() {};
    Aggregate(const std::deque<std::shared_ptr<Figure>>& _shapes) :
    shapes(_shapes) {};

    void add(const std::shared_ptr<Figure>& s) {
      shapes.push_back(s);
    };

    //衝突計算(衝突したらresに情報を格納return true)
    bool intersect(const Ray& ray, Hit& res) const {
      bool hit = false;
      for (auto s : shapes) {
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
