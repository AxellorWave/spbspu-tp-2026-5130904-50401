#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "polygon.hpp"
#include <vector>
#include <iostream>

namespace zharov
{
  using data_t = std::vector< Polygon >;

  void handleArea(std::istream&, std::ostream&, const data_t&);
  void handleMax(std::istream&, std::ostream&, const data_t&);
  void handleMin(std::istream&, std::ostream&, const data_t&);
  void handleCount(std::istream&, std::ostream&, const data_t&);
  void handleRects(std::istream&, std::ostream&, const data_t&);
  void handleRightShapes(std::istream&, std::ostream&, const data_t&);
}

#endif
