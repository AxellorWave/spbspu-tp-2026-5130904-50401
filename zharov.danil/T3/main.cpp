#include <algorithm>
#include <iostream>
#include <vector>
#include <fstream>
#include <functional>
#include <iterator>
#include <limits>
#include <unordered_map>
#include "commands.hpp"
#include "polygon.hpp"

namespace
{
  using cmd_t = std::function< void(std::istream&, std::ostream&) >;

  void runCommands(std::unordered_map< std::string, cmd_t >& cmds)
  {
    std::string cmd;
    if (!(std::cin >> cmd))
    {
      return;
    }
    try
    {
      cmds.at(cmd)(std::cin, std::cout);
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
    runCommands(cmds);
  }
}

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "Bad arg\n";
    return 1;
  }
  std::ifstream file(argv[1]);
  if (!file)
  {
    std::cerr << "Cannot open file\n";
    return 1;
  }
  zharov::data_t polygons;
  {
    using iit_t = std::istream_iterator< zharov::Polygon >;
    std::copy(iit_t{file}, iit_t{}, std::back_inserter(polygons));
    polygons.erase(
      std::remove_if(polygons.begin(), polygons.end(), zharov::isInvalid), polygons.end());
  }
  std::unordered_map< std::string, cmd_t > cmds;
  {
    using namespace std::placeholders;
    cmds["AREA"] = std::bind(zharov::handleArea, _1, _2, std::cref(polygons));
    cmds["MAX"] = std::bind(zharov::handleMax, _1, _2, std::cref(polygons));
    cmds["MIN"] = std::bind(zharov::handleMin, _1, _2, std::cref(polygons));
    cmds["COUNT"] = std::bind(zharov::handleCount, _1, _2, std::cref(polygons));
    cmds["RECTS"] = std::bind(zharov::handleRects, _1, _2, std::cref(polygons));
    cmds["RIGHTSHAPES"] = std::bind(zharov::handleRightShapes, _1, _2, std::cref(polygons));
  }
  runCommands(cmds);
}
