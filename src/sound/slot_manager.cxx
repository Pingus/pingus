//  $Id: slot_manager.cxx,v 1.2 2003/04/19 10:23:19 torangan Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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

#include "slot_manager.hxx"

SlotEntry& find_slot(const std::string& name)
{

}

void
SlotManager::play_sound(const std::string& name)
{
  CL_SoundBuffer         * buffer;
  CL_SoundBuffer_Session sess;

  try {
    buffer = new CL_SoundBuffer (new CL_Sample(filename.c_str(), NULL), true);
    sess   = buffer -> prepare();
  } catch (const CL_Error & e) {
    perr(PINGUS_DEBUG_SOUND) << "Can't open file " << filename << " -- skipping\n"
			     << "  CL_Error: " << e.message << std::endl;
    return;
  }

  sess.set_volume(volume);
  sess.set_pan(panning);
  sess.set_looping(false);
  sess.play();
}

/* EOF */
