//   $Id: Pingus.cc,v 1.4 2000/02/12 12:00:33 grumbel Exp $
//    ___
//   |  _\ A free Lemmings clone
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
#ifndef WIN32
#include <config.h>
#include <getopt.h>
#else /* !WIN32 */
#include "win32/config.h"
#include "win32/getopt.h"
#endif /* !WIN32 */

#include <ClanLib/core.h>
#include <ClanLib/magick.h>

#include "Client.hh"
#include "Server.hh"
#include "TrueServer.hh"
#include "Pingus.hh"
#include "algo.hh"
#include "globals.hh"
#include "PingusSound.hh"
#include "PingusGame.hh"
#include "Playfield.hh"
#include "PingusError.hh"

#include "PingusMenu.hh"
#include "PingusMessageBox.hh"
#include "particles/GroundParticle.hh"

PingusMain::PingusMain()
{
  std::string temp;
  resolution = "640x480";

  temp = string(PACKAGE) + " " + VERSION + " - A free Lemmings clone";
  temp += " - http://pingus.seul.org";

  title = strdup(temp.c_str());

  pingus_datadir_set = false;
}

PingusMain::~PingusMain()
{
  free(title);
}

char* 
PingusMain::get_title()
{
  return title;
}

void
PingusMain::init_modules()
{
  CL_SetupCore::init();
  // CL_SetupCore::init_sound();  
  CL_SetupMagick::init();
  //CL_SetupCore::init_display();
}

void PingusMain::deinit_modules()
{
  CL_SetupCore::deinit();   
}

