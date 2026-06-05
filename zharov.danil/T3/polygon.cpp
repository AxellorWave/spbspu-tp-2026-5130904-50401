#include "polygon.hpp"
#include <limits>

namespace
{
  void skipSpaces(std::istream& in)
  {
    if (in && (in.peek() == ' '))
    {
      in.get();
      skipSpaces(in);
    }
  }

  void readPoints(std::istream& in, std::vector< zharov::Point >& pts, size_t n)
  {
    if (n == 0 || !in)
    {
      return;
    }
    zharov::Point pt;
    in >> pt;
    if (in)
    {
      pts.push_back(pt);
      readPoints(in, pts, n - 1);
    }
  }
}

std::istream& zharov::operator>>(std::istream& in, DelimiterIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  char c = 0;
  in >> c;
  if (in && (c != dest.exp))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& zharov::operator>>(std::istream& in, Point& pt)
{
  skipSpaces(in);
  std::istream::sentry sentry(in, true);
  if (!sentry)
  {
    return in;
  }
  if (in.peek() == '\n')
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  using d = DelimiterIO;
  in >> d{'('} >> pt.x >> d{';'} >> pt.y >> d{')'};
  return in;
}

std::istream& zharov::operator>>(std::istream& in, Polygon& poly)
{
  poly.points.clear();
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  const std::streamsize max = std::numeric_limits< std::streamsize >::max();
  size_t n = 0;
  in >> n;
  if (!in)
  {
    if (!in.eof())
    {
      in.clear();
      in.ignore(max, '\n');
    }
    return in;
  }
  if (n < 3)
  {
    in.ignore(max, '\n');
    return in;
  }
  std::vector< Point > pts;
  pts.reserve(n);
  readPoints(in, pts, n);
  if (pts.size() != n)
  {
    if (!in.eof())
    {
      in.clear();
      in.ignore(max, '\n');
    }
    return in;
  }
  skipSpaces(in);
  if (in.peek() != '\n' && in.peek() != std::char_traits< char >::eof())
  {
    in.ignore(max, '\n');
    return in;
  }
  poly.points = std::move(pts);
  in.ignore(max, '\n');
  return in;
}

bool zharov::isInvalid(const Polygon& p)
{
  return p.points.empty();
}
