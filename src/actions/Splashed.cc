//  $Id: Splashed.cc,v 1.20 2001/11/18 00:03:29 grumbel Exp $
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

#include "../Pingu.hh"
#include "../PingusResource.hh"
#include "../PingusSound.hh"
#include "../algo.hh"
#include "Splashed.hh"

Splashed::Splashed(void)
{
}

void
Splashed::init(void)
{
  sprite = Sprite ("Pingus/splat0", "pingus", 30.0f,
		   Sprite::NONE, Sprite::ONCE);
  sprite.set_align_center_bottom ();
  
  sound_played = false;
  particle_thrown = false;
}

void
Splashed::update(float delta)
{
  sprite.update (delta);

  if (!particle_thrown)
    {
      particle_thrown = true;
      PingusSound::play("splat.wav");
    }

  if (sprite.finished ())
  {
    pingu->set_status(PS_DEAD);
  }
}

void 
Splashed::draw_offset(int x_of, int y_of, float s)
{
  sprite.put_screen (pingu->get_x () + x_of,
		     pingu->get_y () + y_of);
}

/* EOF */
