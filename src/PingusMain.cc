//   $Id: PingusMain.cc,v 1.28 2001/06/14 11:07:18 grumbel Exp $
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

#include <iostream>
#include <string>
#include <fstream>
#include <sys/stat.h>

#include <signal.h>

#ifndef WIN32
#  include <config.h>
#  include <getopt.h>
#else /* !WIN32 */
#  include "win32/config.h"
#  include "win32/getopt.h"
#endif /* !WIN32 */

#include <ClanLib/core.h>
#include <ClanLib/jpeg.h>

#include <boost/smart_ptr.hpp>

#include "my_gettext.hh"

// #include "efence.h"

#include "PingusMain.hh"
#include "algo.hh"
#include "globals.hh"
#include "PingusResource.hh"
#include "System.hh"
#include "PingusGame.hh"
//#include "Playfield.hh"
#include "PingusError.hh"
#include "Loading.hh"
#include "Display.hh"
#include "GlobalEvent.hh"
#include "Config.hh"
#include "Console.hh"
#include "FPSCounter.hh"
#include "PingusMessageBox.hh"
#include "audio.hh"

#include "PingusMenuManager.hh"
//#include "Story.hh"

#include "PingusSound.hh"
#include "PingusSoundDummy.hh"

#ifdef HAVE_LIBSDL_MIXER
#  include "PingusSoundReal.hh"
#endif


void
segfault_handler(int signo)
{
  switch(signo)
    {
    case SIGSEGV:
      puts("\n----------------------------------------------------------");
      puts("segfault_handler: catched a SIGSEGV.\n");
      puts("Woops, Pingus just crashed, congratulations you've found a bug.");
      puts("Please write a little bug report to <grumbel@pingus.cx>, include informations");
      puts("where exacly the SIGSEGV occured and how to reproduce it.");
      puts("Also try include a backtrace, you can get it like this:\n");
      puts("$ gdb pingus core");
      puts("(gdb) bt");
      puts("...\n");
      puts("If that doesn't work, try this:\n");
      puts("$ gdb pingus");
      puts("(gdb) r");
      puts("[play until it crashes again]");
      puts("...\n");
      break;

    default:
      std::cout << "segfault_handler: Got unknown signal: " << signo
		<< " ignoring" << std::endl;
      break;
    }
  assert(0);
}

