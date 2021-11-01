#include "test.h"


coord::coord(int x, int y, int z) : x_{x}, y_{y}, z_{z} {};

void coord::print() {
  std::cout << "X: " << x_ << std::endl;
  std::cout << "Y: " << y_ << std::endl;
  std::cout << "Z: " << z_ << std::endl;
}