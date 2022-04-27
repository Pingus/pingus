#include <iostream>
#include <fstream>

#include <argparser/argparser.hpp>
#include <logmich/log.hpp>

#include "editor/editor_level.hpp"
#include "engine/display/display.hpp"
#include "engine/display/font.hpp"
#include "engine/sound/sound.hpp"
#include "math/size.hpp"
#include "pingus/fonts.hpp"
#include "pingus/globals.hpp"
#include "pingus/path_manager.hpp"
#include "pingus/resource.hpp"
#include "util/raise_exception.hpp"

enum class Syntax { FASTJSON, JSON, SEXPR };

struct ResaveOptions
{
  prio::Format syntax;
  bool stdout;
  std::vector<std::string> files;

  ResaveOptions() :
    syntax(prio::Format::SEXPR),
    stdout(false),
    files()
  {}
};

int main(int argc, char** argv)
{
  argparser::ArgParser argp;
  argp.add_usage(argv[0], "FILE...")
    .add_text("Load a level from file and save it to the same file again.\n"
              "Used for converting levels from an old format to a new one.\n")
    .add_option('h', "help", "", "Show help text")
    .add_option('j', "json", "", "Generate JSON syntax")
    .add_option('f', "fastjson", "", "Generate fast JSON syntax")
    .add_option('s', "sexpr", "", "Generate SExpression/Lisp syntax")
    .add_option('S', "stdout", "", "Write output to stdout");


  auto const& parsed_opts = argp.parse_args(argc, argv);

  if (parsed_opts.size() == 1)
  {
    argp.print_help();
    return 1;
  }

  ResaveOptions opts;
  for(auto const& opt : parsed_opts)
  {
    switch(opt.key)
    {
      case 'h':
        argp.print_help();
        return 1;

      case 'j':
        opts.syntax = prio::Format::JSON;
        break;

      case 'f':
        opts.syntax = prio::Format::FASTJSON;
        break;

      case 's':
        opts.syntax = prio::Format::SEXPR;
        break;

      case 'S':
        opts.stdout = true;
        break;

      case argparser::ArgumentType::REST:
        opts.files.push_back(opt.argument);
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

      auto writer = Writer::from_stream(opts.syntax, *stream);
      level->save_level(writer);
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
