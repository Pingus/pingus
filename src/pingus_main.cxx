//   $Id: pingus_main.cxx,v 1.55 2003/03/28 16:16:00 grumbel Exp $
//    ___
//   |  _\ A Free Lemmings[tm] Clone
//   |   /_  _ _  ___  _   _  ___ 
//   |  || || \ || _ || |_| ||_ -'
//   |__||_||_\_||_  ||_____||___|
//                _| |
//               |___|
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

#include <sys/stat.h>
#include <stdio.h>
#include <signal.h>

#ifdef HAVE_GETTEXT
#  include <locale.h>
#endif

#ifndef WIN32
#  include <config.h>
#  include <getopt.h>
#else /* !WIN32 */
#  include <direct.h>
#  include "win32/config.h"
#  include "win32/getopt.h"
#endif /* !WIN32 */

#include <ClanLib/Core/System/console_window.h>
#include <ClanLib/Display/setupdisplay.h>
#include <ClanLib/Display/Display/display.h>
#include <ClanLib/Display/Input/input.h>

#include <ClanLib/Core/System/setupcore.h>
#include <ClanLib/jpeg.h>
#include <ClanLib/png.h>
#include <ClanLib/GUI/setupgui.h>


#ifdef HAVE_LIBCLANGL
# include <ClanLib/gl.h>
#endif

#include "my_gettext.hxx"

#include "gui/screen_manager.hxx"
#include "gui/input_debug_screen.hxx"
#include "path_manager.hxx"
#include "pingus_main.hxx"
#include "globals.hxx"
#include "system.hxx"
#include "pingus_error.hxx"
#include "global_event.hxx"
#include "config.hxx"
#include "config_xml.hxx"
#include "console.hxx"
#include "fps_counter.hxx"
#include "plf_res_mgr.hxx"
#include "game_session.hxx"
#include "story_screen.hxx"
#include "start_screen.hxx"
#include "savegame_manager.hxx"
#include "stat_manager.hxx"
#include "demo_session.hxx"
#include "debug.hxx"
#include "editor/editor.hxx"
#include "action_data.hxx"
#include "fonts.hxx"
#include "xml_helper.hxx"
#include "pingus_menu_manager.hxx"
#include "pingus_resource.hxx"
#include "pingu_action_factory.hxx"
#include "credits.hxx"
#include "sound/sound.hxx"
#include "cheat.hxx"

using EditorNS::Editor;

void
signal_handler(int signo)
{
  switch(signo)
    {
    case SIGSEGV:
      puts(_("\n,------------------------------------------------------------------------"));
      puts(_("| segfault_handler: catched a SIGSEGV."));
      puts ("|");
      puts(_("| Woops, Pingus just crashed, congratulations you've found a bug."));
      puts(_("| Please write a little bug report to <grumbel@gmx.de>, include informations"));
      puts(_("| where exacly the SIGSEGV occured and how to reproduce it."));
      puts(_("| Also try include a backtrace, you can get it like this:"));
      puts ("|");
      puts(_("| $ gdb pingus core"));
      puts(_("| (gdb) bt"));
      puts(_("| ..."));
      puts ("|");
      puts(_("| If that doesn't work, try this:"));
      puts ("|");
      puts(_("| $ gdb pingus"));
      puts(_("| (gdb) r"));
      puts(_("| [play until it crashes again]"));
      puts(_("| ..."));
      puts ("|");
      puts(_("'------------------------------------------------------------------------\n"));
      break;

    case SIGINT:
      puts(_("\n,------------------------------------------------------------------------"));
      puts (_("| Warning: Pingus recieved a SIGINT, exiting now."));
      puts(_("`------------------------------------------------------------------------\n"));
      break;

    default:
      std::cout << "signal_handler (): Got unknown signal: " << signo << std::endl;
      break;
    }
  puts ("exit(EXIT_FAILURE);");
  exit (EXIT_FAILURE);
}

PingusMain::PingusMain()
{
}

PingusMain::~PingusMain()
{
}

