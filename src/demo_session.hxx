//  $Id: demo_session.hxx,v 1.1 2002/10/03 01:02:12 grumbel Exp $
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

#include "screen.hxx"

class Server;
class XMLPDF;

/** A DemoSession is analog to a GameSession, but instead of loading a
    level and letting the player play a game, a demo file will be
    loaded and the level will be played automatically. */
class DemoSession : public Screen
{
private:
  XMLPDF*     pdf;
  Server*     server;
  DemoPlayer* demo_player;
public:
  /** @param filename the complete filename of the demo file */
  DemoSession(const std::string& filename);
  ~DemoSession();

  /** Draw this screen */
  bool draw (GraphicContext& gc);

  /** Pass a delta to the screen */
  void update (const GameDelta& delta);
  
private:
  DemoSession (const DemoSession&);
  DemoSession& operator= (const DemoSession&);
};

#endif

/* EOF */
