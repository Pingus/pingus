//  Pingus - A free Lemmings clone
//  Copyright (C) 1998-2008 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <config.h>
#include <sys/stat.h>
#include <stdio.h>
#include <signal.h>
#include <locale.h>
#include <iostream>
#include "lisp/lisp.hpp"
#include "lisp/parser.hpp"
#include "editor/editor_level.hpp"
#include "string_util.hpp"
#include "demo_session.hpp"
#include "sexpr_file_reader.hpp"
#include "fonts.hpp"
#include "config_manager.hpp"
#include "display/display.hpp"
#include "pingus_menu.hpp"

#if defined(__APPLE__)
/* Can't use the include, some type names conflict.
#include <CoreFoundation/CoreFoundation.h>
*/
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

#include "SDL.h"

#ifdef ENABLE_BINRELOC
#include "../lib/binreloc/binreloc.h"
#endif 

#include "gettext.h"
#include "tinygettext/dictionary_manager.hpp"
#include "command_line.hpp"

#include "screen/screen_manager.hpp"
#include "path_manager.hpp"
#include "pingus_main.hpp"
#include "globals.hpp"
#include "system.hpp"
#include "pingus_error.hpp"
#include "fps_counter.hpp"
#include "plf_res_mgr.hpp"
#include "game_session.hpp"
#include "story_screen.hpp"

#include "start_screen.hpp"
#include "savegame_manager.hpp"
#include "stat_manager.hpp"
#include "debug.hpp"
#include "resource.hpp"
#include "pingu_action_factory.hpp"
#include "credits.hpp"
#include "sound/sound.hpp"
#include "worldmap/worldmap_screen.hpp"
#include "font_test_screen.hpp"
#include "worldmap/worldmap_screen.hpp"
#include "worldobj_factory.hpp"

#include "editor/editor_screen.hpp"

#if _MSC_VER >= 1400
// Disable stupid deprecation warnings
#pragma warning( disable : 4996 ) 
#endif

extern TinyGetText::DictionaryManager dictionary_manager;

void
signal_handler(int signo)
{
  switch(signo)
    {
    case SIGSEGV:
      puts("\n,------------------------------------------------------------------------");
      puts("| segfault_handler: caught a SIGSEGV.");
      puts("|");
      puts("| Woops, Pingus just crashed, congratulations you've found a bug.");
      puts("| Please write a little bug report to <grumbel@gmx.de>, include information");
      puts("| where exacly the SIGSEGV occurred and how to reproduce it.");
      puts("| Also try to include a backtrace, you can get it by doing:");
      puts("|");
      puts("| $ gdb pingus core");
      puts("| (gdb) bt");
      puts("| ...");
      puts("|");
      puts("| If that doesn't work, try this:");
      puts("|");
      puts("| $ gdb pingus");
      puts("| (gdb) r");
      puts("| [play until it crashes again]");
      puts("| ...");
      puts("|");
      puts("'------------------------------------------------------------------------\n");
      break;

    case SIGINT:
      puts("\n,------------------------------------------------------------------------");
      puts("| Warning: Pingus recieved a SIGINT, exiting now.");
      puts("`------------------------------------------------------------------------\n");
      break;

    default:
      std::cout << "signal_handler (): Got unknown signal: " << signo << std::endl;
      break;
    }
  puts ("exit(EXIT_FAILURE);");
  abort();
  throw "crash";
}

PingusMain::PingusMain() 
{
}

PingusMain::~PingusMain()
{
}

void
PingusMain::read_rc_file (void)
{
  if (cmd_options.no_config_file.is_set() &&
      !cmd_options.no_config_file.get())
    {
      std::string rcfile;

      if (!cmd_options.config_file.is_set())
	rcfile = System::get_userdir() + "config";
      else
	rcfile = cmd_options.config_file.get();

      //constructor of config must be run
      //FIXME: Config config(rcfile);
    }
}

