//  $Id: GlobalEvent.hh,v 1.5 2002/06/07 20:35:14 torangan Exp $
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

#ifndef GLOBALEVENT_HH
#define GLOBALEVENT_HH

class CL_Key;
class CL_InputDevice;


///
class GlobalEvent  
  //public CL_Event_ButtonPress, 
  //public CL_Event_ButtonRelease
{
private:
  
public:
  ///
  virtual void on_button_press(CL_InputDevice *device, const CL_Key &key);
  ///
  virtual void on_button_release(CL_InputDevice *device, const CL_Key &key);
  
};

///
extern GlobalEvent global_event;

#endif

/* EOF */
