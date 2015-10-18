#include <iostream>
#include <fstream>

#include "editor/editor_level.hpp"
#include "engine/display/display.hpp"
#include "engine/display/font.hpp"
#include "engine/sound/sound.hpp"
#include "math/size.hpp"
#include "pingus/fonts.hpp"
#include "pingus/globals.hpp"
#include "pingus/path_manager.hpp"
#include "pingus/resource.hpp"
#include "util/log.hpp"
#include "util/raise_exception.hpp"
#include "util/command_line.hpp"

enum class Syntax { FASTJSON, JSON, SEXPR };

struct ResaveOptions
{
  Syntax syntax;
  bool stdout;
  std::vector<std::string> files;

  ResaveOptions() :
    syntax(Syntax::SEXPR),
    stdout(false),
    files()
  {}
};

int main(int argc, char** argv)
{
  CommandLine argp;
  argp.add_usage("FILE...");
  argp.add_doc("Load a level from file and save it to the same file again.\n"
               "Used for converting levels from an old format to a new one.\n");

  argp.add_option('h', "help", "", "Show help text");
  argp.add_option('j', "json", "", "Generate JSON syntax");
  argp.add_option('f', "fastjson", "", "Generate fast JSON syntax");
  argp.add_option('s', "sexpr", "", "Generate SExpression/Lisp syntax");
  argp.add_option('S', "stdout", "", "Write output to stdout");
  argp.parse_args(argc, argv);

  if (argc == 1)
  {
    argp.print_help();
    return 1;
  }

  ResaveOptions opts;
  while(argp.next())
  {
    switch(argp.get_key())
    {
      case 'h':
        argp.print_help();
        return 1;

      case 'j':
        opts.syntax = Syntax::JSON;
        break;

      case 'f':
        opts.syntax = Syntax::FASTJSON;
        break;

      case 's':
        opts.syntax = Syntax::SEXPR;
        break;

      case 'S':
        opts.stdout = true;
        break;

      case CommandLine::REST_ARG:
        opts.files.push_back(argp.get_argument());
        break;
    }
  }

  g_path_manager.set_path("data");
  Resource::init();

  Display::create_window(NULL_FRAMEBUFFER, Size(640, 480), false, false);

  Fonts::init();
  Sound::PingusSound::init();

  for(size_t i = 0; i < opts.files.size(); ++i)
  {
    auto const& filename = opts.files[i];

    try
    {
      std::cout << "Processing: " << filename << " " << i << "/" << opts.files.size() << std::endl;

      std::cout << "Loading: " << filename << std::endl;
      auto level = Editor::EditorLevel::from_level_file(Pathname(filename, Pathname::SYSTEM_PATH));

      std::ostream* stream = nullptr;
      std::ofstream fout;
      if (opts.stdout)
      {
        stream = &std::cout;
      }
      else
      {
        std::cout << "Saving to:  " << filename << std::endl;
        fout.open(filename);
        if (!fout)
        {
          raise_exception(std::runtime_error,  filename << " : failed to open");
        }

        stream = &fout;
      }

      switch(opts.syntax)
      {
        case Syntax::JSON:
          {
            auto writer = Writer::json(*stream);
            level->save_level(writer);
          }
          break;

        case Syntax::FASTJSON:
          {
            auto writer = Writer::fastjson(*stream);
            level->save_level(writer);
          }
          break;

        case Syntax::SEXPR:
          {
            auto writer = Writer::sexpr(*stream);
            level->save_level(writer);
          }
          break;

        default:
          assert(false && "never reached");
          break;
      }
    }
    catch(std::exception const& err)
    {
      log_error("%1%: exception cought: %2%", filename, err.what());
    }
  }

  Sound::PingusSound::deinit();
  Fonts::deinit();
  Resource::deinit();

  return 0;
}

/* EOF */