PingusMain::PingusMain()
{
  pingus_datadir_set = false;
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
PingusMain::init_modules()
{
  CL_SetupCore::init();
  //  CL_SetupMagick::init();
}

void PingusMain::deinit_modules()
{
  CL_SetupCore::deinit();   
}

void
PingusMain::read_rc_file(void)
{
  if (!no_config_file)
    {
      std::string   rcfile;

      if (config_file.empty())
	rcfile = System::get_statdir() + "config";
      else
	rcfile = config_file;

      // FIXME: kind of weird...
      boost::shared_ptr<Config> config(new Config(rcfile));
    }
}

// Checking for all options, which needs to be known *before* the
// config file is read. 
void 
PingusMain::quick_check_args(int argc, char* argv[])
{
  no_config_file = false;
  for(int i=1; i < argc; i++)
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
PingusMain::check_args(int argc, char* argv[])
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
    {"enable-bg-manipulation", no_argument,  0, 148},
    {"use-datafile",      no_argument,       0, 150},
    {"use-scriptfile",    no_argument,       0, 151},
    {"max-cpu-usage",     no_argument,       0, 153},
    {"frame-skip",        required_argument, 0, 154},

    // FIXME: is the number stuff correct?
    {"fs-preload",      no_argument,       0, 130},
    {"fast",            no_argument,       0, 132},
    {"disable-previews",no_argument,       0, 133}, 
    {"maintainer-mode", no_argument,       0, 134},
    {"enable-uactions", no_argument,       0, 136},
    {"disable-auto-scrolling",   no_argument,       0, 137},

#ifdef HAVE_LIBSDL_MIXER
    // Sound stuff
    {"audio-format",     required_argument, 0, 138},
    {"audio-rate",       required_argument, 0, 139},
    {"audio-channels",   required_argument, 0, 140},
    {"audio-buffers",    required_argument, 0, 141},
#endif
    // 
    {"no-cfg-file",    no_argument,       0, 142},
    {"tile-size",      required_argument, 0, 144},
    {"config-file",    required_argument, 0, 147},
    {"debug",          required_argument, 0, 152},
    {0, 0, 0, 0}
  };

  while(true) {
    c = getopt_long(argc, argv, "r:p:smv:d:l:hVp:bxS:g:it:cqefF", long_options, &option_index);
    
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
	if (verbose) std::cout << "PingusMain:check_args: Levelfile = " << levelfile << std::endl;
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
      std::cout << "PingusMain: Starting Editor" << std::endl;
      break;
    case 'q':
      quick_play = true;
      break;
    case 's': // -s, --enable-sound
      if (verbose) std::cout << "check_args: Sound Effects enabled" << std::endl;
      std::cout <<
	"\n"
	"=================================================================\n"
	"                            WARNING!                             \n"
	"=================================================================\n"
	"Be warned, at the moment there is only some sound code pressent, \n"
	"there are *no* sound files, so this will fail.\n"
	"=================================================================\n" << std::endl;
      sound_enabled = true;
      break;
    case 'g':
      resolution = optarg;
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
      pingus_datadir = optarg;
      pingus_datadir_set = true;
      if (verbose)
	std::cout << "check_args: Pingus Data Dir = "
			  << pingus_datadir << std::endl;
      break;
    case 'n':
      intro_disabled = true;
      break;
    case 'V':
      std::cout << "Pingus Version " << VERSION 
#ifdef HAVE_LIBSDL_MIXER
		<< _(" (with sound support)")
#else
		<< _(" (without sound support)")
#endif
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

    case 136:
      unlimited_actions = true;
      break;

    case 137:
      auto_scrolling = false;
      break;
#ifdef HAVE_LIBSDL_MIXER
    case 138:
      if (strcmp(optarg, "8") == 0)
	pingus_audio_format = AUDIO_S8;
      else
	pingus_audio_format = AUDIO_S16;
      break;

    case 139:
      sscanf(optarg, "%d", &pingus_audio_rate);
      break;
      
    case 140:
      sscanf(optarg, "%d", &pingus_audio_channels);
      break; 
      
    case 141:
      sscanf(optarg, "%d", &pingus_audio_buffers);
      break;
#endif
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

    case 150:
      use_datafile = true;
      break;

    case 151:
      use_datafile = false;
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
      else
	{
	  std::cout << "PingusMain: Unhandled debug flag: " << optarg << std::endl;
	}

      break;

    case 153:
      max_cpu_usage = true;
      break;

    case 154:
      sscanf(optarg, "%f", &frame_skip);
      break;

    default:
      if (verbose) std::cout << _("Unknow char: ") << c << std::endl << std::endl;
      std::cout << _("Usage: ") << argv[0] << _(" [OPTIONS]... [LEVELFILE]") << std::endl;
      std::cout <<
	"\n"
	"Options:\n"
	"   -g, --geometry {width}x{height}\n"
	"                            Set the resolution for pingus (default: 640x480)\n"
	"   -h, --help               Displays this screen\n"
	"   --disable-intro          Disable intro\n"
	"   -F, --disable-fullscreen Disable Fullscreen\n"
	"   -f, --enable-fullscreen  Enable Fullscreen (default)\n"
       	"   -d, --datadir PATH       Set the path to load the data files to `path'\n"
	"   --use-datafile           Use the pre-compiled datafile (default)\n"
	"   --use-scriptfile         Use the scriptfile and read all data from files\n"
	"   -l, --level FILE         Load a custom level from `file'\n"
	"   -v, --verbose            Print some more messages to stdout, can be set\n"
	"                            multible times to increase verbosity\n"
	"   -V, --version            Prints version number and exit\n"
	//	"   --fs-preload             Preload all Levelpreviews\n"
	"   --fast                   Disable some cpu intensive features\n"
	//	"   --disable-previews       Disables all level preview in the level selector\n"
	"   -e, --editor             Launch the Level editor (experimental)\n"
	"   --disable-auto-scrolling Disable automatic scrolling\n"
	"   --disable-swcursor       Disable software cursor, use hw cursor instead\n"
	"   --enable-swcursor        Enable software cursor\n"
	"   --no-cfg-file            Don't read ~/.pingus/config\n"
	"   --config-file FILE       Read config from FILE (default: ~/.pingus/config)\n"
	"   --max-cpu-usage          Use all of the cpu power available, instead of trying to\n"
	"                            reduce CPU usage, might speed up the game on slower machines\n"
	"   --frame-skip N           Show only every N's frame, larger values speed the game up\n"

	"\nDebugging and experimental stuff:\n"
	"   --maintainer-mode        Enables some features, only interesting programmers\n"
	"   --enable-bg-manipulation Enables color manipulation of level backgrounds\n"
	"   --debug OPTION           Enable the output of debugging infos, possible\n"
        "                            OPTION's are tiles, gametime, actions, sound\n"
	"   -t, --speed SPEED        Set the game speed (0=fastest, >0=slower)\n"
	"   -b, --print-fps          Prints the fps to stdout\n"
	"   -i, --enable-gimmicks    Enable some buggy development stuff\n"
	"   -S, --sound-specs FILE   Use files mentioned in FILE\n"
	"   --tile-size INT          Set the size of the map tiles (default: 32)\n"
	
	"\nDemo playing and recording:\n"
	"   -r, --record-demo FILE   Record a demo session to FILE\n"
	"   -p, --play-demo FILE     Plays a demo session from FILE\n"
#ifdef HAVE_LIBSDL_MIXER
	"\nSound:\n"
	"   -s, --enable-sound       Enable sound\n"
	"   -m, --enable-music       Enable music\n"
	"   --audio-format {8,16}    Number of bits (default: 16)\n"
	"   --audio-rate INT         Audio rate in Hz (default: 44000)\n"
	"   --audio-channels {1,2}   Mono(1) or Stereo(2) output (default: 2)\n"
	"   --audio-buffers INT      Audio buffer (default: 4096)\n"
#endif
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
	char cwd[1024];
	if (getcwd (cwd, 1024))
	  levelfile = std::string(cwd) + "/" + levelfile;
      } else {
	std::cout << "Wrong argument: '" << argv[i] << "'" << std::endl;
	std::cout << "A levelfile is already given," << std::endl;
	exit(EXIT_FAILURE);
      }
    }
}

