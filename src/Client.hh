//  $Id: Client.hh,v 1.29 2001/04/15 17:01:51 grumbel Exp $
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

#ifndef CLIENT_HH
#define CLIENT_HH

#include <string>

#include "boost/smart_ptr.hpp"
#include "boost/dummy_ptr.hpp"

#include "Server.hh"
#include "Result.hh"

#include "ButtonPanel.hh"
#include "TimeDisplay.hh"
#include "PinguInfo.hh"
#include "PingusCounter.hh"
#include "SmallMap.hh"
#include "HurryUp.hh"
#include "Controller.hh"

class Playfield;
class ButtonPanel;
class PingusCounter;
class HurryUp;

/** Bla bla.... rewrite this class... */
class Client
{
private:
  ///
  boost::shared_ptr<PLF> plf;
  ///
  Result result;
  ///
  boost::dummy_ptr<Server> server;
  ///
  PingusGameMode       mode;
  ///
  bool fast_forward;
  ///
  bool pause;
  ///
  int  skip_frame;
  ///
  bool do_replay;
  ///
  bool is_finished;

  ///
  std::vector<boost::shared_ptr<GuiObj> > obj;

  ///
  static bool gui_is_init;
  ///
  static boost::shared_ptr<ButtonPanel>   button_panel;
  ///
  static boost::shared_ptr<PingusCounter> pcounter;
  ///
  static boost::shared_ptr<Playfield>     playfield;
  ///
  static boost::shared_ptr<TimeDisplay>   time_display;
  ///
  static boost::shared_ptr<SmallMap>      small_map;
  /// 
  static boost::shared_ptr<HurryUp>       hurry_up;
  boost::shared_ptr<Controller>   controller;
  ///
  CL_Slot on_button_press_slot;
  ///
  CL_Slot on_button_release_slot;

  // Slots for controller handling
  CL_Slot slot_left_pressed;
  CL_Slot slot_left_released;
  CL_Slot slot_middle_pressed;
  CL_Slot slot_right_pressed;
  CL_Slot slot_right_released;
  CL_Slot slot_abort_pressed;
  CL_Slot slot_pause_pressed;
  CL_Slot slot_scroll_left_pressed;
  CL_Slot slot_scroll_right_pressed;
  CL_Slot slot_next_action_pressed;
  CL_Slot slot_previous_action_pressed;
  
  ///
  bool enabled;
public:
  ///
  Client(boost::shared_ptr<Controller> arg_controller, 
	 boost::dummy_ptr<Server> s);
  ///
  virtual ~Client();

  ///
  boost::dummy_ptr<Server>   get_server() { return server; }
  ///
  boost::shared_ptr<Playfield> get_playfield() { return playfield; }

  ///
  void start();
  ///
  void send_next_event();

  ///
  void play_level(boost::shared_ptr<PLF>);

  /// FIXME: Document me... or rewrite me
  void init_display();
  /// FIXME: Document me... or rewrite me
  void deinit_display();
  ///
  void resize_display();

  ///
  void set_fast_forward(bool value);
  ///
  bool get_fast_forward();

  ///
  bool get_pause();
  ///
  void set_pause(bool value);

  ///
  bool replay();
  ///
  void do_restart();
  ///
  bool finished();
  ///
  void set_finished();
  ///
  Result get_result();

  /** Draw all gui elements, etc. */
  void draw ();

  /** Update all parts of the world */
  void update (float delta);

  ///
  virtual void on_button_press(CL_InputDevice *device, const CL_Key &key);
  ///
  virtual void on_button_release(CL_InputDevice *device, const CL_Key &key);

  ///
  void on_mouse_button_press(const CL_Key &key);
  ///
  void on_mouse_button_release(const CL_Key &key);
  
  ///
  void on_keyboard_button_press(const CL_Key &key);
  ///
  void on_keyboard_button_release(const CL_Key &key);

  /// 
  void on_left_pressed (const CL_Vector& pos);
  void on_left_released (const CL_Vector& pos);
  void on_middle_pressed (const CL_Vector& pos);
  void on_right_pressed (const CL_Vector& pos);
  void on_right_released (const CL_Vector& pos);
  void on_abort_pressed (const CL_Vector& pos);
  void on_pause_pressed (const CL_Vector& pos);
  void on_scroll_left_pressed (const CL_Vector& pos);
  void on_scroll_right_pressed (const CL_Vector& pos);
  void on_next_action_pressed (const CL_Vector& pos);
  void on_previous_action_pressed (const CL_Vector& pos);
  ///
  void register_event_handler();
  ///
  void unregister_event_handler();

  ///
  void disable_event_handler();
  ///
  void enable_event_handler();
};

#endif

/* EOF */
