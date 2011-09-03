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
  g_path_manager.set_path("data/");
  Resource::init(true);

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

      std::string orig_image_file = System::normalize_path(desc.filename.get_raw_path());
      std::string image_file = System::normalize_path(desc.filename.get_raw_path());
      std::string outfile = "data/images/" + resname + ".sprite";

      // if directory is the same, cut it out
      if (System::dirname(outfile) == ("data/" + System::dirname(image_file)))
      {
        image_file = System::basename(image_file);
      }
      else if (System::dirname(System::dirname(outfile)) == 
               ("data/" + System::dirname(image_file)))
      {
        image_file = "../" + System::basename(image_file);
      }
      else
      {
        image_file = "/" + image_file;
      }

      if (false)
      {
        std::cout << resname << std::endl;
        std::cout << "     outfile : " << outfile << std::endl;
        std::cout << "     default : " << (is_default_sprite(desc) ? "DEFAULT" : "regular") << std::endl;
        std::cout << "  image_file : " << image_file << std::endl;
        std::cout << "     resname : " << resname << std::endl;
        std::cout << "    filename : " << desc.filename.get_raw_path() << std::endl;
        std::cout << "      offset : " << desc.offset << std::endl;
        std::cout << "      origin : " << desc.origin << std::endl;
        std::cout << "        loop : " << desc.loop << std::endl;
        std::cout << "       speed : " << desc.speed << std::endl;
        std::cout << "       array : " << desc.array << std::endl;
        std::cout << "   frame_pos : " << desc.frame_pos << std::endl;
        std::cout << "  frame_size : " << desc.frame_size << std::endl;
        std::cout << std::endl;
      }

      //std::cout << "images/" + resname << "   " << orig_image_file.substr(0, orig_image_file.size()-4) << std::endl;

      // FIXME: doesn't handle sprites that are default, but in a different path
      if (!is_default_sprite(desc) ||
          ("images/" + resname) != orig_image_file.substr(0, orig_image_file.size()-4))
      {
        System::create_dir(System::dirname(outfile));

        std::cout << "Writing: " << outfile << std::endl;
        std::ofstream out(outfile);
        if (!out)
        {
          throw std::runtime_error(outfile + ": failed to open output file");
        }
        else
        {
          SExprFileWriter writer(out);
          writer.begin_section("pingus-sprite");
          writer.write_string("image", image_file);
          if (desc.offset != Vector2i(0,0))
            writer.write_vector2i("offset", desc.offset);

          if (desc.origin != origin_top_left)
            writer.write_enum("origin", desc.origin, origin2string);

          if (desc.frame_pos != Vector2i(0, 0))
            writer.write_vector2i("position",   desc.frame_pos);

          if (desc.array != Size(1, 1))
          {
            writer.write_int ("speed",  desc.speed);
            writer.write_bool("loop",   desc.loop);
            writer.write_size("array", desc.array);
          }

          if (desc.frame_size != Size(-1, -1)) 
            writer.write_size("size",  desc.frame_size);

          writer.end_section();
          out << std::endl;
        }
      }
    }
  }

  if (false)
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
