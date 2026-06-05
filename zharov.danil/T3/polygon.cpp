#include "polygon.hpp"
#include <iterator>

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
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  using d = DelimiterIO;
  in >> d{'('} >> pt.x >> d{';'} >> pt.y >> d{')'};
  return in;
}

std::istream& zharov::operator>>(std::istream& in, Polygon& poly)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  size_t n = 0;
  in >> n;
  if (!in || n < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  std::vector< Point > pts(n);
  using iit_t = std::istream_iterator< Point >;
  std::copy_n(iit_t{in}, n, pts.begin());
  if (in)
  {
    poly.points = std::move(pts);
  }
  return in;
}
