//   $Id: pingus_main.cxx,v 1.104 2004/04/01 15:18:05 torangan Exp $
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

#include <config.h>
#include <sys/stat.h>
#include <stdio.h>
#include <signal.h>
#include <locale.h>
#include <getopt.h>

#include <ClanLib/display.h>
#include <ClanLib/sound.h>
#include <ClanLib/core.h>
#include <ClanLib/sdl.h>
#include <ClanLib/gl.h>
#include <ClanLib/gui.h>

#include "gettext.h"

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
#include "worldmap/manager.hxx"
#include "string_tokenizer.hxx"
#include "story.hxx"
#include "cheat.hxx"
#include "preview_renderer.hxx"
#include "worldmap/manager.hxx"
#include "worldobj_data_factory.hxx"

namespace Pingus {

using EditorNS::Editor;

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

PingusMain::PingusMain()
{
  show_credits = false;
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
  int c = 0;
  int option_index = 0;
  optind = 0;

  // FIXME: We need some clean up here
  struct option long_options[] =
    {
      {"disable-music",      no_argument,       0, 'm'},
      {"disable-sound",      no_argument,       0, 's'},
      {"enable-cursor",     no_argument,       0, 'c'},
      {"disable-intro",     no_argument,       0, 'n'},
      {"play-demo",         required_argument, 0, 'p'},
      {"disable-demo-recording", required_argument, 0, 'r'},
      {"speed",             required_argument, 0, 't'},
      {"datadir",           required_argument, 0, 'd'},
      {"level",             required_argument, 0, 'l'},
      {"worldmap",          required_argument, 0, 158},
      {"credits",           no_argument,       0, 159},
      {"help",              no_argument,       0, 'h'}, // add -? support
      {"version",           no_argument,       0, 'V'},
      {"verbose",           required_argument, 0, 'v'},
      {"print-fps",         no_argument,       0, 'b'},
      {"sound-specs",       required_argument, 0, 'S'},
      {"geometry",          required_argument, 0, 'g'},
      {"editor",            no_argument,       0, 'e'},
      {"quick-play",        no_argument,       0, 'q'},
      {"fullscreen",        no_argument,       0, 'f'},
      {"window",            no_argument,       0, 'w'},
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
      {"controller",        required_argument, 0, 160},
      {"render-preview",    required_argument, 0, 161},
      {"use-opengl",        no_argument,       0, 'G'},

      // FIXME: is the number stuff correct?
      {"fast",            no_argument,       0, 132},
      {"fast-mode",       no_argument,       0, 132},
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

    case 158: // --worldmap
      worldmapfile = optarg;
      break;

    case 159: // --worldmap
      show_credits = true;
      break;

    case 'l': // -l, --level
      levelfile = optarg;
      break;

    case 't': // -t, --set-speed
      game_speed = atoi(optarg);
      break;

    case 'e':
      start_editor = true;
      //std::cout << "PingusMain: Starting Editor" << std::endl;
      break;

    case 'G':
      use_opengl = true;
      break;

    case 's': // -s, --disable-sound
      sound_enabled = false;
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
        if (screen_width > 800 || screen_height > 600)
          {
            std::cout << _("Warning: Larger resolution than 800x600 will result in visual problems") << std::endl;
          }
      }
      break;
    case 'S':
      std::cout << "not impl. XALA" << std::endl;
      //pingus_soundfile = optarg;
      //if (verbose)
      //std::cout << "check_args: Sound File = " << pingus_soundfile << std::endl;
      break;
    case 'm': // -m, --disable-music
      music_enabled = false;
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
      demo_file = optarg;
      if (verbose)
	std::cout << "Using demofile: " << demo_file << std::endl;
      break;
    case 'v':
      sscanf(optarg, "%d", &verbose);
      std::cout << "Pingus: Verbose level is " << verbose << std::endl;
      break;

    case 'f': // --fullscreen
      fullscreen_enabled = true;
      break;

    case 'w': // --window
      fullscreen_enabled = false;
      break;

      // Starting weird number options... no idea if this is correct.
    case 132:
      fast_mode = true;
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
      swcursor_enabled = false;
      break;

    case 146:
      swcursor_enabled = true;
      break;

    case 147:
      config_file = optarg;
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
      else if (strcmp (optarg, "pathmgr") == 0)
        {
          pingus_debug_flags |= PINGUS_DEBUG_PATHMGR;
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

    case 160:
      controller_file = optarg;
      break;

    case 161:
      std::cout << "Rendering a Level Preview..." << std::endl;
      render_preview = true;
      preview_file   = optarg;
      break;

    default:
      if (verbose) std::cout << _("Unknow char: ") << c << std::endl << std::endl;
      std::cout << _("Usage: ") << argv[0] << _(" [OPTIONS]... [LEVELFILE]") << std::endl;
      std::cout
        << "\n"
        << _("Options:")
        << "\n   -g, --geometry {width}x{height}"
        << "\n                            " << _("Set the resolution for pingus (default: 800x600)")
        << "\n   -h, --help               " << _("Displays this help")
        << "\n   --disable-intro          " << _("Disable intro");
      std::cout << "   -G, --use-opengl         " << _("Use OpenGL");
      std::cout
	<< "\n   -w, --window             " << _("Start in Window Mode")
        << "\n   -f, --fullscreen         " << _("Start in Fullscreen")
        << "\n   -d, --datadir PATH       Set the path to load the data files to `path'"
        //"   --use-datafile           Use the pre-compiled datafile (default)\n"
        //          "   --use-scriptfile         Use the scriptfile and read all data from files\n"
        << "\n   -l, --level    " << _("FILE      ") << _("Load a custom level from FILE")
        << "\n   --worldmap   "<< _("FILE        ") << _("Load a custom worldmap from FILE")
        << "\n   -v, --verbose            " << _("Print some more messages to stdout, can be set")
        << "\n                            " << _("multiple times to increase verbosity")
        << "\n   -V, --version            " << _("Prints version number and exit")
        << "\n   -e, --editor             " << _("Launch the Level editor (experimental)")
        << "\n   --disable-auto-scrolling " << _("Disable automatic scrolling")
        //          "   --disable-swcursor       Disable software cursor, use hw cursor instead\n"
        << "\n   --enable-swcursor        " << _("Enable software cursor")
        //"   --disable-action-help    Disable action button help strings\n"
        //"   --enable-action-help     Enable action button help strings(default)\n"
        << "\n   --no-cfg-file            " << _("Don't read ~/.pingus/config")
        << "\n   --config-file " << _("FILE       ") << _("Read config from FILE (default: ~/.pingus/config)")
        << "\n                            " << _("reduce CPU usage, might speed up the game on slower machines")
        << "\n   --controller FILE        " << _("Uses the controller given in FILE")

        << "\n\n" << _("Debugging and experimental stuff:")
        << "\n   --maintainer-mode        " << _("Enables some features, only interesting programmers")
        << "\n   --debug OPTION           " << _("Enable the output of debugging infos, possible")
        << "\n                            " << _("OPTION's are tiles, gametime, actions, sound, resources, gui,")
        << "\n                            " << "input, pathmgr"
        << "\n   --min-frame-skip N       " << _("Skip at least N frames, larger values speed the game up")
        << "\n   --max-frame-skip N       " << _("Skip at most N frames")
        << "\n   --frame-skip N           " << _("Set both min and max frameskip to N")
        << "\n   -t, --speed SPEED        " << _("Set the game speed (0=fastest, >0=slower)")
        << "\n   -b, --print-fps          " << _("Prints the fps to stdout")
        << "\n   --tile-size INT          " << _("Set the size of the map tiles (default: 32)")
        << "\n   --fast-mode              " << _("Disable some cpu intensive features")
        << "\n   --min-cpu-usage          " << _("Reduces the CPU usage by issuing sleep()")

        << "\n\n" << _("Demo playing and recording:")
        << "\n   -p, --play-demo " << _("FILE     ") << _("Plays a demo session from FILE")
        << "\n   -r, --disable-demo-recording"
        << "\n                            " << _("Record demos for each played level")

        << "\n\n" << _("Sound:")
        << "\n   -s, --disable-sound       " << _("Disable sound")
        << "\n   -m, --disable-music       " << _("Disable music")
        << "\n"
        << std::endl;

      exit(EXIT_SUCCESS);
      break;
    }
  }

