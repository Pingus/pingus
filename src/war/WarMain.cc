//  $Id: WarMain.cc,v 1.8 2001/05/18 19:17:09 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#ifndef WIN32
#  include <config.h>
#  include <getopt.h>
#else /* !WIN32 */
#  include "win32/config.h"
#  include "win32/getopt.h"
#endif /* !WIN32 */

#include <cstdio>

#include "WarGlobals.hh"
#include "WarGame.hh"
#include "WarMain.hh"
#include "../PingusError.hh"
#include "../my_gettext.hh"

/* Option flags and variables */
static struct option const long_options[] =
{
  {"verbose",   no_argument, 0, 'v'},
  {"help",      no_argument, 0, 'h'},
  {"version",   no_argument, 0, 'V'},
  {"file",      required_argument, 0, 'f'},
  {"geometry",  required_argument, 0, 'g'},
  {NULL, 0, NULL, 0}
};

char* 
WarMain::get_title()
{
  return _("Time for war - Lets fight!");
}

int
WarMain::main (int argc, char* argv[])
{
  //use_datafile = false;

  try 
    {
      decode_switches(argc, argv);

      CL_SetupDisplay::init();

      // Initing the display
      CL_Display::set_videomode(WarGlobals::screen_width, 
				WarGlobals::screen_height, 
				16, 
				WarGlobals::fullscreen_enabled, 
				true); // allow resize

      WarGame game;
      game.display();
    }

  catch (CL_Error err) {
    std::cout << "CL_Error: " << err.message << std::endl;
  }

  catch (PingusError err) {
    std::cout << "PingusError: " << err.get_message () << std::endl;
  }

  return 0;
}

void 
WarMain::init_modules()
{
}

void 
WarMain::deinit_modules()
{
}

int  
WarMain::decode_switches (int argc, char **argv)
{
  int c;

  while ((c = getopt_long (argc, argv, 
			   "v"	/* verbose */
			   "h"	/* help */
			   "V"	/* version */
			   "g"  /* geometry */
			   , long_options, (int *) 0)) != EOF)
    {
      switch (c)
	{
	case 'V':
	  std::cout << "warpingus " <<  VERSION << std::endl;
	  exit (0);

	case 'h':
	  usage (0);

	case 'v':
	  WarGlobals::verbose += 1;
	  break;
	  
	case 'g':
	  if (sscanf(optarg, "%dx%d", 
		     &WarGlobals::screen_width, &WarGlobals::screen_height) != 3)
	    {
	      std::cout << "Resolution std::string is wrong, it should be like: \n" 
			<< "\"640x480\" or \"800x600\"" << std::endl;
	      exit(EXIT_FAILURE);
	    }
	  break;
	  
	default:
	  usage (EXIT_FAILURE);
	}
    }
  return 0;
}

void  
WarMain::usage(int status)
{
  std::cout << "warpingus - A free Worms clone\n"
	    << "Usage: warpingus [OPTION]... [LEVEL-FILE]...\n"
	    << "Options:\n"
	    << "  --verbose                  print more information\n"
	    << "  -h, --help                 display this help and exit\n"
	    << "  -V, --version              output version information and exit\n"
	    <<"   -g, --geometry {width}x{height}\n"
	    << std::endl;
  exit(status);
}

/* EOF */
