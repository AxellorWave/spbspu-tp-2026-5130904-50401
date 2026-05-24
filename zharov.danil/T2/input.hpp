#ifndef INPUT_HPP
#define INPUT_HPP
#include <algorithm>
#include <string>
#include <complex>
#include <iomanip>
#include <limits>
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

  struct KeyIO
  {
    std::string key;
    DataStruct& dest;
  };

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
  std::istream& operator>>(std::istream& in, KeyIO&& dest);
  std::istream& operator>>(std::istream& in, UllIO&& dest);
  std::istream& operator>>(std::istream& in, CmpIO&& dest);
  std::ostream& operator<<(std::ostream& out, const ConstCmpIO& dest);
  std::istream& operator>>(std::istream& in, StringIO&& dest);
  std::istream& operator>>(std::istream& in, DataStruct& dest);
  std::ostream& operator<<(std::ostream& out, const DataStruct& dest);
  bool operator<(const DataStruct& lhs, const DataStruct& rhs);
}

#endif
