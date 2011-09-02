#include <fstream>
#include <iostream>
#include <stdexcept>

#include "engine/display/sprite_description.hpp"
#include "pingus/path_manager.hpp"
#include "pingus/resource.hpp"
#include "pingus/resource_manager.hpp"
#include "util/sexpr_file_writer.hpp"
#include "util/system.hpp"

bool is_default_sprite(const SpriteDescription& desc)
{
  return 
    desc.offset     == Vector2i(0, 0) &&
    desc.origin     == origin_top_left &&
    desc.array      == Size(1, 1) &&
    desc.frame_pos  == Vector2i(0, 0) &&
    desc.frame_size == Size(-1, -1);
}

int main(int argc, char** argv)
{
  g_path_manager.set_base_path("data/");
  Resource::init();

  {  // print SpriteDescription
    const ResourceManager::Resources& resources = Resource::resmgr.get_resources();
    for(ResourceManager::Resources::const_iterator it = resources.begin(); it != resources.end(); ++it)
    {
      const std::string& resname = it->first;
      const SpriteDescription& desc = *(it->second);

      if (is_default_sprite(desc) && 
          (desc.loop  != 1 ||
           desc.speed != 100))
      {
        throw std::runtime_error(resname + ": unexpected SpriteDescription");
      }

      std::string base = System::basename(desc.filename.get_raw_path());
      base = base.substr(0, base.size() - 4);
      std::string dir = System::dirname(desc.filename.get_raw_path());

      // FIXME: must make sure that resname and dir+/+base are the
      // same or otherwise adjust the image file path

      std::string outfile = "data/" + resname + ".sprite";

      std::cout << resname << std::endl;
      std::cout << "     default : " << (is_default_sprite(desc) ? "DEFAULT" : "regular") << std::endl;
      std::cout << "     outfile : " << outfile << std::endl;
      //std::cout << "    basename : " << base << std::endl;
      //std::cout << "    dirname  : " << dir << std::endl;
      std::cout << "    filename : " << desc.filename << std::endl;
      std::cout << "     offset  : " << desc.offset << std::endl;
      std::cout << "      origin : " << desc.origin << std::endl;
      std::cout << "        loop : " << desc.loop << std::endl;
      std::cout << "       speed : " << desc.speed << std::endl;
      std::cout << "       array : " << desc.array << std::endl;
      std::cout << "   frame_pos : " << desc.frame_pos << std::endl;
      std::cout << "  frame_size : " << desc.frame_size << std::endl;
      std::cout << std::endl;
      
      if (false)
      if (!is_default_sprite(desc))
      {
        std::ofstream out(outfile);
        if (!out)
        {
          throw std::runtime_error(outfile + ": failed to open output file");
        }
        else
        {
          SExprFileWriter writer(out);
          writer.begin_section("pingus-sprite");
          writer.write_int      ("speed",  desc.speed);
          writer.write_bool     ("loop",   desc.loop);
          writer.write_vector2i ("offset", desc.offset);
          writer.write_enum     ("origin", desc.origin, origin2string);
          writer.write_string   ("image-file",  System::basename(desc.filename.get_raw_path()));
          writer.write_size     ("image-array", desc.array);
          writer.write_vector2i ("image-pos",   desc.frame_pos);
          writer.write_size     ("image-size",  desc.frame_size);
          writer.end_section();
          out << "\n\n;; EOF ;;" << std::endl;
        }
      }
    }
  }

  { // print aliases
    const ResourceManager::Aliases& aliases = Resource::resmgr.get_aliases();
    for(ResourceManager::Aliases::const_iterator it = aliases.begin(); it != aliases.end(); ++it)
    {
      std::cout << it->first << std::endl;
      std::cout << "  " << it->second << std::endl;
      std::cout << std::endl;
    }
  }

  Resource::deinit();

  return 0;
}

/* EOF */
