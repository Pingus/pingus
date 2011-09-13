#include <iostream>

#include "engine/display/surface.hpp"
#include "math/color.hpp"
#include "math/size.hpp"
#include "math/vector3f.hpp"
#include "pingus/pingus_level.hpp"
#include "pingus/res_descriptor.hpp"
#include "pingus/resource.hpp"
#include "util/command_line.hpp"
#include "util/log.hpp"
#include "util/pathname.hpp"
#include "pingus/path_manager.hpp"
#include "pingus/worldobj_renderer.hpp"
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

    // render all the objects
    WorldObjRenderer renderer(out_surface);
    renderer.process(plf.get_objects());

    Screenshot::save(out_surface.get_surface(), files[1].get_sys_path());

    Resource::deinit();
  }

  return 0;
}

/* EOF */
