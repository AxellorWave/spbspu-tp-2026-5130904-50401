#include <iostream>
#include <vector>
#include <iterator>
#include "input.hpp"

int main()
{
  using zharov::DataStruct;
  std::vector< DataStruct > data;
  {
    while (!std::cin.eof())
    {
      using iit_t = std::istream_iterator< DataStruct >;
      std::copy(iit_t{std::cin}, iit_t{}, std::back_inserter(data));
      if (std::cin.fail())
      {
        std::cin.clear();
        std::streamsize max = std::numeric_limits< std::streamsize >::max();
        std::cin.ignore(max, '\n');
      }
    }
  }
  std::sort(data.begin(), data.end(), std::less< DataStruct >{});
  {
    using oit_t = std::ostream_iterator< DataStruct >;
    std::copy(std::begin(data), std::end(data), oit_t{std::cout, "\n"});
  }
}
