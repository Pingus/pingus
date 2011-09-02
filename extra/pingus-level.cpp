#include <iostream>

#include "math/color.hpp"
#include "pingus/pingus_level.hpp"
#include "util/pathname.hpp"

int main(int argc, char** argv)
{
  for(int i = 1; i < argc; ++i)
  {
    Pathname path(argv[i], Pathname::SYSTEM_PATH);
    PingusLevel plf(path);
    
    std::cout << "filename      : " << path << std::endl;
    std::cout << "name          : " << plf.get_levelname() << std::endl;
    std::cout << "checksum      : " << plf.get_checksum() << std::endl;
    std::cout << "description   : " << plf.get_description() << std::endl;
    std::cout << "author        : " << plf.get_author() << std::endl;
    std::cout << "music         : " << plf.get_music() << std::endl;
    std::cout << "ambient light : " 
              << static_cast<int>(plf.get_ambient_light().r) << " "
              << static_cast<int>(plf.get_ambient_light().g) << " " 
              << static_cast<int>(plf.get_ambient_light().b) << " " 
              << static_cast<int>(plf.get_ambient_light().a)
              << std::endl;
    std::cout << "objects       : " << plf.get_objects().size() << std::endl;
    std::cout << std::endl;
  }

  return 0;
}

/* EOF */