void
PingusMain::init_pingus()
{
  if (verbose) 
    {
      std::cout << "-----------------------------------------------------------------\n" 
		<< " Verbosity set to: " << verbose  << "\n"
		<< " If you don't like to get lots of debug messages, than set the\n"
		<< " verbosity down to 0, like this:\n\n" 
		<< "   $ ./pingus --verbose 0\n" 
		<< "-----------------------------------------------------------------\n" 
		<< std::endl;
    }

  fps_counter.init();
  console.init();
  /*
  if (preload_data)
    {
      load_resources("global";
      load_resources("game";
      load_resources("textures";
      load_resources("editor";
      load_resources("fonts";
      load_resources("menu";
      load_resources("pingus";
      load_resources("traps";
    }
  */
}

void
PingusMain::load_resources(std::string filename)
{
  // Loading all resources
  /*
  {
    CL_ResourceManager* res = global";

    loading_screen.draw_progress("..:: " + filename + " ::..",
				 0.0);
    res->load_all_resources();
    loading_screen.draw_progress("..:: " + filename + " ::..",
				 1.0);

    list<std::string>* liste = res->get_resources_of_type("surface");
    list<std::string>::size_type count = 0;
    list<std::string>::size_type list_size = liste->size();
    
    for(std::list<std::string>::iterator i = liste->begin(); i != liste->end(); i++)
      {
	// Loading surfaces to /dev/null...
	CL_Surface::load(i->c_str(), res);
	
	count++;
	if (count  % (list_size / 25) == 0)
	  {
	    loading_screen.draw_progress("..:: " + filename + " ::..",
					 (float)(count) / list_size);
	  }
	CL_System::keep_alive();
      }
    loading_screen.draw_progress("..:: " + filename + " ::..", 1.0);
  }
    */
}
      