void
PingusMain::apply_args()
{
  // FIXME: merge cmd_options with stuff read from config file here
  PingusOptions& options = cmd_options;

  // Mode toggles
  if (options.list_languages.is_set() &&
      options.list_languages.get())
    { // language listing only works after the data path has been set
      std::cout << "Available languages are:" << std::endl;
      std::cout << "========================" << std::endl;
      std::set<std::string> lst = dictionary_manager.get_languages();
      for (std::set<std::string>::iterator i = lst.begin(); i != lst.end(); ++i)
        std::cout << TinyGetText::get_language_def(*i)->name << " (" << *i << ")" << std::endl;

      std::cout << "\nLanguages can be used via:\n\n    pingus --language de\n" << std::endl; 

      exit(EXIT_SUCCESS);
    }
  
  // Display
  if (options.fullscreen.is_set())
    fullscreen_enabled = options.fullscreen.get();

  if (options.swcursor.is_set())
    swcursor_enabled = options.swcursor.get();

  // FIXME: if (options.geometry.is_set())
  // FIXME:   config_manager.set_resolution(options.geometry.get());

  // Sound
  if (options.disable_music.is_set())
    music_enabled = !options.disable_music.get();

  if (options.disable_sound.is_set())
    sound_enabled = !options.disable_sound.get();

  // Misc
  if (options.language.is_set())
    dictionary_manager.set_current_dictionary(options.language.get());

  if (options.auto_scrolling.is_set())
    auto_scrolling = options.auto_scrolling.get();
  
  if (options.controller.is_set())
    controller_file = options.controller.get();

  if (options.maintainer_mode.is_set())
    maintainer_mode = options.maintainer_mode.get();

  if (options.debug.is_set())
    pingus_debug_flags = options.debug.get();

  if (options.speed.is_set())
    game_speed = options.speed.get();

  if (options.desiredfps.is_set())
    desired_fps = options.desiredfps.get();

  if (options.tile_size.is_set())
    tile_size = options.tile_size.get();
}

