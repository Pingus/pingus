//  $Id: event.hxx,v 1.1 2002/07/10 16:15:28 torangan Exp $
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

#ifndef HEADER_PINGUS_INPUT_EVENT_HXX
#define HEADER_PINGUS_INPUT_EVENT_HXX

namespace Input {

  enum EventType { ButtonEvent, MoveEvent, ActionAxisEvent };

  class Event {
    public:
    
      virtual ~Event () { }
      virtual EventType get_type ()=0;
  };

}

#endif

/* EOF */