// Get all filenames and directories
void
PingusMain::get_filenames()
{
  System::init_directories();

#ifndef WIN32
  if (!pingus_datadir_set)
    {
      if (System::exist("../data/data/global.scr"))
	{
	  if (verbose) std::cout << "Assuming that you haven't installed pingus" << std::cout;
      
	  pingus_datadir = "../data/";
	  pingus_datadir_set = true;
	}
      else if (System::exist("./data/data/global.scr"))
	{
	  if (verbose) std::cout << "Assuming that you are running the binary version" << std::endl;
      
	  pingus_datadir = "./data/"; 
	  pingus_datadir_set = true; 
	}
      else
	{
	  if (verbose)
	    std::cout << "Using intern macro PINGUS_DATADIR" << std::endl;
      
	  pingus_datadir = PINGUS_DATADIR;
	  pingus_datadir_set = true;
	}
    }
#else /* !WIN32 */
  //If the User uses Windows, the Datadir is always the Subdirectory "data"
  pingus_datadir_set = true;
  pingus_datadir = "data\\";
#endif /* !WIN32 */

  if (verbose)
    std::cout << "pingus_datadir: " << pingus_datadir << std::endl;

  if (System::exist (pingus_datadir))
    {
      if (verbose > 1)
	std::cout << "Pingus Datadir exist, all looks ok" << std::endl;
      System::change_dir (pingus_datadir);
    } 
  else 
    {
      std::cout << "FIXME: Couldn't find `global.scr', please set the enviroment variable\n"
		<< "PINGUS_DATADIR to the path of the file `pingus.scr' or use the\n"
		<< "-d option." << std::endl;
      exit(EXIT_FAILURE);
    }
  
  // First we try to open the file which was given, if that is not
  // there then we try again with filename+".plf". If still no success
  // we try to search for that file in the pingus_path, if its not
  // there, then we try to search for the filename+".plf" in the
  // pingus_path.
  /* This code really shouldn't be here
  std::string custom_levelfile = levelfile;
  bool levelfile_not_found = false;
  if (!custom_levelfile.empty() && !System::exist(custom_levelfile))
    {
      if (System::exist(custom_levelfile + ".plf"))
	{
	  custom_levelfile += ".plf";
	}
      else
	{
	  if (verbose)
	    std::cout << "Levelfile not found, trying fallbacks" << std::endl;
	  
	  // Search for the level in the datadir
	  custom_levelfile = find_file(pingus_datadir, "/levels/" + levelfile);
	  levelfile_not_found = !System::exist(custom_levelfile.c_str());

	  if (levelfile_not_found) 
	    {
	      custom_levelfile = find_file(pingus_datadir, "/levels/" + levelfile + ".plf");
	      levelfile_not_found = !System::exist(custom_levelfile.c_str());
	    }

	  if (levelfile_not_found)
	    {
	      std::cout << "Couldn't find level file: \"" << levelfile << "\"" << std::endl;
	      exit(EXIT_FAILURE);
	    }
	}
    }

  levelfile = custom_levelfile;
  
  if (verbose)
    std::cout << "Pingus Level File: " << levelfile << std::endl;
  */
}
  
void
PingusMain::init(int argc, char* argv[])
{
  char c;

  PingusMain::quick_check_args(argc, argv);
  PingusMain::read_rc_file();
  PingusMain::check_args(argc, argv);

  if (verbose) 
    {
      std::cout << PACKAGE << " version " << VERSION << std::endl;
    }

  // Translate the geometry std::string to some int's
  if (!resolution.empty())
    {
      if (sscanf(resolution.c_str(), "%d%c%d", &screen_width, &c, &screen_height) != 3) 
	{
	  std::cout << "Resolution std::string is wrong, it should be like: \n" 
	       << "\"640x480\" or \"800x600\"" << std::endl;
	  exit(EXIT_FAILURE);
	}
    }
  // Loading data and initialisising 
  get_filenames();
  //  register_actions(); 
}

