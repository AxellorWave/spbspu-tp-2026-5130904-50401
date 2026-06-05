#include "commands.hpp"
#include <algorithm>
#include <string>
#include <vector>
#include <cmath>
#include <functional>
#include <iomanip>
#include <iterator>
#include <numeric>
#include <stdexcept>

zharov::IOGuard::IOGuard(std::basic_ios< char >& s):
  s_(s),
  width_(s.width()),
  precision_(s.precision()),
  fmt_(s.flags()),
  fill_(s.fill())
{}

zharov::IOGuard::~IOGuard()
{
  s_.width(width_);
  s_.precision(precision_);
  s_.fill(fill_);
  s_.flags(fmt_);
}

namespace
{
  double getTriangleArea(const zharov::Point& o, const zharov::Point& a, const zharov::Point& b)
  {
    return std::abs(static_cast< double >((a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x))) /
      2;
  }

  double getArea(const zharov::Polygon& poly)
  {
    const auto& pts = poly.points;
    std::vector< double > areas(pts.size() - 2);
    using namespace std::placeholders;
    std::transform(pts.begin() + 1, pts.end() - 1, pts.begin() + 2, areas.begin(),
      std::bind(getTriangleArea, std::cref(pts[0]), _1, _2));
    return std::accumulate(areas.begin(), areas.end(), 0.0);
  }

  double sumAreas(const zharov::data_t& data)
  {
    std::vector< double > areas(data.size());
    std::transform(data.begin(), data.end(), areas.begin(), getArea);
    return std::accumulate(areas.begin(), areas.end(), 0.0);
  }

  bool isEven(const zharov::Polygon& p)
  {
    return p.points.size() % 2 == 0;
  }

  bool isOdd(const zharov::Polygon& p)
  {
    return p.points.size() % 2 != 0;
  }

  bool hasExpN(const zharov::Polygon& p, size_t n)
  {
    return p.points.size() == n;
  }
}

void zharov::handleArea(std::istream& in, std::ostream& out, const data_t& data)
{
  std::string sub;
  in >> sub;
  if (!in)
  {
    throw std::invalid_argument("");
  }
  IOGuard guard(out);
  out << std::fixed << std::setprecision(1);
  data_t filtered;
  if (sub == "EVEN")
  {
    std::copy_if(data.begin(), data.end(), std::back_inserter(filtered), isEven);
    out << sumAreas(filtered) << '\n';
  }
  else if (sub == "ODD")
  {
    std::copy_if(data.begin(), data.end(), std::back_inserter(filtered), isOdd);
    out << sumAreas(filtered) << '\n';
  }
  else if (sub == "MEAN")
  {
    if (data.empty())
    {
      throw std::invalid_argument("");
    }
    out << sumAreas(data) / data.size() << '\n';
  }
  else
  {
    size_t n = std::stoull(sub);
    if (n < 3)
    {
      throw std::invalid_argument("");
    }
    using namespace std::placeholders;
    std::copy_if(data.begin(), data.end(), std::back_inserter(filtered), std::bind(hasExpN, _1, n));
    out << sumAreas(filtered) << '\n';
  }
}

void zharov::handleMax(std::istream&, std::ostream&, const data_t&)
{}

void zharov::handleMin(std::istream&, std::ostream&, const data_t&)
{}

void zharov::handleCount(std::istream&, std::ostream&, const data_t&)
{}

void zharov::handleRects(std::istream&, std::ostream&, const data_t&)
{}

void zharov::handleRightShapes(std::istream&, std::ostream&, const data_t&)
{}
