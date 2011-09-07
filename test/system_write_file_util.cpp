#include <iostream>

#include "util/pathname.hpp"
#include "util/system.hpp"

int main(int argc, char** argv)
{
  if (argc != 3)
  {
    std::cout << "Usage: " << argv[0] << " OUTFILE CONTENT" << std::endl;
    return 1;
  }
  else
  {
    System::write_file(argv[1], argv[2]);
  }

  return 0;
}

/* EOF */