char* 
PingusMain::get_title()
{
  static char title[] = "Pingus - http://pingus.seul.org";
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
      //ConfigXML("/home/ingo/projects/pingus/cvs/data/levels/stone3-grumbel.xml");
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
  int c = 0; 
  int option_index = 0;
  optind = 0;

  // FIXME: We need some clean up here
  struct option long_options[] =
    {
      {"enable-music",      no_argument,       0, 'm'},
      {"enable-sound",      no_argument,       0, 's'},
      {"enable-gimmicks",   no_argument,       0, 'i'},
      {"enable-cursor",     no_argument,       0, 'c'},
      {"disable-intro",     no_argument,       0, 'n'},
      {"play-demo",         required_argument, 0, 'p'},
      {"record-demo",       required_argument, 0, 'r'},
      {"speed",             required_argument, 0, 't'},
      {"datadir",           required_argument, 0, 'd'},
      {"level",             required_argument, 0, 'l'},
      {"help",              no_argument,       0, 'h'},
      {"version",           no_argument,       0, 'V'},
      {"verbose",           required_argument, 0, 'v'},
      {"print-fps",         no_argument,       0, 'b'},
      {"sound-specs",       required_argument, 0, 'S'},
      {"geometry",          required_argument, 0, 'g'},
      {"editor",            no_argument,       0, 'e'},
      {"quick-play",        no_argument,       0, 'q'},
      {"enable-fullscreen", no_argument,       0, 'f'},
      {"disable-fullscreen",no_argument,       0, 'F'},
      {"disable-swcursor",  no_argument,       0, 145},
      {"enable-swcursor",   no_argument,       0, 146},
      {"disable-action-help",no_argument,      0, 156},
      {"enable-action-help", no_argument,      0, 157},
      {"enable-bg-manipulation", no_argument,  0, 148},
      {"min-cpu-usage",     no_argument,       0, 153},
      {"min-frame-skip",    required_argument, 0, 154},
      {"max-frame-skip",    required_argument, 0, 155},
      {"frame-skip",        required_argument, 0, 157},
      {"cheat",             required_argument, 0, 156},
#ifdef HAVE_LIBCLANGL
      {"use-opengl",        no_argument,       0, 'G'},
#endif
      // FIXME: is the number stuff correct?
      {"fs-preload",      no_argument,       0, 130},
      {"fast",            no_argument,       0, 132},
      {"fast-mode",            no_argument,       0, 132},
      {"disable-previews",no_argument,       0, 133}, 
      {"maintainer-mode", no_argument,       0, 134},
      {"disable-auto-scrolling",   no_argument,       0, 137},

      // 
      {"no-cfg-file",    no_argument,       0, 142},
      {"tile-size",      required_argument, 0, 144},
      {"config-file",    required_argument, 0, 147},
      {"debug",          required_argument, 0, 152},
      {0, 0, 0, 0}
    };

  while(true) {
    c = getopt_long(argc, argv, "r:p:smv:d:l:hVp:bxS:g:it:cqefFG", long_options, &option_index);
    
    if (c == -1 || c == 1)
      break;

    switch(c) {
    case 'c': // -c, --enable-cursor
      cursor_enabled = true;
      if (verbose) std::cout << "PingusMain:check_args: Cursor enabled" << std::endl;
      break;
    case 'b': // -b, --print-fps
      print_fps = true;
      if (verbose) std::cout << "PingusMain:check_args: Printing fps enabled" << std::endl;
      break;
    case 'l': // -l, --level
      {
	// FIXME: Quick hack to get an absolute path
	char cwd[1024];
	levelfile = optarg;
	if (getcwd (cwd, 1024))
	  levelfile = std::string(cwd) + "/" + levelfile;
	if (verbose) 
	  std::cout << "PingusMain:check_args: Levelfile = " << levelfile << std::endl;
	intro_disabled = true;
      }
      break;
    case 't': // -t, --set-speed
      game_speed = atoi(optarg);
      if (verbose) 
	std::cout << "PingusMain:check_args: Game Speed = "
		  << game_speed << std::endl;
      break;
    case 'e':
      start_editor = true;
      intro_disabled = true;
      std::cout << "PingusMain: Starting Editor" << std::endl;
      break;
    case 'q':
      quick_play = true;
      break;

#ifdef HAVE_LIBCLANGL
    case 'G':
      use_opengl = true;
      break;
#endif

    case 's': // -s, --enable-sound
      sound_enabled = true;
      break;
    case 'g':
      {
        char c;
        if (sscanf(optarg, "%d%c%d", &screen_width, &c, &screen_height) != 3 && c != 'x')
          {
            std::cout << "Resolution std::string is wrong, it should be like: \n" 
                      << "\"640x480\" or \"800x600\"" << std::endl;
            exit(EXIT_FAILURE);
          }
      }
      break;
    case 'S':
      std::cout << "not impl. XALA" << std::endl;
      //pingus_soundfile = optarg;
      //if (verbose) 
      //std::cout << "check_args: Sound File = " << pingus_soundfile << std::endl;
      break;
    case 'm': // -m, --enable-music
      if (verbose) std::cout << "check_args: Music enabled" << std::endl;
      music_enabled = true;
      break;
    case 'd': // -d, --datadir
      if (optarg) 
	{
	  path_manager.add_path(optarg);
	 
	  if (verbose)
	    std::cout << "check_args: Pingus Data Dir = "
		      << optarg << std::endl;
	}
      else
	{
	  std::cout << "PingusMain: -d: optarg is (null)" << std::endl;
	}
      break;
    case 'n':
      intro_disabled = true;
      break;
    case 'V':
      std::cout << "Pingus Version " << VERSION 
		<< std::endl;
      
      std::cout << _("\n\
Copyright (C) 1998 Ingo Ruhnke <grumbel@pingus.cx>\n\
There is NO warranty.  You may redistribute this software\n\
under the terms of the GN General Public License.\n\
For more information about these matters, see the files named COPYING.\
") << std::endl;

      exit(EXIT_SUCCESS);
      break;
    case 'i':
      gimmicks_enabled = true;
      if (verbose) std::cout << "Pingus: Gimmicks enabled" << std::endl;
      break; 
    case 'r': // -r, --record-demo
      record_demo = true;
      demo_file = optarg;
      break;
    case 'p': // -p, --play-demo
      play_demo = true;
      demo_file = optarg;
      if (verbose)
	std::cout << "Using demofile: " << demo_file << std::endl;
      break;
    case 'v':
      sscanf(optarg, "%d", &verbose);
      std::cout << "Pingus: Verbose level is " << verbose << std::endl;
      break;

    case 'f': // --enable-fullscreen
      fullscreen_enabled = true;
      break;
      
    case 'F': // --disable-fullscreen
      fullscreen_enabled = false;
      break;

      // Starting weird number options... no idea if this is correct.
    case 130: // --fs-preload
      fs_preload = true;
      previews_enabled = true;
      break;
    case 132:
      fast_mode = true;
      break;
    case 133:
      previews_enabled = false;
      break;
    case 134: // --maintainer_mode
      std::cout << "---------------------------------" << std::endl
		<< "--- Maintainer Mode activated ---" << std::endl
		<< "---------------------------------" << std::endl; 
      maintainer_mode = true;
      break;

    case 137:
      auto_scrolling = false;
      break;
    case 142: // --no-cfg-file
      // Nothing, since that is handled in quick_check_args()
      break;

    case 144:
      sscanf(optarg, "%d", &tile_size);
      break;
   
    case 145:
      swcursor_enabled = true;
      break;

    case 146:
      swcursor_enabled = false;
      break;

    case 147:
      config_file = optarg;
      break;
      
    case 148:
      background_manipulation_enabled = true;
      break;

    case 152:
      if (strcmp (optarg, "all") == 0)
	{
	  pingus_debug_flags |= PINGUS_DEBUG_ALL;
	}
      else if (strcmp (optarg, "actions") == 0)
	{
	  pingus_debug_flags |= PINGUS_DEBUG_ACTIONS;
	}
      else if (strcmp (optarg, "sound") == 0)
	{
	  pingus_debug_flags |= PINGUS_DEBUG_SOUND;
	}
      else if (strcmp (optarg, "gametime") == 0)
	{
	  pingus_debug_flags |= PINGUS_DEBUG_GAMETIME;
	}
      else if (strcmp (optarg, "tiles") == 0)
	{
	  pingus_debug_flags |= PINGUS_DEBUG_TILES;
	}
      else if (strcmp (optarg, "loading") == 0)
	{
	  pingus_debug_flags |= PINGUS_DEBUG_LOADING;
	}
      else if (strcmp (optarg, "translator") == 0)
	{
	  pingus_debug_flags |= PINGUS_DEBUG_TRANSLATOR;
	}
      else if (strcmp (optarg, "resources") == 0)
        {
          pingus_debug_flags |= PINGUS_DEBUG_RESOURCES;
        }
      else if (strcmp (optarg, "gui") == 0)
        {
          pingus_debug_flags |= PINGUS_DEBUG_GUI;
        }
      else if (strcmp (optarg, "input") == 0)
        {
          pingus_debug_flags |= PINGUS_DEBUG_INPUT;
        }
      else if (strcmp (optarg, "worldmap") == 0)
        {
          pingus_debug_flags |= PINGUS_DEBUG_WORLDMAP;
        }
      else
	{
	  std::cout << "PingusMain: Unhandled debug flag: " << optarg << std::endl;
          exit(EXIT_FAILURE);
	}

      break;

    case 153:
      max_cpu_usage = false;
      break;

    case 154:
      sscanf(optarg, "%d", &min_frame_skip);
      break;

    case 155: // max_frame_skip
      sscanf(optarg, "%d", &max_frame_skip);
      break;

    case 157: // frame_skip
      sscanf(optarg, "%d", &max_frame_skip);
      min_frame_skip = max_frame_skip;
      break;

    case 156: // Cheats
      Cheat::activate(optarg);
      break;

    default:
      if (verbose) std::cout << _("Unknow char: ") << c << std::endl << std::endl;
      std::cout << _("Usage: ") << argv[0] << _(" [OPTIONS]... [LEVELFILE]") << std::endl;
      std::cout <<
	_("\n"
	  "Options:\n"
          "   -g, --geometry {width}x{height}\n"
          "                            Set the resolution for pingus (default: 640x480)\n"
          "   -h, --help               Displays this screen\n"
          "   --disable-intro          Disable intro\n")
		<< std::endl;
#ifdef HAVE_LIBCLANGL
      std::cout <<
	_("   -G, --use-opengl         Use OpenGL\n")
                << std::endl;
#endif
      std::cout <<
	_("   -F, --disable-fullscreen Disable Fullscreen\n"
          "   -f, --enable-fullscreen  Enable Fullscreen (default)\n"
          "   -d, --datadir PATH       Set the path to load the data files to `path'\n"
          "   --use-datafile           Use the pre-compiled datafile (default)\n"
          "   --use-scriptfile         Use the scriptfile and read all data from files\n"
          "   -l, --level FILE         Load a custom level from `file'\n"
          "   -v, --verbose            Print some more messages to stdout, can be set\n"
          "                            multible times to increase verbosity\n"
          "   -V, --version            Prints version number and exit\n"
          //	"   --fs-preload             Preload all Levelpreviews\n"a
          "   --fast-mode              Disable some cpu intensive features\n"
          //	"   --disable-previews       Disables all level preview in the level selector\n"
          "   -e, --editor             Launch the Level editor (experimental)\n"
          "   --disable-auto-scrolling Disable automatic scrolling\n"
          "   --disable-swcursor       Disable software cursor, use hw cursor instead\n"
          "   --enable-swcursor        Enable software cursor\n"
          "   --disable-action-help    Disable action button help strings\n"
          "   --enable-action-help     Enable action button help strings(default)\n"
          "   --no-cfg-file            Don't read ~/.pingus/config\n"
          "   --config-file FILE       Read config from FILE (default: ~/.pingus/config)\n"
          "   --min-cpu-usage          Reduces the CPU usage by issuing sleep()\n"
          "                            reduce CPU usage, might speed up the game on slower machines\n"
          "   --min-frame-skip N       Skip at least N frames, larger values speed the game up\n"
          "   --max-frame-skip N       Skip at most N frames\n"
          "   --frame-skip N           Set both min and max frameskip to N\n"

          "\nDebugging and experimental stuff:\n"
          "   --maintainer-mode        Enables some features, only interesting programmers\n"
          "   --enable-bg-manipulation Enables color manipulation of level backgrounds\n"
          "   --debug OPTION           Enable the output of debugging infos, possible\n"
          "                            OPTION's are tiles, gametime, actions, sound, resources, gui,\n"
          "                            input\n"
          "   -t, --speed SPEED        Set the game speed (0=fastest, >0=slower)\n"
          "   -b, --print-fps          Prints the fps to stdout\n"
          "   -i, --enable-gimmicks    Enable some buggy development stuff\n"
          "   -S, --sound-specs FILE   Use files mentioned in FILE\n"
          "   --tile-size INT          Set the size of the map tiles (default: 32)\n"
          "\n"
          "\nDemo playing and recording:\n"
          "   -r, --record-demo FILE   Record a demo session to FILE\n"
          "   -p, --play-demo FILE     Plays a demo session from FILE\n")
                << std::endl;
      std::cout
        << _("\nSound:\n"
             "   -s, --enable-sound       Enable sound\n"
             "   -m, --enable-music       Enable music\n")
        << std::endl;
      exit(EXIT_SUCCESS);
      break;
    }
  }

  // Treating non option arguments
  for(int i = optind; i < argc; ++i) 
    {
      if (levelfile.empty()) {
	levelfile = argv[i];
	intro_disabled = true;

	if (!System::exist(levelfile))
	  {
	    std::cout << "PingusMain: " << levelfile << " not found" << std::endl;
	    exit (EXIT_FAILURE);
	  }
      } else {
	std::cout << "Wrong argument: '" << argv[i] << "'" << std::endl;
	std::cout << "A levelfile is already given," << std::endl;
	exit(EXIT_FAILURE);
      }
    }
}

