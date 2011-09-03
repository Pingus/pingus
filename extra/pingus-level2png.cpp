#include <iostream>

#include "engine/display/surface.hpp"
#include "math/color.hpp"
#include "math/size.hpp"
#include "math/vector3f.hpp"
#include "pingus/pingus_level.hpp"
#include "pingus/res_descriptor.hpp"
#include "pingus/resource.hpp"
#include "util/command_line.hpp"
#include "util/pathname.hpp"
#include "pingus/path_manager.hpp"
#include "engine/display/screenshot.hpp"

int main(int argc, char** argv)
{
  std::vector<Pathname> files;

  CommandLine argp;
  argp.add_usage("[OPTIONS]... LEVELFILE OUTPUTFILE");

  argp.add_option('h', "help",    "", "Displays this help");
  argp.add_option('b', "background",  "RRGGBBAA", "Set background color");
  argp.add_option('c', "colmap", "", "Render collision map instead of graphic map");

  argp.parse_args(argc, argv);

  while (argp.next())
  {
    switch (argp.get_key()) 
    {          
      case 'h':
        argp.print_help();
        exit(EXIT_SUCCESS);
        break;

      case 'c':
        // FIXME: not implemented
        break;

      case 'b':
        // FIXME: not implemented
        break;

      case CommandLine::REST_ARG:
        files.push_back(Pathname(argp.get_argument(), Pathname::SYSTEM_PATH));
        break;
    }
  }
  
  if (files.size() != 2)
  {
    argp.print_help();
    exit(EXIT_SUCCESS);
  }
  else
  {
    g_path_manager.set_path("data/");
    Resource::init();
    PingusLevel plf(files[0]);

    Surface out_surface(plf.get_size().width, plf.get_size().height);

    // FIXME: alpha doesn't work, as the PNG saver can't handle that
    out_surface.fill(Color(255, 255, 255, 255));

    const std::vector<FileReader>& objects = plf.get_objects();
    
    for(auto it = objects.begin(); it != objects.end(); ++it)
    {
      const FileReader& reader = *it;

      Vector3f pos;
      ResDescriptor desc;

      // FIXME: does not handle sprite alignment
      // FIXME: does not handle remove groundpieces
      // FIXME: does not handle liquid

      if (reader.get_name() != "surface-background" && 
          reader.get_name() != "starfield-background" &&
          reader.get_name() != "solidcolor-background")
      {
        if (reader.read_vector("position", pos) &&
            reader.read_desc("surface", desc))
        {
          Surface surface = Resource::load_surface(desc);
          if (reader.get_name() == "exit")
          {
            // FIXME: hack, should take that info from the resource file
            out_surface.blit(surface, 
                             static_cast<int>(pos.x) - surface.get_width()/2, 
                             static_cast<int>(pos.y) - surface.get_height());
          }
          else if (reader.get_name() == "groundpiece")
          {
            std::string type;
            reader.read_string("type", type);
            if (type == "remove")
            {
              // FIXME: don't have blit_remove()
              out_surface.blit(surface, 
                               static_cast<int>(pos.x), 
                               static_cast<int>(pos.y));
            }
            else
            {
              out_surface.blit(surface, 
                               static_cast<int>(pos.x), 
                               static_cast<int>(pos.y));
            }
          }
          else
          {
            out_surface.blit(surface, 
                             static_cast<int>(pos.x), 
                             static_cast<int>(pos.y));
          }
        }
      }
    }

    Screenshot::save(out_surface.get_surface(), files[1].get_sys_path());

    Resource::deinit();
  }

  return 0;
}

/* EOF */