// FIXME: This sucks, it has lots of memory leaks!!!
void
PingusMain::read_rc_file(void)
{
  char*    homedir = getenv("HOME");
  std::string   rcfile;
  char     line[1024];
  char     option[1024];
  char     argument[1024];
  int      lineno = 0;
  char*    pargv[100];
  int      pargc = 1;

  pargv[0] = "dummy"; // strdup(PACKAGE);

  if (!homedir) {
    rcfile = ".pingus/options";
  } else {
    rcfile = string(homedir) + "/.pingus/options";
  }

  ifstream in(rcfile.c_str());

  if (!in) {
    std::cout << "PingusMain: No config file found at: " << rcfile << std::endl;
    return;
  } 
  
  std::cout << "PingusMain: Parsing config file" << std::endl;
  while(true) {
    in.getline(line, 1023);
    ++lineno;
    if(in.eof()) {
      break;
    }
    
    remove_comments(line);
    if (!line_empty(line)) {
      if (sscanf(line, "%s %s", option, argument) == 2) {
	pargv[pargc++] = strdup((string("--") + option).c_str());
	pargv[pargc++] = strdup(argument);
      } else {
	pargv[pargc++] = strdup((string("--") + option).c_str());
      }
    }
  }
  
  if (pargc == 1) {
    check_args(pargc, pargv);
  } else { 
    check_args(pargc, pargv);
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

  struct option long_options[] =
  {
    {"enable-music",    no_argument,       0, 'm'},
    {"enable-sound",    no_argument,       0, 's'},
    {"enable-gimmicks", no_argument,       0, 'i'},
    {"enable-cursor",   no_argument,       0, 'c'},
    {"disable-intro",   no_argument,       0, 'n'},
    {"play-demo",       required_argument, 0, 'd'},
    {"record-demo",     required_argument, 0, 'r'},
    {"speed",           required_argument, 0, 't'},
    {"datadir",         required_argument, 0, 'd'},
    {"level",           required_argument, 0, 'l'},
    {"help",            no_argument,       0, 'h'},
    {"version",         no_argument,       0, 'V'},
    {"verbose",         required_argument, 0, 'v'},
    {"print-fps",       no_argument,       0, 'b'},
    {"sound-specs",     required_argument, 0, 'S'},
    {"geometry",        required_argument, 0, 'g'},
    {"editor",          no_argument,       0, 'e'},
    {"quick-play",      no_argument,       0, 'q'},
    {"debug-actions",   no_argument,       0, 129},
    {"fs-preload",      no_argument,       0, 130},
    {"fast",            no_argument,       0, 132},
    {"disable-previews",no_argument,       0, 133}, // FIXME: is the number stuff correct?
    {"maintainer-mode", no_argument,       0, 134},
    {"enable-forces",   no_argument,       0, 135},
    {"enable-uactions", no_argument,       0, 136},
    {"oos-scrolling",   no_argument,       0, 137},
    {0, 0, 0, 0}
  };

  while(true) {
    c = getopt_long(argc, argv, "r:p:smv:d:l:hVp:bxS:g:it:cq", long_options, &option_index);
    
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
      levelfile = optarg;
      if (verbose) std::cout << "PingusMain:check_args: Levelfile = " << levelfile << std::endl;
      break;
    case 't': // -t, --set-speed
      game_speed = atoi(optarg);
      if (verbose) 
	cout << "PingusMain:check_args: Game Speed = " << game_speed << std::endl;
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
      pingus_soundfile = optarg;
      if (verbose) 
	cout << "check_args: Sound File = " << pingus_soundfile << std::endl;
      break;
    case 'm': // -m, --enable-music
      if (verbose) std::cout << "check_args: Music enabled" << std::endl;
      music_enabled = true;
      break;
    case 'd': // -d, --datadir
      pingus_datadir = optarg;
      pingus_datadir_set = true;
      if (verbose)
	cout << "check_args: Pingus Data Dir = " << pingus_datadir << std::endl;
      break;
    case 'n':
      intro_disabled = true;
      break;
    case 'V':
      std::cout << "Pingus" << " version " << VERSION << std::endl;
      std::cout <<
	"\n"
	"Copyright (C) 1998 Ingo Ruhnke <grumbel@gmx.de>\n"
	"There is NO warranty.  You may redistribute this software\n"
	"under the terms of the GNU General Public License.\n"
	"For more information about these matters, see the files named COPYING."
	   << std::endl;

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
	cout << "Using demofile: " << demo_file << std::endl;
      break;
    case 'v':
      sscanf(optarg, "%d", &verbose);
      std::cout << "Pingus: Verbose level is " << verbose << std::endl;
      break;
    case 129: // --debug-actions
      debug_actions = true;
      break;
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
      std::cout << "---------------------------------" << endl
	   << "--- Maintainer Mode activated ---" << endl
	   << "---------------------------------" << std::endl; 
      maintainer_mode = true;
      break;
    case 135:
      force_vectors = true;
      break;
    case 136:
      unlimited_actions = true;
      break;
    case 137:
      auto_scrolling = true;
      break;
    default:
      
      std::cout << "Unknow char: " << c << endl << std::endl;
      std::cout << "Usage: " << argv[0] << " [OPTIONS]... [LEVELFILE]" << std::endl;
      std::cout <<
	"\n"
	"Options:\n"
	"   -g, --geometry {width}x{height}\n"
	"                            Set the resolution for pingus (default: 640x480)\n"
	"   -h, --help               Displays this screen\n"
	"   --disable-intro          Disable intro\n"
	"   -s, --enable-sound       Enable sound\n"
	"   -m, --enable-music       Enable music\n"
	"   -i, --enable-gimmicks    Enable some buggy development stuff\n"
	"   -S, --sound-specs FILE   Use files mentioned in FILE\n"
       	"   -d, --datadir PATH       Set the path to load the data files to `path'\n"
	"   -l, --level FILE         Load a custom level from `file'\n"
	"   -t, --speed SPEED        Set the game speed (0=fastest, >0=slower)\n"
	"   -b, --print-fps          Prints the fps to stdout\n"
	"   -v, --verbose            Print some more messages to stdout, can be set\n"
	"                            multible times to increase verbosity\n"
	"   -V, --version            Prints version number and exit\n"
	"   -r, --record-demo FILE   Record a demo session to FILE\n"
	"   -p, --play-demo FILE     Plays a demo session from FILE\n"
	"   --fs-preload             Preload all Levelpreviews\n"
	"   --fast                   Disable some cpu intensive features (not implemented\n"
	"   --disable-previews       Disables all level preview in the level selector\n"
	"   --maintainer-mode        Enables some features, only interesting programmers\n"
	"   -e, --editor             Launch the Level editor (experimental)\n"
	   << std::endl;
      exit(EXIT_SUCCESS);
      break;
    }

  }

  // Treating non option arguments
  for(int i = optind; i < argc; ++i) {
    if (levelfile.empty()) {
      levelfile = argv[i];
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
      std::cout << "------------------------------------------------------\n" 
		<< " Verbosity set to: " << verbose  << "\n"
		<< " If you don't like lots of debug messages set the\n"
		<< "verbosity down to 0, like this:\n" 
		<< "$ ./pingus --verbose 0" << std::endl;
    }
}

// Get all filenames and directories
void
PingusMain::get_filenames()
{
  char* home = getenv("HOME");
  
  if (verbose)
    clog << "Getting filenames..." << std::endl;
  
  if (home) 
    {
      pingus_homedir = home;
      pingus_homedir += "/.pingus/";
    }
  else 
    {
      pingus_homedir = "user/";
    }
  
  if (exist(pingus_homedir)) 
    {
      if (verbose) std::cout << "PingusMain: pingus_homedir = " << pingus_homedir << std::endl;
    } 
  else 
    {
      if (verbose) std::cout << "PingusMain: Creating directory: " << pingus_homedir << "... " << std::flush;
      
      if (mkdir(pingus_homedir.c_str(), S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP) == 0
	  && mkdir((pingus_homedir + "levels/").c_str(), S_IRUSR | S_IWUSR | S_IXUSR | S_IWGRP | S_IXGRP) == 0
	  && mkdir((pingus_homedir + "stat").c_str(), S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP) == 0)
	{
	  if (verbose) std::cout << "finished" << std::endl;
	}
      else 
	{
	  if (verbose) std::cout << "error" << std::endl;
	  if (verbose) std::cout << "---------------------------------------------------------------" << std::endl;
	  perror(pingus_homedir.c_str());
	  if (verbose) std::cout << "---------------------------------------------------------------" << std::endl;
	}
    }
  
#ifndef WIN32
  char* env;

  if (pingus_datadir_set)
    pingus_datadir += ":";
  
  pingus_datadir += pingus_homedir;

  // Search different places for the datadir path, if none can be
  // found, then fail.
  // Bug: could also check for relative or absolute pathnames
  if (pingus_datadir_set) 
    {
      pingus_datadir += ":";
      pingus_datadir += PINGUS_DATADIR;
    } 
  else 
    {
      env = getenv("PINGUS_DATADIR");
    
      if (env) 
	{
	  if (verbose > 1)
	    std::cout << "Using envar $PINGUS_DATADIR" << std::endl;
	
	  pingus_datadir = env;
	  pingus_datadir_set = true;
	}

      // Correct the path name, if no slash is pressent
      //add_slash(pingus_datadir);
  
      if (exist("../data/data/global.dat")) 
	{
	  if (verbose > 1)
	    std::cout << "Assuming that you haven't installed pingus, overriding current value." << std::endl;
	
	  pingus_datadir += ":../data/";
	}

      if (!pingus_datadir_set) 
	{
	  if (verbose > 1)
	    std::cout << "Using intern macro PINGUS_DATADIR" << std::endl;
	
	  pingus_datadir += ":";
	  pingus_datadir += PINGUS_DATADIR;
	}
    }

  if (verbose)
    std::cout << "pingus_datadir: " << pingus_datadir << std::endl;

  // FIXME: find_file() sucks.
  global_datafile   = find_file(pingus_datadir, "data/global.dat");
  pingus_datafile   = find_file(pingus_datadir, "data/pingus.dat");
  
  if (exist(pingus_datafile)) 
    {
      if (verbose > 1)
	cout << "Pingus Datadir exist, all looks ok" << std::endl;
    } 
  else 
    {
      std::cout << "Pingus Datafile: " << pingus_datafile << endl << std::endl;
      std::cout << "Couldn't find `global.dat', please set the enviroment variable\n"
	   << "PINGUS_DATADIR to the path of the file `pingus.dat' or use the\n"
	   << "-p option." << std::endl;
      exit(EXIT_SUCCESS);
    } 

#else /* !WIN32 */
  //If the User uses Windows, the Datadir is always the Subdirectory "data"
  pingus_datadir_set = true;
  pingus_datadir = "data\\";
  global_datafile = pingus_datadir + "data\\global.dat";
  pingus_datafile = pingus_datadir + "data\\pingus.dat";
#endif /* !WIN32 */
  
  if (sound_enabled) {
    if (pingus_soundfile.empty())
      pingus_soundfile = find_file(pingus_datadir, "sounds/sound.specs");
    std::cout << "PingusMain: Reading Sound Specs" << std::endl;

    PingusSound::init(pingus_soundfile);
    std::cout << "PingusMain: Reading Sound Specs finished" << std::endl;
  }

  std::string custom_levelfile = levelfile;
  if (!levelfile.empty()) 
    {
      if (!exist(custom_levelfile)) 
	{
	  if (verbose)
	    std::cout << "Levelfile not found, trying fallbacks" << std::endl;
	  
	  // Search for the level in the datadir
	  custom_levelfile = find_file(pingus_datadir, "/levels/" + custom_levelfile);
	  if (!exist(custom_levelfile.c_str())) 
	    {
	      std::cout << "Couldn't find level file: \"" << custom_levelfile << "\"" << std::endl;
	      exit(EXIT_FAILURE);
	    }
	}
    }
  levelfile = custom_levelfile;
  
  if (verbose)
    std::cout << "Pingus Level File: " << levelfile << std::endl;
}
  
void
PingusMain::init(int argc, char* argv[])
{
  char c;

  argv_0 = argv[0];

  PingusMain::read_rc_file();
  PingusMain::check_args(argc, argv);

  if (verbose) 
    {
      std::cout << PACKAGE << " version " << VERSION << std::endl;
    }

  // Translate the geometry std::string to some int's
  if (sscanf(resolution.c_str(), "%d%c%d", &screen_width, &c, &screen_height) != 3) 
    {
      cerr << "Resolution std::string is wrong, it should be like: \n" 
	   << "\"640x480\" or \"800x600\"" << std::endl;
      exit(EXIT_FAILURE);
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
  // CL_System::init_display();
  CL_SetupCore::init_display();

  if (sound_enabled || music_enabled) 
    {
      if (verbose)
	cout << "Init ClanSound" << std::endl;
      //CL_System::init_sound();
    }

  if (verbose) 
    {
      std::cout << "Using resolution: " 
	   << screen_width << "x" << screen_height << std::endl;
    }

  // Setting the display resolution
  CL_Display::set_videomode(screen_width, screen_height, 16);
}

bool
PingusMain::do_lemmings_mode(void)
{
  if (verbose) 
    {
      std::cout << "PingusMain: Starting Main: " << CL_System::get_time() << std::endl;
    }

  if (!levelfile.empty()) 
    {
      PingusGame game;
      game.start(levelfile);    
    }

  try 
    {
      PingusMenu menu; 
      menu.select();
  }
  
  catch (CL_Error err) 
    {
      std::string str;
      str = "CL_Error: ";
      str += err.message;
      PingusMessageBox box(str);
    }
  
  catch (PingusError err) 
    {
      PingusMessageBox(" PingusError: " + err.message);
    }
  
  std::cout << "Pingus exit normaly" << std::endl;
  
  return true;
}

bool
PingusMain::do_worms_mode(void)
{
  std::cout << "do_worms_mode() not implemented" << std::endl;
  return false;
}

PingusMain::GameMode
PingusMain::select_game_mode(void)
{
  // dummy, should replace with a nice menu
  return Lemmings;
}

int
PingusMain::main(int argc, char** argv)
{
  bool quit = false;

  try 
    {
      init(argc, argv);
      init_clanlib();
      init_pingus();	
      
      if (!intro_disabled && levelfile.empty()) 
	{
	  intro.draw();
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
    std::cout << "Error caught from ClanLib: " << err.message << std::endl;
  }

  catch (PingusError err) {
    std::cout << "Error caught from Pingus: " << err.message << std::endl;
  }

  catch (bad_alloc a) {
    std::cout << "Pingus: Out of memory!" << std::endl;
  }

  catch (exception a) {
    std::cout << "Pingus: Standard exception caught!:\n" << a.what() << std::endl;
  }

  catch (...) {
    std::cout << "Pingus: Unknown throw caught!" << std::endl;
  }

  return 0;
}

void
PingusMain::remove_comments(char* line)
{
  for (int i=0; line[i] != '\0'; ++i) {
    if (line[i] == '#') {
      line[i] = '\0';
      break;
    }
  }
}

bool 
PingusMain::line_empty(char* line)
{
  for (int i=0; line[i] != '\0'; i++) {
    if (!isspace(line[i]))
      return false;
  }
  return true;
}

/* EOF */