// Get all filenames and directories
void
PingusMain::init_path_finder()
{
  System::init_directories();

#ifndef WIN32
  if (maintainer_mode)
    std::cout << "Directory name of " << executable_name << " - " << System::dirname(executable_name)
              << std::endl;

  // FIXME: Do we need this any longer?
  char* pingus_datadir_env = getenv ("PINGUS_DATADIR");
  if (pingus_datadir_env)
    path_manager.add_path (pingus_datadir_env);

  /* Some magic for detecting the path */
  path_manager.add_path (System::dirname(executable_name) + "/../data/");
  path_manager.add_path ("../data/");     // started from 'src/'
  path_manager.add_path ("data/");        // started from base directory with 'src/pingus'
  path_manager.add_path ("share/games/pingus/");  // started from base directory of the binary
  path_manager.add_path ("../share/games/pingus/");  // started from base directory of the binary
  path_manager.add_path (PINGUS_DATADIR); // started from $PATH
  // As a last hope we try this:
  path_manager.add_path ("/usr/share/pingus/");
  path_manager.add_path ("/usr/local/share/pingus/");

  std::list<std::string> file_list;

  file_list.push_back ("data/core.scr");

  if (!path_manager.find_path (file_list))
    {
      std::cout << "Error: Couldn't find 'data/core.scr', please set the enviroment variable\n"
		<< "PINGUS_DATADIR to the path of the file `data/core.scr' or use the\n"
		<< "-d option." << std::endl;
      exit(EXIT_FAILURE);
    }

#else /* !WIN32 */
  //If the User uses Windows, the Datadir is always the Subdirectory "data"
  path_manager.set_path("data");
#endif /* !WIN32 */
 
#ifdef HAVE_GETTEXT
  if (maintainer_mode)
    std::cout << "Setting gettext path to: " << path_manager.get_base_path () + "/../../locale" << std::endl;
  const char* ret = setlocale (LC_ALL, "");
  if (ret == NULL)
    {
      std::cout << "ERROR: setlocale failed!" <<  std::endl;
    }
  else
    {
      std::cout << "setlocale returned '" << ret << "'" << std::endl;
    }
  bindtextdomain (PACKAGE, (path_manager.get_base_path () + "/../../locale/").c_str());
  // We use another LOCALEDIR to make static binaries possible
  // bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
#endif 
  std::cout << "BasePath: " << path_manager.get_base_path () << std::endl;
}

