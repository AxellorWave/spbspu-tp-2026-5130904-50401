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
    const std::vector< zharov::Point >& pts = poly.points;
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

  bool lessArea(const zharov::Polygon& a, const zharov::Polygon& b)
  {
    return getArea(a) < getArea(b);
  }

  bool lessVertexes(const zharov::Polygon& a, const zharov::Polygon& b)
  {
    return a.points.size() < b.points.size();
  }

  struct Vec
  {
    int dx, dy;
  };

  Vec makeEdge(const zharov::Point& a, const zharov::Point& b)
  {
    return {b.x - a.x, b.y - a.y};
  }

  bool isRightAngle(const Vec& u, const Vec& v)
  {
    return u.dx * v.dx + u.dy * v.dy == 0;
  }

  bool hasRightAngle(const zharov::Polygon& p)
  {
    const auto& pts = p.points;
    std::vector< Vec > edges(pts.size() - 1);
    std::transform(pts.begin(), pts.end() - 1, pts.begin() + 1, edges.begin(), makeEdge);
    edges.push_back(makeEdge(pts.back(), pts.front()));
    bool found = std::adjacent_find(edges.begin(), edges.end(), isRightAngle) != edges.end();
    return found || isRightAngle(edges.back(), edges.front());
  }

  bool isRect(const zharov::Polygon& p)
  {
    if (p.points.size() != 4)
    {
      return false;
    }
    const std::vector< zharov::Point >& v = p.points;
    return isRightAngle(makeEdge(v[3], v[0]), makeEdge(v[0], v[1])) &&
      isRightAngle(makeEdge(v[0], v[1]), makeEdge(v[1], v[2])) &&
      isRightAngle(makeEdge(v[1], v[2]), makeEdge(v[2], v[3]));
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

void zharov::handleMax(std::istream& in, std::ostream& out, const data_t& data)
{
  if (data.empty())
  {
    throw std::invalid_argument("");
  }
  std::string sub;
  in >> sub;
  if (!in)
  {
    throw std::invalid_argument("");
  }
  if (sub == "AREA")
  {
    IOGuard guard(out);
    out << std::fixed << std::setprecision(1);
    out << getArea(*std::max_element(data.begin(), data.end(), lessArea)) << '\n';
  }
  else if (sub == "VERTEXES")
  {
    out << std::max_element(data.begin(), data.end(), lessVertexes)->points.size() << '\n';
  }
  else
  {
    throw std::invalid_argument("");
  }
}

void zharov::handleMin(std::istream& in, std::ostream& out, const data_t& data)
{
  if (data.empty())
  {
    throw std::invalid_argument("");
  }
  std::string sub;
  in >> sub;
  if (!in)
  {
    throw std::invalid_argument("");
  }
  if (sub == "AREA")
  {
    IOGuard guard(out);
    out << std::fixed << std::setprecision(1);
    out << getArea(*std::min_element(data.begin(), data.end(), lessArea)) << '\n';
  }
  else if (sub == "VERTEXES")
  {
    out << std::min_element(data.begin(), data.end(), lessVertexes)->points.size() << '\n';
  }
  else
  {
    throw std::invalid_argument("");
  }
}

void zharov::handleCount(std::istream& in, std::ostream& out, const data_t& data)
{
  std::string sub;
  in >> sub;
  if (!in)
  {
    throw std::invalid_argument("");
  }
  if (sub == "EVEN")
  {
    out << std::count_if(data.begin(), data.end(), isEven) << '\n';
  }
  else if (sub == "ODD")
  {
    out << std::count_if(data.begin(), data.end(), isOdd) << '\n';
  }
  else
  {
    size_t n = std::stoull(sub);
    if (n < 3)
    {
      throw std::invalid_argument("");
    }
    using namespace std::placeholders;
    out << std::count_if(data.begin(), data.end(), std::bind(hasExpN, _1, n)) << '\n';
  }
}

void zharov::handleRects(std::istream&, std::ostream& out, const data_t& data)
{
  out << std::count_if(data.begin(), data.end(), isRect) << '\n';
}

void zharov::handleRightShapes(std::istream&, std::ostream& out, const data_t& data)
{
  out << std::count_if(data.begin(), data.end(), hasRightAngle) << '\n';
}
