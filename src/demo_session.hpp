//  $Id$
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

#ifndef HEADER_PINGUS_DEMO_SESSION_HXX
#define HEADER_PINGUS_DEMO_SESSION_HXX

#include "gui/gui_screen.hpp"


class Server;
class XMLPDF;
class DemoPlayer;
class PingusCounter;

/** A DemoSession is analog to a GameSession, but instead of loading a
    level and letting the player play a game, a demo file will be
    loaded and the level will be played automatically. */
class DemoSession : public GUIScreen
{
private:
  XMLPDF*     pdf;
  Server*     server;
  DemoPlayer* demo_player;

  // GUI stuff
  PingusCounter* pcounter;
public:
  /** @param filename the complete filename of the demo file */
  DemoSession(const std::string& filename);
  ~DemoSession();

  /** Draw this screen */
  void draw_background(DrawingContext& gc);

  /** Pass a delta to the screen */
  void update(float delta);

private:
  DemoSession (const DemoSession&);
  DemoSession& operator= (const DemoSession&);
};


#endif

/* EOF */