void
PingusMain::print_greeting_message()
{
  std::cout << "Welcome to Pingus "VERSION"!\n"
            << "=======================\n" << std::endl;
  
#ifdef HAVE_LIBCLANVORBIS
  std::cout << "clanVorbis support: ok" << std::endl;
#else
  std::cout << "clanVoribs support: missing (.ogg music files will not be playable)" << std::endl;
#endif

#ifdef HAVE_LIBCLANMIKMOD
  std::cout << "clanMikMod support: ok" << std::endl;
#else
  std::cout << "clanMikMod support: missing (.it and .s3m music files will not be playable)" << std::endl;
#endif

#ifdef HAVE_GETTEXT
  std::cout << "getext support: ok" << std::endl;
  std::cout << gettext("gettext language: english") << std::endl;
#else
  std::cout << "getext support: missing (only support for english will be available" << std::endl;
#endif

  if (sound_enabled)
    std::cout << "sound support: enabled" << std::endl;
  else
    std::cout << "sound support: disabled" << std::endl;

  if (music_enabled)
    std::cout << "music support: enabled" << std::endl;
  else
    std::cout << "music support: disabled" << std::endl;

  std::cout << "resolution set to: " << screen_width << "x" << screen_height << std::endl;

  std::cout << std::endl;
}

