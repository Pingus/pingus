#include <iostream>

#include <argparser/argparser.hpp>

#include "math/color.hpp"
#include "math/size.hpp"
#include "pingus/pingus_level.hpp"
#include "util/pathname.hpp"

int main(int argc, char** argv)
{
  std::vector<Pathname> files;
  enum {
    kAuthor      = (1<<0),
    kName        = (1<<1),
    kChecksum    = (1<<2),
    kDescription = (1<<3),
    kMusic       = (1<<4),
    kFilename    = (1<<5),
    kSize        = (1<<6),
    kActions     = (1<<7)
  };

  unsigned int mode = 0;

  argparser::ArgParser argp;
  argp.add_usage(argv[0], "[OPTIONS]... [FILE]...")

    .add_option('h', "help",    "", "Displays this help")

    .add_option('a', "author", "", "Display author name")
    .add_option('n', "name", "", "Display level name")
    .add_option('s', "size", "", "Display level size")
    .add_option('c', "checksum", "", "Display checksum of the level")
    .add_option('d', "description", "", "Display description of the level")
    .add_option('m', "music", "", "Display music of the level")
    .add_option('A', "actions", "", "Display actions")
    .add_option('f', "filename", "", "Display filename of the level");

  for(auto const& opt : argp.parse_args(argc, argv))
  {
    switch (opt.key)
    {
      case 'h':
        argp.print_help();
        exit(EXIT_SUCCESS);
        break;

      case 'a':
        mode |= kAuthor;
        break;

      case 'A':
        mode |= kActions;
        break;

      case 'n':
        mode |= kName;
        break;

      case 'c':
        mode |= kChecksum;
        break;

      case 'd':
        mode |= kDescription;
        break;

      case 'm':
        mode |= kMusic;
        break;

      case 'f':
        mode |= kFilename;
        break;

      case 's':
        mode |= kSize;
        break;

      case argparser::ArgumentType::REST:
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
    for(auto it = files.begin(); it != files.end(); ++it)
    {
      const Pathname& path = *it;
      PingusLevel plf(path);

      if (mode == 0)
      {
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
        std::cout << "objects       : " << plf.get_objects().get_objects().size() << std::endl;
        std::cout << std::endl;
      }
      else
      {
        if (mode & kFilename)
        {
          std::cout << path << ": ";
        }

        if (mode & kChecksum)
        {
          std::cout << plf.get_checksum() << std::endl;
        }

        if (mode & kName)
        {
          std::cout << plf.get_levelname() << std::endl;
        }

        if (mode & kDescription)
        {
          std::cout << plf.get_description() << std::endl;
        }

        if (mode & kAuthor)
        {
          std::cout << plf.get_author() << std::endl;
        }

        if (mode & kSize)
        {
          std::cout << plf.get_size().width() << "x" << plf.get_size().height() << std::endl;
        }

        if (mode & kMusic)
        {
          std::cout << plf.get_music() << std::endl;
        }

        if (mode & kActions)
        {
          auto actions = plf.get_actions();
          for(auto action = actions.begin(); action != actions.end(); ++action)
          {
            std::cout << action->first << ":" << action->second << " ";
          }
          std::cout << std::endl;
        }
      }
    }
  }

  return 0;
}

/* EOF */