  // make sure that we're not recording a demo while already playing one
  if (play_demo)
    enable_demo_recording = false;


  // Treating non option arguments
  for(int i = optind; i < argc; ++i)
    {
      if (levelfile.empty()) {
	levelfile = argv[i];

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
  path_manager.add_path(System::dirname(executable_name) + "/../data/");
  path_manager.add_path("../data/");     // started from 'src/'
  path_manager.add_path("data/");        // started from base directory with 'src/pingus'
  path_manager.add_path("share/games/pingus/");  // started from base directory of the binary
  path_manager.add_path("../share/games/pingus/");  // started from base directory of the binary
  path_manager.add_path("share/pingus/");  // started from base directory of the binary
  path_manager.add_path("../share/pingus/");  // started from base directory of the binary
  path_manager.add_path(PINGUS_DATADIR); // started from $PATH

  // somebody created a symlink in /usr/bin/ or so to the real binary elsewhere
  if (System::is_symlink(executable_name))
    {
      std::string real_path = System::dirname(System::readlink(executable_name));
      path_manager.add_path(real_path + "/../data/");     // started from 'src/'
      path_manager.add_path(real_path + "/data/");        // started from base directory with 'src/pingus'
      path_manager.add_path(real_path + "/share/games/pingus/");  // started from base directory of the binary
      path_manager.add_path(real_path + "/../share/games/pingus/");  // started from base directory of the binary
      path_manager.add_path(real_path + "/share/pingus/");  // started from base directory of the binary
      path_manager.add_path(real_path + "/../share/pingus/");  // started from base directory of the binary
    }

  // somebody added the real binary to PATH
  {
    const char* path = getenv("PATH");
    if (path)
      {
        StringTokenizer tokenizer(path, ':');
        for (StringTokenizer::iterator i = tokenizer.begin(); i != tokenizer.end(); ++i)
          {
            std::string exe = *i + "/pingus";
            if (System::exist(exe))
              {
                path_manager.add_path(*i + "/../data/");     // started from 'src/'
                path_manager.add_path(*i + "/data/");        // started from base directory with 'src/pingus'
                path_manager.add_path(*i + "/share/games/pingus/");  // started from base directory of the binary
                path_manager.add_path(*i + "/../share/games/pingus/");  // started from base directory of the binary
                path_manager.add_path(*i + "/share/pingus/");  // started from base directory of the binary
                path_manager.add_path(*i + "/../share/pingus/");  // started from base directory of the binary

                // somebody added a symlink to the real binary to the PATH
                if(System::is_symlink(*i + "/pingus"))
                  {
                    std::string real_path = System::dirname(System::readlink(exe));
                    path_manager.add_path(real_path + "/../data/");     // started from 'src/'
                    path_manager.add_path(real_path + "/data/");        // started from base directory with 'src/pingus'
                    path_manager.add_path(real_path + "/share/games/pingus/");  // started from base directory of the binary
                    path_manager.add_path(real_path + "/share/pingus/");
                    path_manager.add_path(real_path + "/../share/games/pingus/");  // started from base directory of the binary
                    path_manager.add_path(real_path + "/../share/pingus/");  // started from base directory of the binary
                  }

                break;
              }
          }
      }
  }

  // As a last hope we try this:
  path_manager.add_path ("/usr/local/share/games/pingus/");
  path_manager.add_path ("/usr/share/games/pingus/");

  path_manager.add_path ("/usr/local/share/pingus/");
  path_manager.add_path ("/usr/share/pingus/");

#else /* !WIN32 */
  path_manager.add_path("../data");
  path_manager.add_path("data");
  path_manager.add_path(".");

#endif /* !WIN32 */
  std::list<std::string> file_list;
  file_list.push_back ("data/core.scr");

  if (!path_manager.find_path (file_list))
    {
      std::cout << "Error: Couldn't find 'data/core.scr', please set the enviroment variable\n"
		<< "PINGUS_DATADIR to the path of the file `data/core.scr' or use the\n"
		<< "-d option." << std::endl;
      exit(EXIT_FAILURE);
    }

#ifdef HAVE_GETTEXT
  if (maintainer_mode)
    std::cout << "Setting gettext path to: " << path_manager.get_base_path () + "/../../locale" << std::endl;
  const char* ret;

  if ((ret = setlocale (LC_MESSAGES, "")) == NULL)
    {
      std::cout << "ERROR: setlocale LC_MESSAGES failed!" <<  std::endl;
    }
  else
    {
      if (maintainer_mode)
        std::cout << "setlocale returned '" << ret << "'" << std::endl;
    }

#ifndef WIN32
  bindtextdomain(PACKAGE, path_manager.complete("/../../locale/").c_str());
#else
  bindtextdomain(PACKAGE, path_manager.complete("../locale/").c_str());
#endif

  // We use another LOCALEDIR to make static binaries possible
  // bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain(PACKAGE);

  // Forcing codeset to ISO-8859-1, since usage of
  // setlocate(LC_CTYPE,"") causes all sorts of throuble
  bind_textdomain_codeset(PACKAGE, "ISO-8859-1");
#endif

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

  if (!render_preview)
    {
      // Register the global event catcher
      on_button_press_slot   = window->get_ic()->get_keyboard().sig_key_down().connect (&global_event, &GlobalEvent::on_button_press);
      on_button_release_slot = window->get_ic()->get_keyboard().sig_key_up().connect (&global_event, &GlobalEvent::on_button_release);
    }

  // Set the root screen
  if (show_input_debug_screen) // show a debug screen
    {
      ScreenManager::instance()->push_screen(new InputDebugScreen (), true);
    }
  else if (render_preview)
    {
      if (levelfile.empty())
        {
          PingusError::raise("You need to give a level file to render a preview");
        }
      else
        {
          PreviewRenderer::render(PLFResMgr::load_plf_from_filename(levelfile),
                                  preview_file);
        }
    }
  else if (show_credits)
    {
      ScreenManager::instance()->push_screen(Credits::instance(), false);
    }
  else if (!levelfile.empty ()) // start editor or a game_session
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
          if (start_editor)
            {
              Editor::instance ()->load_level (levelfile);
              levelfile = "";
              ScreenManager::instance()->push_screen(Editor::instance (), false);
            }
          else
            {
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
  else if (!worldmapfile.empty())
    {
      WorldMapNS::WorldMapManager::instance()->load(worldmapfile);
      ScreenManager::instance()->push_screen(WorldMapNS::WorldMapManager::instance());
    }
  else // start a normal game
    {
      ScreenManager::instance()->push_screen (PingusMenuManager::instance (), false);
      //ScreenManager::instance()->push_screen (new StoryScreen(), true);
    }

  if (!render_preview)
    {
      // show the main menu, the rest of the game is spawn from there
      if (maintainer_mode)
        std::cout << "PingusMain::start screen manager" << std::endl;
      ScreenManager::instance ()->display ();
      if (maintainer_mode)
        std::cout << "PingusMain::quit game and screen_manager" << std::endl;

      // unregister the global event catcher
      window->get_ic()->get_keyboard().sig_key_down().disconnect(on_button_press_slot);
      window->get_ic()->get_keyboard().sig_key_up().disconnect(on_button_release_slot);
    }
}

int
PingusMain::main(int argc, char** argv)
{
  executable_name = argv[0];

  // Register the segfault_handler
#ifndef WIN32
  signal(SIGSEGV, signal_handler);
#endif
  //signal(SIGINT, signal_handler);

  // Redirect stdout to somewhere where it is readable
#if defined WIN32 && defined _DEBUG
  CL_ConsoleWindow cl_console(PACKAGE VERSION);
  cl_console.redirect_stdio();
#endif

  // Init error/warning/notice streams
  pout.add (std::cout);
  pout.add (console);
  pwarn.add (std::cout);
  pout.add (console);
  perr.add (std::cout);
  perr.add (console);

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
      if (!render_preview)
        {
          CL_Display::clear();
          CL_Display::flip();
        }
      start_game();

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

      deinit_pingus();
      deinit_clanlib();

#if defined WIN32 && defined _DEBUG
  cl_console.wait_for_key();
#endif

  return 0;
}

void
PingusMain::init_clanlib()
{
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

      window = new CL_DisplayWindow(PACKAGE_STRING,
                           screen_width, screen_height, fullscreen_enabled, false);
      sound  = new CL_SoundOutput(44100);

      CL_Display::clear();
      CL_Display::flip();
    }
  CL_SetupGUI::init ();
  
  on_exit_press_slot = window->sig_window_close().connect(this, &PingusMain::on_exit_press);
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
  CL_SetupGUI::deinit ();

  if (use_opengl)
    CL_SetupGL::deinit();
  else
    CL_SetupSDL::deinit();

  CL_SetupDisplay::deinit ();
  CL_SetupCore::deinit();
}

void
PingusMain::init_pingus()
{
  SavegameManager::instance();
  StatManager::init();

  Fonts::init();
  Story::init();
  ScreenManager::init();
  PingusMenuManager::init();
  Sound::PingusSound::init();
  PingusResource::init();
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
  Sound::PingusSound::deinit();
  PingusMenuManager::deinit();
  PLFResMgr::free_plf_map();
  WorldObjDataFactory::deinit();
  WorldMapNS::WorldMapManager::deinit();
  ScreenManager::deinit();
  StatManager::deinit();
  SavegameManager::deinit();
  xmlCleanupParser();
}

} // namespace Pingus

/* EOF */
