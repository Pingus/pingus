//  $Id: generic_main.cxx,v 1.2 2002/06/28 22:21:59 grumbel Exp $
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


#include <iostream>

#include <ClanLib/Core/System/setupcore.h>
#include <ClanLib/jpeg.h>
#include <ClanLib/png.h>
#include <ClanLib/gui.h>

#include "system.hxx"
#include "pingus_main.hxx"

/* Headers needed for i18n / gettext */
#include <clocale>
#include <config.h>
#include "my_gettext.hxx"

GenericMain my_app; // notice this line. It creates the global instance.

char* 
GenericMain::get_title()
{
  return main_obj->get_title();
}

int 
GenericMain::main(int argc, char** argv)
{
  //std::ios::sync_with_stdio(false);
  //Uncomment for gcc-v3
  //std::ios_base::sync_with_stdio(false);

#ifdef HAVE_SETLOCALE
  setlocale (LC_ALL, "");
#endif

  // Init stuff needed by gettext
#ifdef HAVE_GETTEXT
  std::cout << _("Gettext: PACKAGE=") << PACKAGE  << _(", LOCALEDIR=") << LOCALEDIR << std::endl;
  // FIXME: We try to read from the install directory, even if we are
  // FIXME: not installed
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
#endif

  int ret_value;

  CL_SetupCore::init ();
  CL_SetupPNG::init ();
  CL_SetupJPEG::init ();
  CL_SetupGUI::init ();

  std::cout << std::string(_("Starting Pingus ")) + VERSION  << std::endl;
  std::cout << _("Build:") << __TIME__ << " " << __DATE__ << std::endl;


  if (System::basename(argv[0]) == "warpingus")
    {
      //main_obj = new WarMain();
      std::cout << "War no longer supported" << std::endl;
    }
  else if (System::basename(argv[0]) == "pingus")
    {
      main_obj = new PingusMain();
    }
  else
    {
      std::cout << _("GenericMain: Executable name \"") 
		<< argv[0] << _("\" unknown, default to Pingus mode") << std::endl;
      main_obj = new PingusMain();
    }

  ret_value = main_obj->main(argc, argv);

  CL_SetupGUI::deinit ();
  CL_SetupPNG::deinit();
  CL_SetupJPEG::deinit();
  CL_SetupCore::deinit();

  return ret_value;
}

/* EOF */