void
PingusMain::start_game ()
{
  if (verbose) {
    pout << _("PingusMain: Starting Main: ") << CL_System::get_time() << std::endl;
  }

  if (print_fps)
    Display::add_flip_screen_hook(&fps_counter);

  // Register the global event catcher
  on_button_press_slot = CL_Input::sig_button_press ().connect (&global_event, &GlobalEvent::on_button_press);
  on_button_release_slot = CL_Input::sig_button_release ().connect (&global_event, &GlobalEvent::on_button_release);
      
  // Set the root screen
  if (show_input_debug_screen) // show a debug screen
    {
      ScreenManager::instance()->push_screen(new InputDebugScreen (), true);
    }
  else if (!levelfile.empty ()) // start editor or a game_session
    {
      bool successfull = true;
      if (!System::exist(levelfile))
        {
          if (System::exist(levelfile + ".xml"))
            levelfile += ".xml";
          else if (System::exist("levels/" + levelfile + ".xml"))
            levelfile = "levels/" + levelfile + ".xml";
          else
            {
              pout << _("PingusMain: Levelfile not found, ignoring: ") << levelfile << std::endl;
              successfull = false;
            }
        }

      if (successfull)
        {
          if (start_editor) 
            {
              Editor::instance ()->load_level (levelfile);
              levelfile = "";
              ScreenManager::instance()->push_screen(Editor::instance (), false);
            } 
          else
            {
              /*ScreenManager::instance()->push_screen
                (new PingusGameSession (PLFResMgr::load_plf_from_filename(levelfile), false), true);*/
              ScreenManager::instance()->push_screen
                (new StartScreen(PLFResMgr::load_plf_from_filename(levelfile)),
                 true);
            }
        }    
    }
  else if (start_editor) // Start an empty editor workspace
    { 
      ScreenManager::instance()->push_screen(Editor::instance (), false);
    }
  else if (!demo_file.empty()) // start a demo
    {
      ScreenManager::instance()->push_screen(new DemoSession (demo_file));
    }
  else // start a normal game
    {
      ScreenManager::instance()->push_screen (PingusMenuManager::instance (), false);
      //ScreenManager::instance()->push_screen (new StoryScreen(), true);
    }

  // show the main menu, the rest of the game is spawn from there
  if (maintainer_mode)
    std::cout << "PingusMain::start screen manager" << std::endl;
  ScreenManager::instance ()->display ();
  if (maintainer_mode)
    std::cout << "PingusMain::quit game and screen_manager" << std::endl;
  
  // unregister the global event catcher
  CL_Input::sig_button_press ().disconnect (on_button_press_slot);
  CL_Input::sig_button_release ().disconnect(on_button_release_slot);
}

