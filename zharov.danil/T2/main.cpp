#include <iostream>
#include <complex>

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
