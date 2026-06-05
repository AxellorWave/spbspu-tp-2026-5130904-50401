#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <vector>
#include "polygon.hpp"

namespace zharov
{
  class IOGuard
  {
  public:
    explicit IOGuard(std::basic_ios< char >& s);
    ~IOGuard();

  private:
    std::basic_ios< char >& s_;
    std::streamsize width_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
    char fill_;
  };

  using data_t = std::vector< Polygon >;

  void handleArea(std::istream&, std::ostream&, const data_t&);
  void handleMax(std::istream&, std::ostream&, const data_t&);
  void handleMin(std::istream&, std::ostream&, const data_t&);
  void handleCount(std::istream&, std::ostream&, const data_t&);
  void handleRects(std::istream&, std::ostream&, const data_t&);
  void handleRightShapes(std::istream&, std::ostream&, const data_t&);
}

#endif