int
PingusMain::main(int argc, char** argv)
{
  executable_name = argv[0];
  
  // Register the segfault_handler
  //signal(SIGSEGV, signal_handler);
  //signal(SIGINT,  signal_handler);

  // Redirect stdout to somewhere where it is readable
  CL_ConsoleWindow cl_console(PACKAGE VERSION);
  cl_console.redirect_stdio();

  try 
    {
      quick_check_args(argc, argv);
      read_rc_file();
      check_args(argc, argv);

      init_path_finder();

      print_greeting_message();

      init_clanlib();
      init_pingus();	

      // Avoid uglyness on window opening
      CL_Display::clear_display();
      CL_Display::flip_display();

      start_game();
    
      deinit_pingus();
      deinit_clanlib();
    }
  
  catch (const CL_Error& err) {
    std::cout << _("Error caught from ClanLib: ") << err.message << std::endl;
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

  return 0;
}


void
PingusMain::init_clanlib()
{
  CL_SetupCore::init ();
  CL_SetupPNG::init ();
  CL_SetupJPEG::init ();
  CL_SetupGUI::init ();

#ifdef HAVE_LIBCLANGL
  if (use_opengl) {
    CL_SetupGL::init();
    std::cout << "Using OpenGL" << std::endl;
  }
#endif

  CL_SetupDisplay::init();

  if (verbose) 
    {
      std::cout << "Using resolution: " 
		<< screen_width << "x" << screen_height << std::endl;
    }

  // Initing the display
  CL_Display::set_videomode(screen_width, screen_height, 16, 
			    fullscreen_enabled, 
			    false); // allow resize
  CL_Display::clear_display();
  CL_Display::flip_display();
  
#ifdef HAVE_LIBCLANGL
  if (use_opengl)
    {
      CL_OpenGL::begin_2d ();
      glEnable (GL_BLEND);
    }
#endif
}

void
PingusMain::deinit_clanlib()
{
  CL_SetupDisplay::deinit();
#ifdef HAVE_LIBCLANGL
  if (use_opengl)
    CL_SetupGL::deinit(); 
#endif
  CL_SetupGUI::deinit ();
  CL_SetupPNG::deinit();
  CL_SetupJPEG::deinit();
  CL_SetupCore::deinit();
}

void
PingusMain::init_pingus()
{
  // Init error/warning/notice streams
  pout.add (std::cout);
  pout.add (console);
  pwarn.add (std::cout);
  pout.add (console);
  perr.add (std::cout);
  perr.add (console);

  SavegameManager::instance();
  StatManager::init();
 
  ScreenManager::init();
  PingusSound::init();
  PingusResource::init();
  Fonts::init();
  XMLhelper::init();
  PinguActionFactory::init();
  Editor::init(); 
  Credits::init();

  fps_counter.init();
  console.init();

  // FIXME: See action_data.hxx, a bit ugly
  init_default_actions();
}

void
PingusMain::deinit_pingus()
{
  Credits::deinit();
  Editor::deinit();
  PinguActionFactory::deinit();
  XMLhelper::deinit();
  Fonts::deinit();
  PingusResource::deinit();
  PingusSound::deinit();
  ScreenManager::deinit();
  StatManager::deinit();
}  

/* EOF */
