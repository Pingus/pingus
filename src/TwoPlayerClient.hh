//  $Id: TwoPlayerClient.hh,v 1.1 2001/04/13 22:17:46 grumbel Exp $
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

#ifndef TWOPLAYERCLIENT_HH
#define TWOPLAYERCLIENT_HH

#include "Controller.hh"
#include "boost/smart_ptr.hpp"
#include "boost/dummy_ptr.hpp"
#include "Server.hh"
#include "GuiObj.hh"
#include "Cursor.hh"
#include "ButtonPanel.hh"

class TwoPlayerClient
{
private:
  boost::shared_ptr<Controller> controller1;
  boost::shared_ptr<Controller> controller2;

  boost::dummy_ptr<Server> server;

  boost::shared_ptr<Cursor> cursor1;
  boost::shared_ptr<Cursor> cursor2;

  boost::shared_ptr<ButtonPanel> button_panel1;
  boost::shared_ptr<ButtonPanel> button_panel2;
  
  std::vector<boost::shared_ptr<GuiObj> > gui_objs;
  typedef std::vector<boost::shared_ptr<GuiObj> >::iterator GuiObjIter;
  
  CL_Slot slot_left1_pressed;
  CL_Slot slot_left2_pressed;
  CL_Slot slot_right1_pressed;
  CL_Slot slot_right2_pressed;
  CL_Slot slot_right1_released;
  CL_Slot slot_right2_released;
public:
  TwoPlayerClient (boost::shared_ptr<Controller> arg_controller1, 
		   boost::shared_ptr<Controller> arg_controller2, 
		   boost::dummy_ptr<Server> s);
  ~TwoPlayerClient ();

  void draw ();
  void update (float delta);

  void on_left1_pressed (const CL_Vector& pos);
  void on_left2_pressed (const CL_Vector& pos);
  void on_right1_pressed (const CL_Vector& pos);
  void on_right2_pressed (const CL_Vector& pos);
  void on_right1_released (const CL_Vector& pos);
  void on_right2_released (const CL_Vector& pos);
};

#endif

/* EOF */