void
PingusMain::init_clanlib()
{
  // Init ClanLib
  if (verbose) 
    std::cout << "Init ClanLib" << std::endl;

  CL_SetupDisplay::init();

  if (sound_enabled || music_enabled) 
    {
      if (verbose)
	std::cout << "Init Sound" << std::endl;
#ifdef HAVE_LIBSDL_MIXER
      PingusSound::init (new PingusSoundReal ());
      //PingusSound::init (new PingusSoundDummy ());
#else
      std::cout << "Sound enabled, but not supported by binary." << std::endl;
      PingusSound::init (new PingusSoundDummy ());
#endif
    }
  else
    {
      if (verbose)
	std::cout << "Sound disabled" << std::endl;
      PingusSound::init (new PingusSoundDummy ());
    }

  if (verbose) 
    {
      std::cout << "Using resolution: " 
	   << screen_width << "x" << screen_height << std::endl;
    }

  // Initing the display
  CL_Display::set_videomode(screen_width, screen_height, 16, 
			    fullscreen_enabled, 
			    true); // allow resize
}

bool
PingusMain::do_lemmings_mode(void)
{
  if (verbose) {
    std::cout << _("PingusMain: Starting Main: ") << CL_System::get_time() << std::endl;
  }

  if (print_fps)
    Display::add_flip_screen_hook(&fps_counter);


  on_button_press_slot = CL_Input::sig_button_press.connect (CL_CreateSlot(&global_event, &GlobalEvent::on_button_press));
  on_button_release_slot = CL_Input::sig_button_release.connect (CL_CreateSlot(&global_event, &GlobalEvent::on_button_release));

  //pingus_story.display ();

  if (!levelfile.empty ())
    {
      PingusGame game;
      
      if (System::exist(levelfile))
	game.start_game(levelfile);
      else if (System::exist(levelfile + ".xml"))
	game.start_game(levelfile + ".xml");
      else if (System::exist("levels/" + levelfile + ".xml"))
	game.start_game("levels/" + levelfile);
      else
	{
	  std::cout << _("PingusMain: Levelfile not found, ignoring: ") << levelfile << std::endl;
	}
    }

  try  {
    PingusMenuManager menu;
    menu.display ();
    //PingusMenu menu; 
    //menu.select();
  }
  
  catch (CL_Error err) {
    std::string str;
    str = "CL_Error: ";
    str += err.message;
    PingusMessageBox box(str);
  }
  
  catch (PingusError err) {
    PingusMessageBox(" PingusError: " + err.get_message ());
  }

  CL_Input::sig_button_press.disconnect (on_button_press_slot);
  CL_Input::sig_button_release.disconnect(on_button_release_slot);

  return true;
}

bool
PingusMain::do_worms_mode(void)
{
  std::cout << "do_worms_mode() not implemented" << std::endl;
  std::cout << "Have a look at Beans." << std::endl;
  return false;
}

PingusMain::GameMode
PingusMain::select_game_mode(void)
{
  return Lemmings;
}

int
PingusMain::main(int argc, char** argv)
{
  bool quit = false;

  // Register the segfault_handler
  //signal(SIGSEGV, segfault_handler);

  try 
    {
      init(argc, argv);
      init_clanlib();
      init_pingus();	
      
      if (!intro_disabled && levelfile.empty()) 
	{
	  //intro.draw();
	}
      
      while (!quit) 
	{
	  switch (select_game_mode()) {
	  case PingusMain::Lemmings: // Select the Lemmings(tm) like mode
	    quit = do_lemmings_mode();
	    break;
	  case PingusMain::Worms:  // Select the Worms(tm) like mode
	    quit = do_worms_mode();
	    break;
	  }
	}
  }
  
  catch (CL_Error err) {
    std::cout << _("Error caught from ClanLib: ") << err.message << std::endl;
  }

  catch (PingusError err) {
    std::cout << _("Error caught from Pingus: ") << err.get_message () << std::endl;
  }

  catch (std::bad_alloc a) {
    std::cout << _("Pingus: Out of memory!") << std::endl;
  }

  catch (std::exception a) {
    std::cout << _("Pingus: Standard exception caught!:\n") << a.what() << std::endl;
  }

  catch (...) {
    std::cout << _("Pingus: Unknown throw caught!") << std::endl;
  }

  return 0;
}

/* EOF */
