#include <fstream>
#include <iostream>
#include <stdexcept>

#include "engine/display/sprite_description.hpp"
#include "pingus/path_manager.hpp"
#include "pingus/resource.hpp"
#include "pingus/resource_manager.hpp"
#include "util/sexpr_file_writer.hpp"
#include "util/system.hpp"

/** Creates a sed script from the aliases in the resource file */
int main()
{
  g_path_manager.set_path("data/");
  Resource::init(true);

  // print aliases
  const ResourceManager::Aliases& aliases = Resource::resmgr.get_aliases();
  for(ResourceManager::Aliases::const_iterator it = aliases.begin(); it != aliases.end(); ++it)
  {
    std::cout << "s%" << it->first << "%" << it->second << "%g" << std::endl;
  }

  return 0;
}

/* EOF */
