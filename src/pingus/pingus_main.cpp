// Pingus - A free Lemmings clone
// Copyright (C) 1998-2008 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "pingus/pingus_main.hpp"

#include <filesystem>
#include <iostream>
#include <signal.h>

#include <argpp/argpp.hpp>
#include <logmich/log.hpp>
#include <strut/from_string.hpp>

#include "editor/editor_level.hpp"
#include "editor/editor_screen.hpp"
#include "engine/input/driver_factory.hpp"
#include "engine/input/manager.hpp"
#include "engine/system/sdl_system.hpp"
#include "pingus/application.hpp"
#include "pingus/config_manager.hpp"
#include "pingus/event_name.hpp"
#include "pingus/screens/demo_session.hpp"
#include "pingus/screens/level_menu.hpp"
#include "pingus/screens/pingus_menu.hpp"
#include "pingus/worldmap/worldmap_screen.hpp"
#include "util/system.hpp"

#if defined(__APPLE__)
/* Can't use the include, some type names conflict.
   #include <CoreFoundation/CoreFoundation.h>
*/
#include <unistd.h>
#include <strings.h>
#include <sys/stat.h>
extern "C" {
  typedef unsigned char UInt8;
  typedef void* CFTypeRef;
  typedef CFTypeRef CFURLRef;
  CFURLRef CFBundleGetMainBundle(void);
  CFURLRef CFBundleCopyResourcesDirectoryURL(CFTypeRef);
  bool CFURLGetFileSystemRepresentation(CFURLRef, bool, UInt8*, int);
  void CFRelease(CFTypeRef);
}
#endif

#ifdef _WIN32
#  include "win32/console.hpp"
#endif

#include "gettext.h"
#include "tinygettext/dictionary_manager.hpp"
#include "tinygettext/log.hpp"

#include "engine/screen/screen_manager.hpp"
#include "pingus/globals.hpp"
#include "pingus/path_manager.hpp"
#include "pingus/plf_res_mgr.hpp"

#include "engine/sound/sound.hpp"
#include "pingus/resource.hpp"
#include "pingus/savegame_manager.hpp"
#include "pingus/screens/credits.hpp"
#include "pingus/screens/font_test_screen.hpp"
#include "pingus/screens/start_screen.hpp"
#include "pingus/screens/story_screen.hpp"
#include "pingus/stat_manager.hpp"
#include "pingus/worldobj_factory.hpp"

