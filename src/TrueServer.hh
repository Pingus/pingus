//  $Id: TrueServer.hh,v 1.7 2001/04/01 18:00:37 grumbel Exp $
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

#ifndef TRUESERVER_HH
#define TRUESERVER_HH

#include "Server.hh"

///
class TrueServer : public Server
{
private:
  ///
  bool fast_forward;
  ///
  bool pause;
  ///
  unsigned int  last_time;
  ///
  int  local_game_speed;
  ///
  bool client_needs_redraw;
  ///
  float delta;
public:
  ///
  TrueServer(PLF* level_data);
  ///
  TrueServer();
  ///
  virtual ~TrueServer();

  ///
  void start(PLF* level_data);
  ///
  bool enough_time_passed(void);
  ///
  void update(void);
  ///
  bool needs_redraw();

  ///
  void set_fast_forward(bool value);
  ///
  bool get_fast_forward();
  
  ///
  void set_pause(bool);
  ///
  bool get_pause();
}///
;

#endif

/* EOF */
