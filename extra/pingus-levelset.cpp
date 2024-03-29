#include <iostream>
#include <iostream>

#include <argpp/argpp.hpp>

#include "engine/display/display.hpp"
#include "engine/display/screenshot.hpp"
#include "engine/display/surface.hpp"
#include "math/color.hpp"
#include "math/size.hpp"
#include "math/vector2f.hpp"
#include "pingus/globals.hpp"
#include "pingus/levelset.hpp"
#include "pingus/path_manager.hpp"
#include "pingus/pingus_level.hpp"
#include "pingus/res_descriptor.hpp"
#include "pingus/resource.hpp"
#include "pingus/savegame_manager.hpp"
#include "pingus/stat_manager.hpp"
#include "util/pathname.hpp"
#include "util/system.hpp"

using namespace pingus;

int main(int argc, char** argv)
{
  std::vector<Pathname> files;
  enum {
    kTitle       = (1<<0),
    kDescription = (1<<1),
    kLevels      = (1<<2),
    kFilename    = (1<<3)
  };

  unsigned int mode = 0;

  argpp::Parser argp;
  argp.add_usage(argv[0], "[OPTIONS]... [FILE]...")

    .add_option('h', "help",    "", "Displays this help")

    .add_option('t', "title", "", "Display title of the levelset")
    .add_option('d', "description", "", "Display description of the levelset")
    .add_option('l', "levels", "", "Display levels in this levelset")
    .add_option('f', "filename", "", "Display filename of the level");

  for(auto const& opt : argp.parse_args(argc, argv))
  {
    switch (opt.key)
    {
      case 'h':
        argp.print_help();
        exit(EXIT_SUCCESS);
        break;

      case 't':
        mode |= kTitle;
        break;

      case 'd':
        mode |= kDescription;
        break;

      case 'l':
        mode |= kLevels;
        break;

      case 'f':
        mode |= kFilename;
        break;

      case argpp::ArgumentType::REST:
        files.push_back(Pathname(opt.argument, Pathname::SYSTEM_PATH));
        break;
    }
  }

  if (files.empty())
  {
    argp.print_help();
    exit(EXIT_SUCCESS);
  }
  else
  {
    // FIXME: a little ugly that levelset loads sprites and savegames
    System::init_directories();
    g_path_manager.set_path("data/");
    SavegameManager savegame_manager("savegames/savegames.scm");
    StatManager stat_manager("savegames/variables.scm");
    Resource::init();

    Display::create_window(FramebufferType::NULL_FRAMEBUFFER, Size(), false, false);

    for(auto it = files.begin(); it != files.end(); ++it)
    {
      Pathname const& path = *it;
      std::unique_ptr<Levelset> levelset = Levelset::from_file(path);

      if (mode == 0)
      {
        std::cout << "filename      : " << path << std::endl;
        std::cout << "title         : " << levelset->get_title() << std::endl;
        std::cout << "description   : " << levelset->get_description() << std::endl;
        std::cout << "levels        : " << std::endl;
        for(int i = 0; i < levelset->get_level_count(); ++i)
        {
          std::cout << "  " << levelset->get_level(i)->resname << std::endl;
        }
        std::cout << std::endl;
      }
      else
      {
        if (mode & kFilename)
        {
          std::cout << path << ": ";
        }

        if (mode & kTitle)
        {
          std::cout << levelset->get_title() << std::endl;
        }

        if (mode & kDescription)
        {
          std::cout << levelset->get_description() << std::endl;
        }

        if (mode & kLevels)
        {
          for(int i = 0; i < levelset->get_level_count(); ++i)
          {
            std::cout << "  " << levelset->get_level(i)->resname << std::endl;
          }
          std::cout << std::endl;
        }
      }
    }

    Resource::deinit();
  }

  return 0;
}

/* EOF */
