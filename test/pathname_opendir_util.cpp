#include <iostream>

#include "pingus/path_manager.hpp"
#include "util/pathname.hpp"

int main(int argc, char** argv)
{
  g_path_manager.set_path("data");
  for(int i = 1; i < argc; ++i)
  {
    g_path_manager.add_overlay_path(argv[i]);
  }

  Pathname path("levels", Pathname::DATA_PATH);
  auto lst = path.opendir();
  for(auto it = lst.begin(); it != lst.end(); ++it)
  {
    std::cout << *it << std::endl;
  }
  return 0;
}

/* EOF */
