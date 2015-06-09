#include <uitest/uitest.hpp>
#include <iostream>

#include "util/system.hpp"

UITEST_S(System, realpath, "PATH", "")
{
  std::cout << System::realpath(arg) << std::endl;
}

UITEST(System, opendir_recursive, "DIRECTORY")
{
  std::vector<std::string> lst = System::opendir_recursive(args[0]);
  for(auto it = lst.begin(); it != lst.end(); ++it)
  {
    std::cout << *it << std::endl;
  }
}

/* EOF */
