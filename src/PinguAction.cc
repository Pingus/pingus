//  $Id: PinguAction.cc,v 1.7 2000/08/03 10:31:17 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
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

#include <cassert>
#include "PingusResource.hh"
#include "PinguAction.hh"

CL_ResourceManager* PinguAction::local_res_p;  

PinguAction::PinguAction()
{
  is_finished = false;
  font = PingusResource::load_font("Fonts/numbers","fonts");
  surface = 0;
  active = false;
  environment = (PinguEnvironment)land;
}

PinguAction::~PinguAction()
{
}

void
PinguAction::set_pingu(Pingu* pingu_data)
{
  pingu = pingu_data;
  assert(pingu);
  init();
}

// Checks if the pingu action needs to catch another pingu (needed for 
// example by the blocker)
bool 
PinguAction::need_catch() 
{ 
  return false; 
}

// Wrapper around the colmap, to get the pixels infront of the pingu,
// from is current position
int
PinguAction::rel_getpixel(int x, int y)
{
  return pingu->get_world()->get_colmap()->getpixel(pingu->x_pos + (x * pingu->direction), (pingu->y_pos) - y);
}

void
PinguAction::draw_offset(int x, int y, float s)
{
  // FIXME: This can be removed
  if (pingu->get_status() == (dead || exited)) 
    {
      std::cout << "PinguAction: This should never be reached, please report." << std::endl;
      assert(0);
      return;
    }
  
  if (s == 1.0) 
    {
      if (is_multi_direct) 
	{
	  surface->put_screen(pingu->x_pos + x + x_offset(), pingu->y_pos + y + y_offset(), 
			      ++counter + ((pingu->direction.is_left()) ? 0 : counter.size()));
	} 
      else 
	{
	  surface->put_screen(pingu->x_pos + x + x_offset(), pingu->y_pos + y + y_offset(),
			      ++counter);
	}
    } 
  else 
    {
      if (is_multi_direct) 
	{
	  surface->put_screen(int((pingu->x_pos + x + x_offset()) * s), int((pingu->y_pos + y + y_offset()) * s), 
			      s, s, ++counter + ((pingu->direction.is_left()) ? 0 : counter.size()));
	} 
      else
	{
	  surface->put_screen(int((pingu->x_pos + x + x_offset()) * s), int((pingu->y_pos + y + y_offset()) * s),
			      s, s, ++counter);
	}
    }
}

PinguEnvironment
PinguAction::get_environment()
{
  return environment;
}

int
PinguAction::SetResourceManager(CL_ResourceManager* res)
{
  local_res_p = res;
  return true; //
}

CL_ResourceManager*
PinguAction::local_res(void)
{
  return local_res_p;
}

ActionType
PinguAction::get_type(void)
{
  return (ActionType)ONCE;
}

const std::string&
PinguAction::name()
{
  return action_name;
}

bool
PinguAction::is_active(void)
{
  return active;
}

int
PinguAction::x_offset(void)
{
  return -16;
}

int
PinguAction::y_offset(void)
{
  return -32;
}

PinguAction* 
PinguAction::create(void)
{
  return allocate();
}

/* EOF */