namespace pingus {

PingusMain::PingusMain() :
  cmd_options()
{
}

PingusMain::~PingusMain()
{
}

void
PingusMain::read_rc_file (void)
{
  if (!cmd_options.no_config_file.is_set() ||
      !cmd_options.no_config_file.get())
  {
    std::string filename = System::get_userdir() + "config";

    if (!System::exist(filename))
    {
      log_info("{}: config file not found", filename);
    }
    else
    {
      try
      {
        CommandLineOptions options;
        options.merge(Options::from_file(Pathname(filename, Pathname::SYSTEM_PATH)));
        options.merge(cmd_options);
        cmd_options = options;
      }
      catch(std::exception const& err)
      {
        log_error("{}", err.what());
      }
    }
  }
}

void
PingusMain::apply_args()
{
  // FIXME: merge cmd_options with stuff read from config file here
  auto& options = cmd_options;

  // Mode toggles
  if (options.list_languages.is_set() &&
      options.list_languages.get())
  { // language listing only works after the data path has been set
    std::cout << "Available languages are:" << std::endl;
    std::cout << "========================" << std::endl;
    std::set<tinygettext::Language> lst = dictionary_manager.get_languages();
    for (std::set<tinygettext::Language>::iterator i = lst.begin(); i != lst.end(); ++i)
      std::cout << i->get_name() << " (" << i->str() << ")" << std::endl;

    std::cout << "\nLanguages can be used via:\n\n    pingus --language de\n" << std::endl;

    exit(EXIT_SUCCESS);
  }

  if (options.software_cursor.is_set())
    globals::software_cursor = options.software_cursor.get();

  // Sound
  if (options.disable_music.is_set())
    globals::music_enabled = !options.disable_music.get();

  if (options.disable_sound.is_set())
    globals::sound_enabled = !options.disable_sound.get();

  // Misc
  if (options.language.is_set())
    dictionary_manager.set_language(tinygettext::Language::from_name(options.language.get()));

  if (options.auto_scrolling.is_set())
    globals::auto_scrolling = options.auto_scrolling.get();

  if (options.drag_drop_scrolling.is_set())
    globals::drag_drop_scrolling = options.drag_drop_scrolling.get();

  if (options.developer_mode.is_set())
    globals::developer_mode = options.developer_mode.get();

  if (options.speed.is_set())
    globals::game_speed = options.speed.get();

  if (options.desiredfps.is_set())
    globals::desired_fps = options.desiredfps.get();

  if (options.tile_size.is_set())
    globals::tile_size = options.tile_size.get();
}

void
PingusMain::parse_args(int argc, char** argv)
{
  argpp::Parser argp;

  argp.add_usage(argv[0], _("[OPTIONS]... [FILE]"))
    .add_text(_("Pingus is a puzzle game where you need to guide a bunch of little penguins around the world."));

  argp.add_group(_("General Options:"))
    .add_option('h', "help", "",
                _("Displays this help"))
    .add_option('V', "version", "",
                _("Print version number and exit"))
    .add_option('v', "verbose", "",
                _("Enable info level log output"))
    .add_option('D', "debug", "",
                _("Enable debug level log output"))
    .add_option('Q', "quiet", "",
                _("Disable all log output"));

  argp.add_group(_("Display Options:"))
    .add_option('w', "window", "",
                _("Start in Window Mode"))
    .add_option('f', "fullscreen", "",
                _("Start in Fullscreen"))
    .add_option('r', "renderer", "RENDERER",
                _("Use the given renderer (default: sdl)"))
    .add_option('g', "geometry", "WIDTHxHEIGHT",
                _("Set the window resolution for pingus (default: 800x600)"))
    .add_option('R', "fullscreen-resolution", "WIDTHxHEIGHT",
                _("Set the resolution used in fullscreen mode (default: 800x600)"))
    .add_option(346, {}, "software-cursor", "",
                _("Enable software cursor"));

  argp.add_group(_("Game Options:"))
    .add_option(337, {}, "no-auto-scrolling", "",
                _("Disable automatic scrolling"))
    .add_option(338, {}, "drag-drop-scrolling", "",
                _("Enable drag'n drop scrolling"));

  argp.add_group(_("Sound Options:"))
    .add_option('s', "disable-sound", "",
                _("Disable sound"))
    .add_option('m', "disable-music", "",
                _("Disable music"));

  argp.add_group("Language Options:")
    .add_option('l', "language", "LANG",
                _("Select language to use with Pingus"))
    .add_option(365, {}, "list-languages", "",
                _("List all available languages"));

  argp.add_group("Editor Options:")
    .add_option('e', "editor", "",
                _("Loads the level editor"));

  argp.add_group(_("Directory Options:"))
    .add_option('d', "datadir", _("DIR"),
                _("Load game datafiles from DIR"))
    .add_option('u', "userdir", _("DIR"),
                _("Load config files and store savegames in DIR"))
    .add_option('a', "addon", _("DIR"),
                _("Load game modifications from DIR"))
    .add_option(342, {}, "no-cfg-file", "",
                _("Don't read ~/.pingus/config"))
    .add_option('c', "config", _("FILE"),
                _("Read config options from FILE"))
    .add_option(360, {}, "controller", "FILE",
                _("Uses the controller given in FILE"));

  argp.add_group(_("Debug Options:"))
    .add_option(334, {}, "developer-mode",  "",
                _("Enables some special features for developers"))
    .add_option('t', "speed", "SPEED",
                _("Set the game speed (0=fastest, >0=slower)"))
    .add_option('k', "fps", "FPS",
                _("Set the desired game framerate (frames per second)"))
    .add_option(344, {}, "tile-size", "INT",
                _("Set the size of the map tiles (default: 32)"));

  for(auto const& opt : argp.parse_args(argc, argv))
  {
    switch (opt.key)
    {
      case 'r': // --renderer
        if (opt.argument == "help")
        {
          std::cout << "Available renderers: " << std::endl;
          std::cout << "     sdl: Software rendering" << std::endl;
          std::cout << "  opengl: Hardware accelerated graphics" << std::endl;
          std::cout << "    null: No rendering at all, for debugging" << std::endl;
          exit(EXIT_SUCCESS);
        }
        else
        {
          cmd_options.framebuffer_type.set(FramebufferType_from_string(opt.argument));

          //FIXME:
          //std::cout << "Unknown renderer: " << opt.argument
          //<< " use '--renderer help' to get a list of available renderer" << std::endl;
          //exit(EXIT_FAILURE);
        }
        break;

      case 'e': // -e, --editor
        cmd_options.editor.set(true);
        break;

      case 't': // -t, --set-speed
        cmd_options.speed.set(strut::from_string<int>(opt.argument));
        break;

      case 'k': // -k, --set-fps
        cmd_options.desiredfps.set(strut::from_string<float>(opt.argument));
        break;

      case 's': // -s, --disable-sound
        cmd_options.disable_sound.set(true);
        break;

      case 'm': // -m, --disable-music
        cmd_options.disable_music.set(true);
        break;

      case 'g':
        {
          int w;
          int h;
          if (sscanf(opt.argument.c_str(), "%dx%d", &w, &h) != 2)
          {
            std::cout << "Resolution std::string is wrong, it should be like: \n"
                      << "\"640x480\" or \"800x600\"" << std::endl;
            exit(EXIT_FAILURE);
          }
          cmd_options.geometry.set(Size(w, h));
        }
        break;

      case 'R':
        {
          int w;
          int h;
          if (sscanf(opt.argument.c_str(), "%dx%d", &w, &h) != 2)
          {
            std::cout << "Resolution std::string is wrong, it should be like: \n"
                      << "\"640x480\" or \"800x600\"" << std::endl;
            exit(EXIT_FAILURE);
          }
          cmd_options.fullscreen_resolution.set(Size(w, h));
        }
        break;

      case 'd': // -d, --datadir
        cmd_options.datadir.set(opt.argument);
        break;

      case 'a': // -a, --addon
        g_path_manager.add_overlay_path(opt.argument);
        break;

      case 'u': // -u, --userdir
        cmd_options.userdir.set(opt.argument);
        break;

      case 'V':
        std::cout <<
          "Pingus " PROJECT_VERSION "\n"
          "Copyright (C) 1998-2011 Ingo Ruhnke <grumbel@gmail.com>\n"
          "See the file AUTHORS for a complete list of contributors.\n"
          "Pingus comes with ABSOLUTELY NO WARRANTY. This is free software, and you are\n"
          "welcome to redistribute it under certain conditions; see the file COPYING for details."
                  << std::endl;
        exit(EXIT_SUCCESS);
        break;

      case 'f': // --fullscreen
        cmd_options.fullscreen.set(true);
        break;

      case 'w': // --window
        cmd_options.fullscreen.set(false);
        break;

      case 334: // --developer-mode
        cmd_options.developer_mode.set(true);
        globals::developer_mode = true;
        break;

      case 337:
        cmd_options.auto_scrolling.set(false);
        break;

      case 338:
        cmd_options.drag_drop_scrolling.set(true);
        break;

      case 342: // --no-cfg-file
        cmd_options.no_config_file.set(true);
        break;

      case 344:
        cmd_options.tile_size.set(strut::from_string<int>(opt.argument));
        break;

      case 346:
        cmd_options.software_cursor.set(true);
        break;

      case 'c':
        cmd_options.merge(Options::from_file(Pathname(opt.argument, Pathname::SYSTEM_PATH)));
        break;

      case 'D':
        logmich::set_log_level(logmich::LogLevel::DEBUG);
        break;

      case 'v':
        logmich::set_log_level(logmich::LogLevel::INFO);
        break;

      case 'Q':
        logmich::set_log_level(logmich::LogLevel::NONE);
        break;

      case 360:
        cmd_options.controller.set(opt.argument);
        break;

      case 'l': // language
        cmd_options.language.set(opt.argument);
        break;

      case 365: // list-languages
        cmd_options.list_languages.set(true);
        break;

      case 'h':
        argp.print_help();
        exit(EXIT_SUCCESS);
        break;

      case argpp::ArgumentType::REST:
        if (!cmd_options.rest.is_set())
        {
          cmd_options.rest.set(opt.argument);
        }
        else
        {
          std::cout << "Wrong argument: '" << opt.argument << "'" << std::endl;
          std::cout << "You can only give one file argument," << std::endl;
          exit(EXIT_FAILURE);
        }
        break;

      default:
        std::cout << "Error: Got " << opt.option << " " << opt.argument << std::endl;
        break;
    }
  }
}

// Get all filenames and directories
void
PingusMain::init_path_finder()
{
  if (cmd_options.userdir.is_set())
    System::set_userdir(cmd_options.userdir.get());

  System::init_directories();

  if (cmd_options.datadir.is_set())
  {
    g_path_manager.set_path(cmd_options.datadir.get());
  }
  else
  { // do magic to guess the datadir
#if defined(__APPLE__)
    char path[PATH_MAX];
    getcwd(path, PATH_MAX);
    if (std::filesystem::exists(std::filesystem::path(path) / "data/images/fonts/chalk-40px.font"))
    {
      g_path_manager.set_path("data"); // assume game is run from source dir
    }
    else
    {
      char resource_path[PATH_MAX];
      CFURLRef ref = CFBundleCopyResourcesDirectoryURL(CFBundleGetMainBundle());
      if (!ref || !CFURLGetFileSystemRepresentation(ref, true, (UInt8*)resource_path, PATH_MAX))
      {
        std::cout << "Error: Couldn't get Resources path.\n" << std::endl;
        exit(EXIT_FAILURE);
      }
      CFRelease(ref);
      g_path_manager.set_path(resource_path);
    }
#else
    g_path_manager.set_path("data"); // assume game is run from source dir
#endif
  }

  // Language is automatically picked from env variable
  //dictionary_manager.set_language(tinygettext::Language::from_env("it_IT.utf8")); // maybe overwritten by file ~/.pingus/config
  dictionary_manager.add_directory(g_path_manager.complete("po/"));
}

void
PingusMain::print_greeting_message()
{
  std::string greeting = "Welcome to Pingus " PROJECT_VERSION;
  greeting += "!";
  std::cout <<  greeting << std::endl;
  for (unsigned int i = 0; i < greeting.length(); ++i)
    std::cout.put('=');
  std::cout << std::endl;

  std::cout << "userdir:                 " << System::get_userdir() << std::endl;
  std::cout << "datadir:                 " << g_path_manager.get_path() << std::endl;
  std::cout << "language:                "
            << dictionary_manager.get_language().get_name()
            << " ("
            << dictionary_manager.get_language().str()
            << ")"
            << std::endl;

  if (globals::sound_enabled)
    std::cout << "sound support:           enabled" << std::endl;
  else
    std::cout << "sound support:           disabled" << std::endl;

  if (globals::music_enabled)
    std::cout << "music support:           enabled" << std::endl;
  else
    std::cout << "music support:           disabled" << std::endl;

  std::cout << "fullscreen:              ";
  if (cmd_options.fullscreen.is_set() && cmd_options.fullscreen.get())
  {
    std::cout << cmd_options.fullscreen_resolution.get().width() << "x"
              << cmd_options.fullscreen_resolution.get().height() << std::endl;
  }
  else
  {
    std::cout << "disabled" << std::endl;
  }

  std::cout << std::endl;
}

void
PingusMain::start_game ()
{
  Application app(cmd_options);
  app.run();
}

int
PingusMain::run(int argc, char** argv)
{
#ifdef _WIN32
  win32_redirect_stdio();
#endif

  logmich::set_log_level(logmich::LogLevel::WARNING);

  tinygettext::Log::set_log_info_callback(nullptr);

  try
  {
    // FIXME force set language using System::get_language() to get it from env
    dictionary_manager.set_language(tinygettext::Language::from_env(System::get_language()));

    parse_args(argc, argv); // here language and po dir isn't set, no traslation in command line
    init_path_finder(); // here init language path
    read_rc_file(); // here set language if ~/.pingus/config exist and language value is set
    apply_args(); // here set language if arg -l is specified

    print_greeting_message();

    // init the display
    FramebufferType fbtype = FramebufferType::SDL;
    if (cmd_options.framebuffer_type.is_set())
    {
      fbtype = cmd_options.framebuffer_type.get();
    }

    bool fullscreen = cmd_options.fullscreen.is_set() ? cmd_options.fullscreen.get() : false;
    bool resizable  = cmd_options.resizable.is_set()  ? cmd_options.resizable.get()  : true;

    Size screen_size(1024, 768);
    if (fullscreen)
    {
      if (cmd_options.fullscreen_resolution.is_set())
      {
        screen_size = cmd_options.fullscreen_resolution.get();
      }
    }
    else
    {
      if (cmd_options.geometry.is_set())
      {
        screen_size = cmd_options.geometry.get();
      }
    }

    SDLSystem system;
    try
    {
      system.create_window(fbtype, screen_size, fullscreen, resizable);
    }
    catch(std::exception const& err)
    {
      if (fbtype == FramebufferType::SDL)
      {
        throw;
      }
      else
      {
        log_error("couldn't create window, falling back to SDL: {}", err.what());
        system.create_window(FramebufferType::SDL, screen_size, fullscreen, resizable);
        config_manager.set_renderer(FramebufferType::SDL);
      }
    }

    // init other components
    SavegameManager savegame_manager("savegames/savegames.scm");
    StatManager stat_manager("savegames/variables.scm");

    // FIXME: turn these into RAII
    Resource::init();
    pingus::fonts::init();
    pingus::sound::PingusSound::init();

    config_manager.apply(cmd_options);

    // start and run the actual game
    start_game();
  }
  catch (std::bad_alloc const&)
  {
    std::cout << _("Pingus: Out of memory!") << std::endl;
  }
  catch (std::exception const& a)
  {
    std::cout << _("Pingus: Standard exception caught!:\n") << a.what() << std::endl;
  }
  catch (...)
  {
    std::cout << _("Pingus: Unknown throw caught!") << std::endl;
  }

  pingus::sound::PingusSound::deinit();
  pingus::fonts::deinit();
  WorldObjFactory::deinit();
  Resource::deinit();

  return 0;
}

} // namespace pingus

/* EOF */
