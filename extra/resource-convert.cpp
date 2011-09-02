#include <iostream>

#include "engine/display/sprite_description.hpp"
#include "pingus/path_manager.hpp"
#include "pingus/resource.hpp"
#include "pingus/resource_manager.hpp"

int main(int argc, char** argv)
{
  g_path_manager.set_base_path("data/");
  Resource::init();
  
  const ResourceManager::Resources& resources = Resource::resmgr.get_resources();

  for(ResourceManager::Resources::const_iterator it = resources.begin(); it != resources.end(); ++it)
  {
    const std::string& resname = it->first;
    const SpriteDescription& desc = *(it->second);

    std::cout << resname << std::endl;
    std::cout << "    filename : " << desc.filename << std::endl;
    std::cout << "     offset  : " << desc.offset << std::endl;
    std::cout << "      origin : " << desc.origin << std::endl;
    std::cout << "        loop : " << desc.loop << std::endl;
    std::cout << "       speed : " << desc.speed << std::endl;
    std::cout << "       array : " << desc.array << std::endl;
    std::cout << "   frame_pos : " << desc.frame_pos << std::endl;
    std::cout << "  frame_size : " << desc.frame_size << std::endl;
    std::cout << std::endl;
  }

  Resource::deinit();

  return 0;
}

/* EOF */