void
PingusMain::parse_args(int argc, char** argv)
{
  CommandLine argp;
  argp.add_usage("pingus [OPTIONS]... [FILE]");
  argp.add_doc("Pingus is a puzzle game where you need to guide a bunch of little penguins around the world.");

  argp.add_option('h', "help", "", 
                  _("Displays this help"));
  argp.add_option('v', "version", "", 
                  _("Print version number and exit"));
    
  argp.add_group(_("Display:"));
  argp.add_option('w', "window", "",
                  _("Start in Window Mode"));
  argp.add_option('f', "fullscreen", "",
                  _("Start in Fullscreen"));
  argp.add_option('r', "renderer", "RENDERER",
                  _("Use the given renderer (default: delta)"));
  argp.add_option(346, "enable-swcursor", "",
                  _("Enable software cursor"));
  argp.add_option('g', "geometry", "{width}x{height}",  
                  _("Set the resolution for pingus (default: 800x600)"));

  argp.add_group(_("Sound:"));
  argp.add_option('s', "disable-sound", "", 
                  _("Disable sound"));
  argp.add_option('m', "disable-music", "", 
                  _("Disable music"));

  argp.add_group("Language Options:");
  argp.add_option('l', "language", "LANG",
                  _("Select language to use with Pingus"));
  argp.add_option(365, "list-languages", "",
                  _("List all available languages"));

  argp.add_group("Modes:");
  argp.add_option('e', "editor", "",
                  _("Loads the level editor"));
  argp.add_option('S', "save", "FILENAME",
                  _("Save the level given level to FILENAME and quit"));
  argp.add_option(363, "font", "",
                  _("Test a font"));
  argp.add_option(359, "credits", "",
                  _("Shows the credits"));

  argp.add_group(_("Misc Options:"));
  argp.add_option('d', "datadir", _("PATH"),
                  _("Set the path to load the data files to 'path'"));
  argp.add_option('u', "userdir", _("PATH"),
                  _("Set the path to load and save user files (savegames, etc.) to PATH"));

  argp.add_option(337, "disable-auto-scrolling", "",
                  _("Disable automatic scrolling"));
  argp.add_option(342, "no-cfg-file", "",
                  _("Don't read ~/.pingus/config"));
  argp.add_option(347, "config-file", _("FILE"),
                  _("Read config from FILE (default: ~/.pingus/config) reduce CPU usage, "
                    "might speed up the game on slower machines"));
  argp.add_option(360, "controller", "FILE",
                  _("Uses the controller given in FILE"));

  argp.add_group(_("Debugging and experimental stuff:"));
  argp.add_option(334, "maintainer-mode",  "",  
                  _("Enables some features, only interesting to programmers"));

  argp.add_option('D', "debug",  "OPTION", 
                  _("Enable the output of debugging info, possible "
                    "OPTIONs are tiles, gametime, actions, sound, resources, gui, "
                    "input, pathmgr"));
  argp.add_option('t', "speed", "SPEED",
                  _("Set the game speed (0=fastest, >0=slower)"));
  argp.add_option('k', "fps", "FPS",
		  _("Set the desired game framerate (frames per second)"));
  argp.add_option(344, "tile-size", "INT",
                  _("Set the size of the map tiles (default: 32)"));

  argp.parse_args(argc, argv);
  argp.set_help_indent(20);

  while (argp.next())
    {
      switch (argp.get_key()) 
        {          
          case 'r': // --renderer
            if (argp.get_argument() == "delta")
              {
                framebuffer_type = DELTA_FRAMEBUFFER;
              }
            else if (argp.get_argument() == "opengl")
              {
                framebuffer_type = OPENGL_FRAMEBUFFER;
              }
            else if (argp.get_argument() == "sdl")
              {
                framebuffer_type = SDL_FRAMEBUFFER;
              }
            else if (argp.get_argument() == "help")
              {
                std::cout << "Available renderers: " << std::endl;
                std::cout << "   delta: Software rendering with dirty-rectangles (default)" << std::endl;
                std::cout << "     sdl: Software rendering" << std::endl;
                std::cout << "  opengl: Hardware accelerated graphics" << std::endl;
                exit(EXIT_SUCCESS);
              }
            else
              {
                std::cout << "Unknown renderer: " << argp.get_argument()
                          << " use '--renderer help' to get a list of available renderer" << std::endl;
                exit(EXIT_FAILURE);
              }
            break;

          case 359: // --credits
            cmd_options.credits.set(true);
            break;

          case 'e': // -e, --editor
            cmd_options.editor.set(true);
            break;

          case 't': // -t, --set-speed
            cmd_options.speed.set(StringUtil::to<int>(argp.get_argument()));  
            break;

          case 'k': // -k, --set-fps
	    cmd_options.desiredfps.set(StringUtil::to<float>(argp.get_argument()));
	    break;

          case 's': // -s, --disable-sound
            cmd_options.disable_sound.set(true);
            break;

          case 'S':
            cmd_options.save.set(argp.get_argument());
            break;

          case 'm': // -m, --disable-music
            cmd_options.disable_music.set(true);
            break;
            
          case 'g':
            {
              Size size; 
              if (sscanf(argp.get_argument().c_str(), "%dx%d", &size.width, &size.height) != 2)
                {
                  std::cout << "Resolution std::string is wrong, it should be like: \n"
                            << "\"640x480\" or \"800x600\"" << std::endl;
                  exit(EXIT_FAILURE);
                }
              cmd_options.geometry.set(size);
            }
            break;

          case 'd': // -d, --datadir
            cmd_options.datadir.set(argp.get_argument());
            break;

          case 'u': // -u, --userdir
            cmd_options.userdir.set(argp.get_argument());
            break;

          case 'v':
            std::cout << "Pingus " << VERSION << std::endl;
            std::cout << "\n"
              "Copyright (C) 1998-2008 Ingo Ruhnke <grumbel@gmx.de>\n"
              "See the file AUTHORS for a complete list of contributors.\n\n"
              "There is NO warranty.  You may redistribute this software\n"
              "under the terms of the GNU General Public License.\n"
              "For more information about these matters, see the files named COPYING." << std::endl;
            exit(EXIT_SUCCESS);
            break;
          
          case 'f': // --fullscreen
            cmd_options.fullscreen.set(true);
            break;
          
          case 'w': // --window
            cmd_options.fullscreen.set(false);
            break;

          case 334: // --maintainer-mode
            cmd_options.maintainer_mode.set(true);
            maintainer_mode = true;
            break;

          case 337:
            cmd_options.auto_scrolling.set(false);
            break;

          case 342: // --no-cfg-file
            cmd_options.no_config_file.set(true); 
            break;

          case 344:
            cmd_options.tile_size.set(StringUtil::to<int>(argp.get_argument()));
            break;

          case 346:
            cmd_options.swcursor.set(true);
            break;

          case 347:
            cmd_options.config_file.set(argp.get_argument());
            break;

          case 'D':
            if (argp.get_argument() == "all")
              {
                cmd_options.debug.set(cmd_options.debug.get() | PINGUS_DEBUG_ALL);
              }
            else if (argp.get_argument() == "actions")
              {
                cmd_options.debug.set(cmd_options.debug.get() | PINGUS_DEBUG_ACTIONS);
              }
            else if (argp.get_argument() == "sound")
              {
                cmd_options.debug.set(cmd_options.debug.get() | PINGUS_DEBUG_SOUND);
              }
            else if (argp.get_argument() == "gametime")
              {
                cmd_options.debug.set(cmd_options.debug.get() | PINGUS_DEBUG_GAMETIME);
              }
            else if (argp.get_argument() == "tiles")
              {
                cmd_options.debug.set(cmd_options.debug.get() | PINGUS_DEBUG_TILES);
              }
            else if (argp.get_argument() == "loading")
              {
                cmd_options.debug.set(cmd_options.debug.get() | PINGUS_DEBUG_LOADING);
              }
            else if (argp.get_argument() == "translator")
              {
                cmd_options.debug.set(cmd_options.debug.get() | PINGUS_DEBUG_TRANSLATOR);
              }
            else if (argp.get_argument() == "resources")
              {
                cmd_options.debug.set(cmd_options.debug.get() | PINGUS_DEBUG_RESOURCES);
              }
            else if (argp.get_argument() == "gui")
              {
                cmd_options.debug.set(cmd_options.debug.get() | PINGUS_DEBUG_GUI);
              }
            else if (argp.get_argument() == "input")
              {
                cmd_options.debug.set(cmd_options.debug.get() | PINGUS_DEBUG_INPUT);
              }
            else if (argp.get_argument() == "worldmap")
              {
                cmd_options.debug.set(cmd_options.debug.get() | PINGUS_DEBUG_WORLDMAP);
              }
            else if (argp.get_argument() == "pathmgr")
              {
                cmd_options.debug.set(cmd_options.debug.get() | PINGUS_DEBUG_PATHMGR);
              }
            else
              {
                std::cout << "PingusMain: Unhandled debug flag: " << argp.get_argument() << std::endl;
                exit(EXIT_FAILURE);
              }

            // Update Pingus debug flags
            pingus_debug_flags = cmd_options.debug.get();
            break;

          case 360:
            cmd_options.controller.set(argp.get_argument());
            break;

          case 363: // font test
            cmd_options.font.set(true);
            break;

          case 'l': // language
            cmd_options.language.set(argp.get_argument());
            break;

          case 365: // list-languages
            cmd_options.list_languages.set(true);
            break;

          case 'h':
            argp.print_help();
            exit(EXIT_SUCCESS);
            break;

          case CommandLine::REST_ARG:
            if (!cmd_options.rest.is_set())
              {
                cmd_options.rest.set(argp.get_argument());
              }
            else
              {
                std::cout << "Wrong argument: '" << argp.get_argument() << "'" << std::endl;
                std::cout << "You can only give one file argument," << std::endl;
                exit(EXIT_FAILURE);
              }
            break;

          default:
            std::cout << "Error: Got " << argp.get_key() << " " << argp.get_argument() << std::endl;
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
      path_manager.set_base_path(cmd_options.datadir.get());
    }
  else
    { // do magic to guess the datadir
#if defined(__APPLE__)
      char resource_path[PATH_MAX];
      CFURLRef ref = CFBundleCopyResourcesDirectoryURL(CFBundleGetMainBundle());
      if (!ref || !CFURLGetFileSystemRepresentation(ref, true, (UInt8*)resource_path, PATH_MAX))
        {
          std::cout << "Error: Couldn't get Resources path.\n" << std::endl;
          exit(EXIT_FAILURE);
        }
      CFRelease(ref);
      path_manager.add_path("data");
#elif ENABLE_BINRELOC
      BrInitError error;
      if (br_init(&error) == 0)
        {
          std::cout << "Warning: BinReloc failed to initialize (error code " << error << ")" << std::endl;
          std::cout << "Will fallback to hardcoded default path." << std::endl; 
        }

      char* exe_path = br_find_exe_dir(".");
      path_manager.add_path(exe_path + std::string("/../share/pingus/data/"));
      path_manager.add_path(exe_path + std::string("/data"));
      path_manager.add_path("data"); // assume game is run from source dir, without any magic
      free(exe_path);
#else
      path_manager.add_path("data"); // assume game is run from source dir
#endif

      if (!path_manager.find_path("data/core.res"))
        {
          std::cout << "Error: Couldn't find 'data/core.res' use the --datadir DIR option." << std::endl;
          exit(EXIT_FAILURE);
        }
    }

  // Language is automatically picked from env variable
  // dictionary_manager.set_language("de"); 
  dictionary_manager.add_directory(path_manager.complete("po/"));

  if (maintainer_mode)
    std::cout << "BasePath: " << path_manager.get_base_path () << std::endl;
}

void
PingusMain::print_greeting_message()
{
  std::string greeting = "Welcome to Pingus "VERSION;
  greeting += "!";
  std::cout <<  greeting << std::endl;
  for (unsigned int i = 0; i < greeting.length(); ++i)
    std::cout.put('=');
  std::cout << std::endl;

  std::cout << "data path:               " << path_manager.get_base_path() << std::endl;
  std::cout << "language:                " 
            << dictionary_manager.get_dictionary().get_language()->name 
            << " ("
            << dictionary_manager.get_dictionary().get_language()->code 
            << ")"
            << std::endl;

  if (sound_enabled)
    std::cout << "sound support:           enabled" << std::endl;
  else
    std::cout << "sound support:          disabled" << std::endl;

  if (music_enabled)
    std::cout << "music support:           enabled" << std::endl;
  else
    std::cout << "music support:          disabled" << std::endl;

  // FIXME: std::cout << "resolution:              " 
  // FIXME: << config_manager.get_resolution().width << "x"
  // FIXME:            << config_manager.get_resolution().height << std::endl;
  std::cout << "fullscreen:              "
            << (fullscreen_enabled ? " enabled" : "disabled")
            << std::endl;

  std::cout << std::endl;
}

void
PingusMain::start_game ()
{
  ScreenManager screen_manager;

  if (cmd_options.save.is_set())
    { // Load a level and save it again, useful to convert it in a new format
      if (!cmd_options.rest.is_set())
        {
          std::cout << "Error: level argument required" << std::endl;
        }
      else
        {
          Pathname filename(cmd_options.rest.get(), Pathname::SYSTEM_PATH);
          Editor::EditorLevel level;
          std::cout << "Loading: " << filename.str() << std::endl;
          level.load_level(filename);
          std::cout << "Saving:  " << cmd_options.save.get() << std::endl;
          level.save_level(cmd_options.save.get());
        }
    }
  else if (cmd_options.credits.is_set() && cmd_options.credits.get())
    { // just show the credits screen
      screen_manager.push_screen(new Credits());
    }
  else if (cmd_options.font.is_set() && cmd_options.font.get())
    { // run the Font Test
      if (!cmd_options.rest.is_set())
        {
          std::cout << "Error: FILE argument required" << std::endl;
        }
      else
        {
          Pathname file(cmd_options.rest.get(), Pathname::SYSTEM_PATH);
          screen_manager.push_screen(new FontTestScreen(file)); 
        }
    }
  else if (cmd_options.editor.is_set() && cmd_options.editor.get())
    { // Editor
      Editor::EditorScreen* editor = new Editor::EditorScreen();
      // optionally load a map in the editor if it was given
      if (cmd_options.rest.is_set())
        editor->load(Pathname(cmd_options.rest.get(), Pathname::SYSTEM_PATH));

      screen_manager.push_screen(editor);
    }
  else if (cmd_options.rest.is_set())
    { // just start the map that was passed on the command line
      if (StringUtil::has_suffix(cmd_options.rest.get(), ".pingus-demo"))
        { // Demo file
          screen_manager.push_screen
            (new DemoSession(Pathname(cmd_options.rest.get(), Pathname::SYSTEM_PATH)));
        }
      else
        { // Level file
          screen_manager.push_screen
            (new StartScreen(PLFResMgr::load_plf_from_filename(Pathname(cmd_options.rest.get(),
                                                                        Pathname::SYSTEM_PATH))));
        }
    }
  else // start a normal game
    {
      pout(PINGUS_DEBUG_LOADING) << "starting normal game" << std::endl;
      screen_manager.push_screen(new PingusMenu());
      pout(PINGUS_DEBUG_LOADING) << "done: starting normal game" << std::endl;
    }

  screen_manager.display();
}

int
PingusMain::main(int argc, char** argv)
{
   // Register the segfault_handler
#ifndef WIN32
  signal(SIGSEGV, signal_handler);
#endif
  //signal(SIGINT, signal_handler);

  // Init error/warning/notice streams
  pout.add(std::cout);
  pwarn.add(std::cout);
  perr.add(std::cout);

  try
    {
      parse_args(argc, argv);
      init_path_finder();
      read_rc_file();
      apply_args();

      print_greeting_message();

      init_sdl();
      init_pingus();

      // start and run the actual game
      start_game();
    }

  catch (const PingusError& err) {
    std::cout << _("Error caught from Pingus: ") << err.get_message () << std::endl;
  }

  catch (const std::bad_alloc&) {
    std::cout << _("Pingus: Out of memory!") << std::endl;
  }

  catch (const std::exception& a) {
    std::cout << _("Pingus: Standard exception caught!:\n") << a.what() << std::endl;
  }

  catch (...) {
    std::cout << _("Pingus: Unknown throw caught!") << std::endl;
  }

  deinit_pingus();

  return 0;
}

void
PingusMain::init_sdl()
{
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) != 0) {
    std::cout << "Unable to initialize SDL: " << SDL_GetError() << std::endl;
    exit(1);
  }
  atexit(SDL_Quit); 

  Size screen_size(800,600);
  if (cmd_options.geometry.is_set())
    screen_size = cmd_options.geometry.get();

  Display::set_video_mode(screen_size, fullscreen_enabled);

  SDL_WM_SetCaption("Pingus " VERSION " - SDL Edition", 0 /* icon */);

  SDL_EnableUNICODE(1);
}

void
PingusMain::deinit_sdl()
{
}

void
PingusMain::on_exit_press()
{
  std::cout << "Exit pressed" << std::endl;
  ScreenManager::instance()->pop_all_screens();
}

void
PingusMain::init_pingus()
{
  SavegameManager::instance();
  StatManager::init();
  Resource::init();
  Fonts::init();
  Sound::PingusSound::init();
  PinguActionFactory::init();
}

void
PingusMain::deinit_pingus()
{
  Fonts::deinit();
  PinguActionFactory::deinit();
  Sound::PingusSound::deinit();
  WorldObjFactory::deinit();
  StatManager::deinit();
  SavegameManager::deinit();
  Resource::deinit();
}

int main(int argc, char** argv)
{
  PingusMain app;
  return app.main(argc, argv);
}

/* EOF */
