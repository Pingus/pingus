//  $Id: GenericMain.cc,v 1.1 2000/08/06 13:05:31 grumbel Exp $
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
#include <ClanLib/core.h>
#include <ClanLib/magick.h>

#include "System.hh"
#include "war/WarMain.hh"
#include "PingusMain.hh"
#include "GenericMain.hh"

GenericMain my_app; // notice this line. It creates the global instance.

void
GenericMain::init_modules()
{
  CL_SetupCore::init();
  CL_SetupMagick::init();
}

void
GenericMain::deinit_modules()
{
  CL_SetupCore::deinit();
}

char* 
GenericMain::get_title()
{
  return main_obj->get_title();
}

int 
GenericMain::main(int argc, char* argv[])
{
  std::cout << "Starting..." << std::endl;

  if (System::basename(argv[0]) == "warpingus")
    {
      main_obj = new WarMain();
    }
  else if (System::basename(argv[0]) == "pingus")
    {
      main_obj = new PingusMain();
    }
  else
    {
      std::cout << "GenericMain: Executable name \"" 
		<< argv[0] << "\" unknown, default to Pingus mode" << std::endl;
      main_obj = new PingusMain();
    }

  return main_obj->main(argc, argv);
}

/* EOF */
