//   $Id: pingus_main.cxx,v 1.104 2004/04/01 15:18:05 torangan Exp $
//    ___
//   | _ \                         |      ,--.    |
//   |   /_  _ _  ___  _   _  ___  |   ,-/ o  \   |
//   |  || || \ || _ || |_| ||_ -' |   `-.    /   |
//   |__||_||_\_||_  ||_____||___| |     /\   \   |
//                _| |             |    |  |_| |  |
//               |___|             |    o\/____\  |
//
//   Copyright (C) 1998 Ingo Ruhnke <grumbel@gmx.de>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program; if not, write to the Free Software
//   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#include <config.h>
#include <sys/stat.h>
#include <stdio.h>
#include <signal.h>
#include <locale.h>
#include <iostream>
#include <physfs.h>
#include "lisp/lisp.hpp"
#include "lisp/parser.hpp"
#include "sexpr_file_reader.hpp"

#include "SDL.h"

//#include <ClanLib/display.h>
//#include <ClanLib/sound.h>
//#include <ClanLib/core.h>
//#include <ClanLib/sdl.h>
//#include <ClanLib/gl.h>
//#include <ClanLib/gui.h>

#include "gettext.h"

#include "command_line.hpp"

#include "gui/screen_manager.hxx"
#include "dummy_screen.hxx"
// #include "gui/input_debug_screen.hxx"
#include "path_manager.hxx"
#include "pingus_main.hxx"
#include "globals.hxx"
#include "system.hxx"
#include "pingus_error.hxx"
// #include "global_event.hxx"
#include "config.hxx"
#include "console.hxx"
// #include "fps_counter.hxx"
// #include "plf_res_mgr.hxx"
// #include "game_session.hxx"
// #include "story_screen.hxx"
// #include "start_screen.hxx"
// #include "savegame_manager.hxx"
// #include "stat_manager.hxx"
// #include "demo_session.hxx"
#include "debug.hxx"
//#include "fonts.hxx"
#include "pingus_menu_manager.hxx"
#include "resource.hxx"
// #include "pingu_action_factory.hxx"
// #include "credits.hxx"
#include "sound/sound.hxx"
// #include "worldmap/manager.hxx"
#include "cheat.hxx"
// #include "blitter_test.hxx"
// #include "preview_renderer.hxx"
// #include "worldmap/manager.hxx"
// #include "worldobj_factory.hxx"

// #include "editor/editor_screen.hxx"

#if _MSC_VER >= 1400
// Disable stupid deprecation warnings
#pragma warning( disable : 4996 ) 
#endif


