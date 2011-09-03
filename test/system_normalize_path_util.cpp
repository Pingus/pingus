#include <iostream>

#include "util/system.hpp"

int main(int argc, char** argv)
{
  for(int i = 1; i < argc; ++i)
  {
    std::cout << System::normalize_path(argv[i]) << std::endl;
  }

  return 0;
}

/* EOF */
