#ifndef BVH_H
#define BVH_H
#include <vector>
#include <cmath>
#include <algorithm>

struct BVH_node {
  float bbox[2][3];
  int children[2];
  vector<Triangle *> polygons;
};

float surfaceArea(const float bbox[2][3]) {
  float dx = bbox[1][0] - bbox[0][0];
  float dy = bbox[1][1] - bbox[0][1];
  float dz = bbox[1][2] - bbox[0][2];
  return 2*(dx*dy + dx*dz * dy*dz);
}

void emptyAABB(float bbox[2][3]) {
  bbox[0][0] = bbox[0][1] = bbox[0][2] = INF;
  bbox[1][0] = bbox[1][1] = bbox[1][2] = -INF;
}

void mergeAABB(const float bbox1[2][3], const float bbox2[2][3], float result[2][3]) {
  for (int j = 0; j < 3; j++) {
    result[0][j] = min(bbox1[0][j], bbox2[0][j]);
    result[1][j] = min(bbox1[1][j], bbox2[1][j]);
  }
}

#endif