void
signal_handler(int signo)
{
  switch(signo)
    {
    case SIGSEGV:
      puts("\n,------------------------------------------------------------------------");
      puts(_("| segfault_handler: catched a SIGSEGV."));
      puts  ("|");
      puts(_("| Woops, Pingus just crashed, congratulations you've found a bug."));
      puts(_("| Please write a little bug report to <grumbel@gmx.de>, include informations"));
      puts(_("| where exacly the SIGSEGV occured and how to reproduce it."));
      puts(_("| Also try include a backtrace, you can get it like this:"));
      puts  ("|");
      puts  ("| $ gdb pingus core");
      puts  ("| (gdb) bt");
      puts  ("| ...");
      puts  ("|");
      puts(_("| If that doesn't work, try this:"));
      puts  ("|");
      puts  ("| $ gdb pingus");
      puts  ("| (gdb) r");
      puts(_("| [play until it crashes again]"));
      puts  ("| ...");
      puts  ("|");
      puts  ("'------------------------------------------------------------------------\n");
      break;

    case SIGINT:
      puts("\n,------------------------------------------------------------------------");
      puts (_("| Warning: Pingus recieved a SIGINT, exiting now."));
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

PingusMain::PingusMain() :
  blitter_test(false),
  show_credits(false),
  editor(false),
  refresh_rate(60)
{
}

PingusMain::~PingusMain()
{
}

char*
PingusMain::get_title()
{
#ifdef OFFICIAL_PINGUS_BUILD
  static char title[] = "Pingus "VERSION" - http://pingus.seul.org";
#else
  static char title[] = "Pingus "VERSION" (unofficial build) - http://pingus.seul.org";
#endif
  return title;
  //return (string(PACKAGE) + " " + VERSION + " - http://pingus.seul.org").c_str();
}

void
PingusMain::read_rc_file (void)
{
  if (!no_config_file)
    {
      std::string rcfile;

      if (config_file.empty())
	rcfile = System::get_statdir() + "config";
      else
	rcfile = config_file;

      //constructor of config must be run
      Config config(rcfile);
    }
}

// Checking for all options, which needs to be known *before* the
// config file is read.
void
PingusMain::quick_check_args(int argc, char** argv)
{
  no_config_file = false;
  for(int i=1; i < argc; ++i)
    {
      if (strcmp(argv[i], "--no-cfg-file") == 0)
	{
	  no_config_file = true;
	}
    }
}

// check_ars() checks the command line for options and set the
// corresponding global variables to the set values.
void
PingusMain::check_args(int argc, char** argv)
{
#ifdef WIN32
  cursor_enabled = true;
#endif

  CommandLine argp;
  argp.add_usage("pingus [OPTIONS]... [FILES]...");
  argp.add_doc("Pingus is a puzzle game where you need to guide a bunch of little penguins around the world.");
    
  argp.add_group(_("Options:"));
  argp.add_option('g', "geometry", "{width}x{height}",  
                  _("Set the resolution for pingus (default: 800x600)"));
  argp.add_option('h', "help", "", 
                  _("Displays this help"));
  argp.add_option('n', "disable-intro", "", 
                  _("Disable intro"));
  argp.add_option('G', "use-opengl", "",
                  _("Use OpenGL"));
  argp.add_option('S', "use-sdl", "",
                  _("Use SDL"));
  argp.add_option('w', "window", "",
                  _("Start in Window Mode"));
  argp.add_option('f', "fullscreen", "",
                  _("Start in Fullscreen"));
  argp.add_option('R', "refresh-rate", "",
                  _("Set the refresh rate in fullscreen (default: 60)"));                  
  argp.add_option('d', "datadir", _("PATH"),
                  _("Set the path to load the data files to 'path'"));
  argp.add_option('l', "level",  _("FILE"), 
                  _("Load a custom level from FILE"));
  argp.add_option(358, "worldmap", _("FILE"),
                  _("Load a custom worldmap from FILE"));
  argp.add_option('e', "editor", "",
                  _("Loads the level editor"));
  argp.add_option('v', "verbose", "", 
                  _("Print some more messages to stdout, can be set multiple times to increase verbosity"));
  argp.add_option('V', "version", "", 
                  _("Prints version number and exit"));
  argp.add_option(337, "disable-auto-scrolling", "",
                  _("Disable automatic scrolling"));
  argp.add_option(346, "enable-swcursor", "",
                  _("Enable software cursor"));
  argp.add_option(342, "no-cfg-file", "",
                  _("Don't read ~/.pingus/config"));
  argp.add_option(347, "config-file", "", _("FILE"), 
                  _("Read config from FILE (default: ~/.pingus/config) reduce CPU usage, "
                    "might speed up the game on slower machines"));
  argp.add_option(360, "controller", "FILE",
                  _("Uses the controller given in FILE"));

  argp.add_group(_("Debugging and experimental stuff:"));
  argp.add_option(334, "maintainer-mode",  "",  
                  _("Enables some features, only interesting programmers"));
  argp.add_option(352, "debug",  "OPTION", 
                  _("Enable the output of debugging infos, possible"
                    "OPTION's are tiles, gametime, actions, sound, resources, gui,"
                    "input, pathmgr"));
  argp.add_option(354, "min-frame-skip", "N",
                  _("Skip at least N frames, larger values speed the game up"));
  argp.add_option(355, "max-frame-skip", "N",
                  _("Skip at most N frames"));
  argp.add_option(357, "frame-skip",  "N",
                  _("Set both min and max frameskip to N"));
  argp.add_option('t', "speed", "SPEED",
                  _("Set the game speed (0=fastest, >0=slower)"));
  argp.add_option('b', "print-fps", "",
                  _("Prints the fps to stdout"));
  argp.add_option(344, "tile-size", "INT",
                  _("Set the size of the map tiles (default: 32)"));
  argp.add_option(332, "fast-mode", "",
                  _("Disable some cpu intensive features"));
  argp.add_option(353, "min-cpu-usage", "",
                  _("Reduces the CPU usage by issuing sleep()"));
  argp.add_option(359, "show-credits", "",
                  _("Shows the credits"));

  argp.add_group(_("Demo playing and recording:"));
  argp.add_option('p', "play-demo", _("FILE"), 
                  _("Plays a demo session from FILE"));
  argp.add_option('r', "disable-demo-recording", "",
                  _("Record demos for each played level"));

  argp.add_group(_("Sound:"));
  argp.add_option('s', "disable-sound", "", 
                  _("Disable sound"));
  argp.add_option('m', "disable-music", "", 
                  _("Disable music"));

  argp.parse_args(argc, argv);
  argp.set_help_indent(20);
  
  while (argp.next())
    {
      switch (argp.get_key()) 
        {
        case 'c': // -c, --enable-cursor
          cursor_enabled = true;
          if (verbose) std::cout << "PingusMain:check_args: Cursor enabled" << std::endl;
          break;
            
        case 'b': // -b, --print-fps
          print_fps = true;
          if (verbose) std::cout << "PingusMain:check_args: Printing fps enabled" << std::endl;
          break;

        case 358: // --worldmap
          worldmapfile = argp.get_argument();
          break;

        case 359: // --worldmap
          show_credits = true;
          break;
            
        case 'l': // -l, --level
          levelfile = argp.get_argument();
          break;
          
        case 'e': // -e, --editor
          editor = true;

        case 't': // -t, --set-speed
          game_speed = atoi(argp.get_argument().c_str());
          break;

        case 'G':
          use_opengl = true;
          break;

        case 'S':
          use_opengl = false;
          break;

        case 's': // -s, --disable-sound
          sound_enabled = false;
          break;
            
        case 'g':
          {
            char c;
            if (sscanf(argp.get_argument().c_str(), "%d%c%d", &screen_width, &c, &screen_height) != 3 && c != 'x')
              {
                std::cout << "Resolution std::string is wrong, it should be like: \n"
                          << "\"640x480\" or \"800x600\"" << std::endl;
                exit(EXIT_FAILURE);
              }
            if (screen_width > 800 || screen_height > 600)
              {
                std::cout << _("Warning: Larger resolution than 800x600 will result in visual problems") << std::endl;
              }
          }
          break;

        case 'm': // -m, --disable-music
          music_enabled = false;
          break;

        case 'd': // -d, --datadir
          path_manager.add_path(argp.get_argument());
            
          if (verbose)
            std::cout << "check_args: Pingus Data Dir = "
                      << argp.get_argument() << std::endl;
          break;

        case 'V':
          std::cout << "Pingus Version " << VERSION
#ifndef OFFICIAL_PINGUS_BUILD
                    << " (unofficial build)"
#endif
                    << std::endl;
            
          std::cout << "\n"
            "Copyright (C) 2003 Ingo Ruhnke <grumbel@gmx.de>\n"
            "There is NO warranty.  You may redistribute this software\n"
            "under the terms of the GNU General Public License.\n"
            "For more information about these matters, see the files named COPYING." << std::endl;
            
          exit(EXIT_SUCCESS);
          break;
        case 'r': // -r, --enabled-demo-recording
          enable_demo_recording = false;
          break;
        case 'p': // -p, --play-demo
          play_demo = true;
          demo_file = argp.get_argument();
          if (verbose)
            std::cout << "Using demofile: " << demo_file << std::endl;
          break;
        case 'v':
          sscanf(argp.get_argument().c_str(), "%d", &verbose);
          std::cout << "Pingus: Verbose level is " << verbose << std::endl;
          break;

        case 'f': // --fullscreen
          fullscreen_enabled = true;
          break;

        case 'R': // --refresh-rate
          sscanf(argp.get_argument().c_str(), "%d", &refresh_rate);
          std::cout << "Pingus: Refresh rate is " << refresh_rate << std::endl;
          break;
          
        case 'w': // --window
          fullscreen_enabled = false;
          break;

          // Starting weird number options... no idea if this is correct.
        case 332:
          fast_mode = true;
          break;
        case 334: // --maintainer_mode
          std::cout << "---------------------------------" << std::endl
                    << "--- Maintainer Mode activated ---" << std::endl
                    << "---------------------------------" << std::endl;
          maintainer_mode = true;
          break;

        case 337:
          auto_scrolling = false;
          break;
        case 342: // --no-cfg-file
          // Nothing, since that is handled in quick_check_args()
          break;

        case 344:
          sscanf(argp.get_argument().c_str(), "%d", &tile_size);
          break;

        case 345:
          swcursor_enabled = false;
          break;

        case 346:
          swcursor_enabled = true;
          break;

        case 347:
          config_file = argp.get_argument();
          break;

        case 352:
          if (argp.get_argument() == "all")
            {
              pingus_debug_flags |= PINGUS_DEBUG_ALL;
            }
          else if (argp.get_argument() == "actions")
            {
              pingus_debug_flags |= PINGUS_DEBUG_ACTIONS;
            }
          else if (argp.get_argument() == "sound")
            {
              pingus_debug_flags |= PINGUS_DEBUG_SOUND;
            }
          else if (argp.get_argument() == "gametime")
            {
              pingus_debug_flags |= PINGUS_DEBUG_GAMETIME;
            }
          else if (argp.get_argument() == "tiles")
            {
              pingus_debug_flags |= PINGUS_DEBUG_TILES;
            }
          else if (argp.get_argument() == "loading")
            {
              pingus_debug_flags |= PINGUS_DEBUG_LOADING;
            }
          else if (argp.get_argument() == "translator")
            {
              pingus_debug_flags |= PINGUS_DEBUG_TRANSLATOR;
            }
          else if (argp.get_argument() == "resources")
            {
              pingus_debug_flags |= PINGUS_DEBUG_RESOURCES;
            }
          else if (argp.get_argument() == "gui")
            {
              pingus_debug_flags |= PINGUS_DEBUG_GUI;
            }
          else if (argp.get_argument() == "input")
            {
              pingus_debug_flags |= PINGUS_DEBUG_INPUT;
            }
          else if (argp.get_argument() == "worldmap")
            {
              pingus_debug_flags |= PINGUS_DEBUG_WORLDMAP;
            }
          else if (argp.get_argument() == "pathmgr")
            {
              pingus_debug_flags |= PINGUS_DEBUG_PATHMGR;
            }
          else
            {
              std::cout << "PingusMain: Unhandled debug flag: " << argp.get_argument() << std::endl;
              exit(EXIT_FAILURE);
            }

          break;

        case 353:
          max_cpu_usage = false;
          break;

        case 354:
          sscanf(argp.get_argument().c_str(), "%d", &min_frame_skip);
          break;

        case 355: // max_frame_skip
          sscanf(argp.get_argument().c_str(), "%d", &max_frame_skip);
          break;

        case 357: // frame_skip
          sscanf(argp.get_argument().c_str(), "%d", &max_frame_skip);
          min_frame_skip = max_frame_skip;
          break;

        case 356: // Cheats
          Cheat::activate(argp.get_argument());
          break;

        case 360:
          controller_file = argp.get_argument();
          break;

        case 361:
          std::cout << "Rendering a Level Preview..." << std::endl;
          render_preview = true;
          preview_file   = argp.get_argument();
          break;

        case 362: // Blitter test
          blitter_test = true;
          break;

        case 'h':
          argp.print_help();
          exit(EXIT_SUCCESS);
          break;

        case CommandLine::REST_ARG:
          if (levelfile.empty()) 
            {
              levelfile = argp.get_argument();
            
              if (!System::exist(levelfile))
                {
                  std::cout << "PingusMain: " << levelfile << " not found" << std::endl;
                  exit (EXIT_FAILURE);
                }
            } 
          else 
            {
              std::cout << "Wrong argument: '" << argp.get_argument() << "'" << std::endl;
              std::cout << "A levelfile is already given," << std::endl;
              exit(EXIT_FAILURE);
            }
          break;

        default:
          std::cout << "Error: Got " << argp.get_key() << " " << argp.get_argument() << std::endl;
          break;
        }
    }

  // make sure that we're not recording a demo while already playing one
  if (play_demo)
    enable_demo_recording = false;
}

// Get all filenames and directories
void
PingusMain::init_path_finder()
{
  System::init_directories();

  if (maintainer_mode)
    std::cout << "Directory name of " << executable_name << " - " << System::dirname(executable_name)
              << std::endl;

#ifdef __APPLE__
  char resource_path[PATH_MAX];
  CFURLRef ref = CFBundleCopyResourcesDirectoryURL(CFBundleGetMainBundle());
  if (!ref || !CFURLGetFileSystemRepresentation(ref, true, (UInt8*)resource_path, PATH_MAX))
    {
      std::cout << "Error: Couldn't get Resources path.\n" << std::endl;
      exit(EXIT_FAILURE);
    }
  CFRelease(ref);
  //path_manager.add_path(CL_String::get_path(std::string(resource_path) + "/data/"));
#else
  //path_manager.add_path(CL_String::get_path(CL_System::get_exe_path() + "/data/"));
  //path_manager.add_path(CL_String::get_path(CL_System::get_exe_path() + "/../data/"));
  //path_manager.add_path(CL_String::get_path(CL_System::get_exe_path() + "/../share/games/pingus/"));
#endif

  std::list<std::string> file_list;
  file_list.push_back ("data/core.xml");

  //   if (!path_manager.find_path (file_list))
  //     {
  //       std::cout << "Error: Couldn't find 'data/core.xml', please set the enviroment variable\n"
  //                 << "PINGUS_DATADIR to the path of the file `data/core.scr' or use the\n"
  //                 << "-d option." << std::endl;
  //       exit(EXIT_FAILURE);
  //     }

  dictionary_manager.add_directory(path_manager.complete("po/"));
  // Language is automatically picked from env variable
  // dictionary_manager.set_language("de"); 

  const char* lang = getenv("LC_ALL");
  if(!lang) lang = getenv("LC_MESSAGES");
  if(!lang) lang = getenv("LANG");
  if(lang)
    {
      std::string language(lang);
      language.resize(2);
      if(language == "cs" || language == "sr")
        {
          dictionary_manager.set_charset("ISO-8859-2");
          //Pingus::Fonts::encoding = "ISO-8859-2";
        }
      else if(language == "tr")
        {
          dictionary_manager.set_charset("ISO-8859-9");
          //Pingus::Fonts::encoding = "ISO-8859-9";
        }
      else
        {
          dictionary_manager.set_charset("ISO-8859-1");
          //Pingus::Fonts::encoding = "ISO-8859-1";
        }
    }

  if (maintainer_mode)
    std::cout << "BasePath: " << path_manager.get_base_path () << std::endl;
}

void
PingusMain::print_greeting_message()
{
  std::string greeting = "Welcome to Pingus "VERSION;
#ifndef OFFICIAL_PINGUS_BUILD
  greeting += " (unofficial build)";
#endif
  greeting += "!";
  std::cout <<  greeting << std::endl;
  for (unsigned int i = 0; i < greeting.length(); ++i)
    std::cout.put('=');
  std::cout << std::endl;

#ifdef HAVE_LIBCLANVORBIS
  std::cout << _("clanVorbis support:           ok") << std::endl;
#else
  std::cout << _("clanVoribs support:  missing (.ogg music files will not be playable)") << std::endl;
#endif

#ifdef HAVE_LIBCLANMIKMOD
  std::cout << _("clanMikMod support:           ok") << std::endl;
#else
  std::cout << _("clanMikMod support:  missing (music files will not be playable)") << std::endl;
#endif

#ifdef HAVE_GETTEXT
  std::cout << _("getext support:               ok") << std::endl;
  std::cout << _("gettext language:        english") << std::endl;
#else
  std::cout << "getext support: missing (only support for english will be available)" << std::endl;
#endif

  if (sound_enabled)
    std::cout << _("sound support:           enabled") << std::endl;
  else
    std::cout << _("sound support:          disabled") << std::endl;

  if (music_enabled)
    std::cout << _("music support:           enabled") << std::endl;
  else
    std::cout << _("music support:          disabled") << std::endl;

  std::cout << _("resolution set to:       ") << screen_width << "x" << screen_height << std::endl;
  std::cout << _("fullscreen:              ")
            << (fullscreen_enabled ? _(" enabled") : _("disabled"))
            << std::endl;
  std::cout << _("refresh rate:            ") << refresh_rate << std::endl;
  std::cout << _("using OpenGL:            ") << use_opengl << std::endl;

  std::cout << std::endl;
}

void
PingusMain::start_game ()
{
  { // SExpr Parser Test code
    std::cout << "Parser Test" << std::endl;
    lisp::Lisp* sexpr = lisp::Parser::parse("test.scm");
    std::cout << "Parser Test..." << std::endl;
    std::cout << sexpr->get_type() << " " << sexpr->get_list_size() << std::endl;
    sexpr = sexpr->get_list_elem(0);
    if (sexpr)
      {
        SExprFileReader reader(sexpr);
    
        int t = 0; 
        std::string str;
        reader.read_int("test", t);
        reader.read_string("teststr", str);

        std::cout << reader.get_name() << ": t == " << t << " str: " << str << std::endl;
      }
    else
      {
        std::cout << "Not found" << std::endl;
      }
  }

  if (verbose) {
    pout << _("PingusMain: Starting Main: ") << SDL_GetTicks() << std::endl;
  }

  //if (print_fps)
  //    Display::add_flip_screen_hook(&fps_counter);

  if (!render_preview)
    {
      // Register the global event catcher
      //on_button_press_slot   = window->get_ic()->get_keyboard().sig_key_down().connect (&global_event, &GlobalEvent::on_button_press);
      //on_button_release_slot = window->get_ic()->get_keyboard().sig_key_up().connect (&global_event, &GlobalEvent::on_button_release);
    }

  // Set the root screen
  if (show_input_debug_screen) // show a debug screen
    {
      //ScreenManager::instance()->push_screen(new InputDebugScreen (), true);
    }
  else if (render_preview)
    {
      if (levelfile.empty())
        {
          PingusError::raise("You need to give a level file to render a preview");
        }
      else
        {
          //PreviewRenderer::render(PLFResMgr::load_plf_from_filename(levelfile),
          //                        preview_file);
        }
    }
  else if (show_credits)
    {
      //ScreenManager::instance()->push_screen(Credits::instance(), false);
    }
  else if (!levelfile.empty ()) 
    {
      bool successfull = true;
      if (!System::exist(levelfile))
        {
          if (System::exist(levelfile + ".xml"))
            levelfile += ".pingus";
          else if (System::exist("levels/" + levelfile + ".pingus"))
            levelfile = "levels/" + levelfile + ".pingus";
          else
            {
              pout << _("PingusMain: Levelfile not found, ignoring: ") << levelfile << std::endl;
              successfull = false;
            }
        }

      if (successfull)
        {
          //ScreenManager::instance()->push_screen
            //(new StartScreen(PLFResMgr::load_plf_from_filename(levelfile)),
             //   true);
        }
    }
  else if (!demo_file.empty()) // start a demo
    {
      //ScreenManager::instance()->push_screen(new DemoSession (demo_file));
    }
  else if (!worldmapfile.empty())
    {
      //WorldMapNS::WorldMapManager::instance()->load(worldmapfile);
      //ScreenManager::instance()->push_screen(WorldMapNS::WorldMapManager::instance());
    }
  else if (editor == true)
    {
      //ScreenManager::instance()->push_screen (new Editor::EditorScreen());
    }
  else // start a normal game
    {
      std::cout << "starting normal game" << std::endl;
      ScreenManager::instance()->push_screen(PingusMenuManager::instance (), false);
      ////ScreenManager::instance()->push_screen(new StoryScreen(), true);
      //ScreenManager::instance()->push_screen(new DummyScreen(), true);
      std::cout << "done: starting normal game" << std::endl;
    }

  if (!render_preview)
    {
      // show the main menu, the rest of the game is spawn from there
      if (maintainer_mode)
        std::cout << "PingusMain::start screen manager" << std::endl;
      ScreenManager::instance()->display();
      if (maintainer_mode)
        std::cout << "PingusMain::quit game and screen_manager" << std::endl;

      // unregister the global event catcher
      ////window->get_ic()->get_keyboard().sig_key_down().disconnect(on_button_press_slot);
      ////window->get_ic()->get_keyboard().sig_key_up().disconnect(on_button_release_slot);
    }
}

int
PingusMain::main(int argc, char** argv)
{
  PHYSFS_init(argv[0]);
  PHYSFS_addToSearchPath("data/", 0);

  executable_name = argv[0];

  // Register the segfault_handler
#ifndef WIN32
  signal(SIGSEGV, signal_handler);
#endif
  //signal(SIGINT, signal_handler);

  // Init error/warning/notice streams
  pout.add (std::cout);
  pout.add (console);
  pwarn.add (std::cout);
  pout.add (console);
  perr.add (std::cout);
  perr.add (console);

  try
    {
      init_path_finder();

      quick_check_args(argc, argv);
      read_rc_file();
      check_args(argc, argv);

      print_greeting_message();

      init_sdl();
      init_pingus();

      // Avoid uglyness on window opening
      if (!render_preview)
        {
          ////CL_Display::clear();
          ////CL_Display::flip();
        }

      if (blitter_test)
        {
          ////BlitterTest test;
          ////test.run();
        }
      else
        {
          start_game();
        }
    }

  ////catch (const CL_Error& err) {
  ////std::cout << _("Error caught from ClanLib: ") << err.message << std::endl;
////}

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
  deinit_clanlib();

  PHYSFS_deinit();

  return 0;
}

void
PingusMain::init_sdl()
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cout << "Unable to initialize SDL: " << SDL_GetError() << std::endl;
    exit(1);
  }
  atexit(SDL_Quit); 
  Display::set_video_mode(screen_width, screen_height);
  SDL_WM_SetCaption(PACKAGE_STRING " - SDL Edition", 0 /* icon */);
}

