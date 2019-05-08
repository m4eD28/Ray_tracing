#ifndef RANDOM_H
#define RANDOM_H
#include <random>

std::random_device rnd_dev;
std::mt19937 mt(rnd_dev());
std::uniform_real_distribution<> dist(0, 1);
inline double rnd() {
  return dist(mt);
}

void sampleDisk(double& x, double& y) {
  double u1 = rnd();
  double u2 = rnd();
  x = std::sqrt(u1)*std::cos(2*M_PI*u2);
  y = std::sqrt(u1)*std::sin(2*M_PI*u2);
}
#endif
