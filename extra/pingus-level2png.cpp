#include <iostream>
#include <sstream>

#include <argpp/argpp.hpp>
#include <logmich/log.hpp>

#include "engine/display/surface.hpp"
#include "math/color.hpp"
#include "math/size.hpp"
#include "math/vector2f.hpp"
#include "pingus/pingus_level.hpp"
#include "pingus/res_descriptor.hpp"
#include "pingus/resource.hpp"
#include "pingus/prefab_file.hpp"
#include "util/pathname.hpp"
#include "util/system.hpp"
#include "util/writer.hpp"
#include "pingus/path_manager.hpp"
#include "pingus/worldobj_renderer.hpp"
#include "engine/display/screenshot.hpp"

using namespace pingus;

int main(int argc, char** argv)
{
  std::vector<Pathname> files;
  bool crop = false;

  argpp::Parser argp;
  argp.add_usage(argv[0], "[OPTIONS]... LEVELFILE OUTPUTFILE")
    .add_option('h', "help",    "", "Displays this help")
    .add_option('b', "background",  "RRGGBBAA", "Set background color")
    .add_option('c', "crop", "", "crop output to the actual objects rendered, not levelsize ");

  for(auto const& opt : argp.parse_args(argc, argv))
  {
    switch (opt.key)
    {
      case 'h':
        argp.print_help();
        exit(EXIT_SUCCESS);
        break;

      case 'c':
        crop = true;
        break;

      case 'b':
        // FIXME: not implemented
        break;

      case argpp::ArgumentType::REST:
        files.push_back(Pathname(opt.argument, Pathname::SYSTEM_PATH));
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

    // render all the objects
    WorldObjRenderer renderer;
    Size size;

    if (System::get_file_extension(files[0].get_raw_path()) == "prefab")
    {
      PrefabFile prefab = PrefabFile::from_path(files[0]);
      renderer.process(prefab.get_objects());
      crop = true;
    }
    else
    {
      PingusLevel plf(files[0]);
      renderer.process(plf.get_objects());
      size = plf.get_size();
    }

    Surface out_surface;

    if (crop)
    {
      geom::irect rect = renderer.get_clip_rect();

      out_surface = Surface(rect.width(), rect.height());

      // FIXME: alpha doesn't work, as the PNG saver can't handle that
      out_surface.fill(Color(255, 255, 255, 255));

      renderer.blit(out_surface, -rect.left(), -rect.top());

      // create a .sprite file to handle the offset
      std::string outfile = System::cut_file_extension(files[1].get_sys_path()) + ".sprite";
      Vector2i offset(rect.left(), rect.top());

      std::ostringstream out;
      Writer writer(out);
      writer.begin_mapping("pingus-sprite");
      writer.write("image", System::cut_file_extension(System::basename(files[0].get_raw_path())) + ".png");
      writer.write("offset", offset);
      writer.end_mapping();
      out << std::endl;
      log_info("writing: %1%", outfile);
      System::write_file(outfile, out.str());
    }
    else
    {
      out_surface = Surface(size.width(), size.height());

      // FIXME: alpha doesn't work, as the PNG saver can't handle that
      out_surface.fill(Color(255, 255, 255, 255));

      renderer.blit(out_surface);
    }

    Screenshot::save_png(files[1].get_sys_path(), out_surface.get_data(),
                         out_surface.get_width(), out_surface.get_height(), out_surface.get_pitch());
    Resource::deinit();
  }

  return 0;
}

/* EOF */