void
PingusMain::deinit_sdl()
{
}

void
PingusMain::init_clanlib()
{
#if 0
  if (render_preview)
    {
      CL_SetupCore::init ();
      // Register only the resource types
      CL_SetupDisplay::init(true);
    }
  else
    {
      CL_SetupCore::init ();

      if (use_opengl) CL_SetupGL::init();
      else            CL_SetupSDL::init();

      CL_SetupDisplay::init();

      if (verbose) {
        std::cout << "Using resolution: "
                  << screen_width << "x" << screen_height << std::endl;
      }

      CL_DisplayWindowDescription window_desc;
      window_desc.set_size(CL_Size(screen_width, screen_height));
      window_desc.set_title(PACKAGE_STRING);
      window_desc.set_fullscreen(fullscreen_enabled);
      window_desc.set_allow_resize(false);
      window_desc.set_refresh_rate(refresh_rate);
      
      window = new CL_DisplayWindow(window_desc);
     
  //CL_Display::clear();
  //  CL_Display::flip();
    }
  CL_SetupGUI::init ();
  
  on_exit_press_slot = window->sig_window_close().connect(this, &PingusMain::on_exit_press);
#endif 
}

void
PingusMain::on_exit_press()
{
  std::cout << "Exit pressed" << std::endl;
  ScreenManager::instance()->clear();
}

