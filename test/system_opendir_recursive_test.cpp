#include <iostream>

#include "util/system.hpp"

int main(int argc, char** argv)
{
  for(int i = 1; i < argc; ++i)
  {
    std::vector<std::string> lst = System::opendir_recursive(argv[i]);
    for(auto it = lst.begin(); it != lst.end(); ++it)
    {
      std::cout << *it << std::endl;
    }
  }
  return 0;
}

/* EOF */
