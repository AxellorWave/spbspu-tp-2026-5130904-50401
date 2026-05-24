#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <complex>
#include <iomanip>
#include <iterator>
#include <sstream>

namespace zharov
{
  using ull_t = unsigned long long;

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

  struct DataStruct
  {
    ull_t key1;
    std::complex< double > key2;
    std::string key3;
  };

  struct DelimiterIO
  {
    char exp;
  };

  struct UllIO
  {
    ull_t& ref;
  };

  struct CmpIO
  {
    std::complex< double >& ref;
  };

  struct ConstCmpIO
  {
    const std::complex< double >& ref;
  };

  struct StringIO
  {
    std::string& ref;
  };

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
  std::istream& operator>>(std::istream& in, UllIO&& dest);
  std::istream& operator>>(std::istream& in, CmpIO&& dest);
  std::ostream& operator<<(std::ostream& out, const ConstCmpIO& dest);
  std::istream& operator>>(std::istream& in, StringIO&& dest);
  std::istream& operator>>(std::istream& in, DataStruct& dest);
  std::ostream& operator<<(std::ostream& out, const DataStruct& dest);
  bool operator<(const DataStruct& lhs, const DataStruct& rhs);
}

int main()
{}

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

std::istream& zharov::operator>>(std::istream& in, DataStruct& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  DataStruct input;
  {
    using sep = DelimiterIO;
    using ull = UllIO;
    using cmp = CmpIO;
    using str = StringIO;
    std::string key1, key2, key3;
    in >> sep{'('};
    in >> sep{':'} >> key1 >> ull{input.key1};
    in >> sep{':'} >> key2 >> cmp{input.key2};
    in >> sep{':'} >> key3 >> str{input.key3};
    in >> sep{':'} >> sep{')'};
  }
  if (in)
  {
    dest = input;
  }
  return in;
}

std::ostream& zharov::operator<<(std::ostream& out, const DataStruct& src)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  IOGuard fmtguard(out);
  out << "(";
  out << ":\"key1\" 0" << src.key1;
  out << ":\"key2\" " << ConstCmpIO{src.key2};
  out << ":\"key3\" " << src.key3;
  out << ":)";
  return out;
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

std::istream& zharov::operator>>(std::istream& in, StringIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  return std::getline(in >> DelimiterIO{'"'}, dest.ref, '"');
}

std::istream& zharov::operator>>(std::istream& in, UllIO&& dest)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  double a = 0, b = 0;
  in >> DelimiterIO{{'0'}};
  in >> a;
  dest.ref = a;
  return in
}

std::istream& zharov::operator>>(std::istream& in, CmpIO&& dest)
{
  std::istream::sentry s(in);
  if (!s)
  {
    return in;
  }
  double a = 0, b = 0;
  in >> DelimiterIO{{'#'}} >> DelimiterIO{{'c'}};
  in >> DelimiterIO{{'('}} >> a >> b >> DelimiterIO{{')'}};
  dest.ref = {a, b};
  return in;
}

std::ostream& zharov::operator<<(std::ostream& out, const ConstCmpIO& dest)
{
  std::ostream::sentry s(out);
  if (!s)
  {
    return out;
  }
  double a = 0, b = 0;
  out << "#c(";
  out << std::fixed << std::setprecision(1) << dest.ref.real() << ' ' << dest.ref.imag() << ')';
  return out;
}

bool zharov::operator<(const DataStruct& lhs, const DataStruct& rhs)
{
  bool f = lhs.key1 < rhs.key1;
  if (!f)
  {
    f = f || std::abs(lhs.key2) < std::abs(rhs.key2);
  }
  f = f || lhs.key3.length() < rhs.key3.length();
  return f;
}