void
PingusMain::deinit_clanlib()
{
#if 0
  CL_SetupCore::deinit();
  CL_SetupGUI::deinit ();

  if (use_opengl)
    CL_SetupGL::deinit();
  else
    CL_SetupSDL::deinit();

  CL_SetupDisplay::deinit ();
#endif 
}

void
PingusMain::init_pingus()
{
////   SavegameManager::instance();
////   StatManager::init();
  Resource::init();
  Fonts::init();
////   ScreenManager::init();
////   PingusMenuManager::init();
  Sound::PingusSound::init();
////   PinguActionFactory::init();
////   Credits::init();
////   WorldMapNS::WorldMapManager::instance();

////  fps_counter.init();
  console.init();
}

void
PingusMain::deinit_pingus()
{
////  fps_counter.deinit();
  console.deinit();

  Fonts::deinit();
////  Credits::deinit();
////  PinguActionFactory::deinit();
  Sound::PingusSound::deinit();
////  PingusMenuManager::deinit();
////  WorldObjFactory::deinit();
////  WorldMapNS::WorldMapManager::deinit();
////  ScreenManager::deinit();
////  StatManager::deinit();
////  SavegameManager::deinit();
  Resource::deinit();
}


int main(int argc, char** argv)
{
  PingusMain app;
  return app.main(argc, argv);
}

/* EOF */
