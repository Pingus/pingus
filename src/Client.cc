//  $Id: Client.cc,v 1.1 2000/02/04 23:45:18 mbn Exp $
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

#include "globals.hh"
#include "Client.hh"
#include "FadeOut.hh"
#include "Playfield.hh"
#include "LevelInterrupt.hh"
#include "Result.hh"
#include "PingusLevelResult.hh"

Client::Client(Server* s)
{
  server = s;
  fast_forward = false;
  skip_frame = 0;
  do_replay = false;
  is_finished = false;

  event = new ClientEvent;
  event->client = this;
}

void
Client::start(string filename, PingusGameMode m)
{
  fast_forward = false;
  do_replay = false;
  is_finished = false;
  skip_frame = 0;

  mode = m;

  event->disable_event_handler();
  play_level(filename);
  event->enable_event_handler();

  FadeOut::random();

  cout << "Displaying results..." << flush;
  
  PingusLevelResult r(server->get_world()->get_result());
  r.draw();
  
  cout << "finished" << endl;
}

void
Client::start(string plf_filename, string psm_filename)
{
  play_level(plf_filename, psm_filename);
  do_replay = false;
  FadeOut::random();
}

void
Client::play_level(string plf_filename, string psm_filename)
{
  vector<GuiObj* > obj;
 
  if (verbose) {
    cout << "Constructing Window Objs: " << plf_filename << endl;
    cout << "Starting Level: '" << plf_filename << "'" << endl;
  }

  PLF*         plf          = new PLF(plf_filename);

  if (!psm_filename.empty()) {
    plf->set_psm_filename(psm_filename);
  }

  server->start(plf);

  playfield    = new Playfield(plf, server->get_world());
  button_panel = new ButtonPanel(plf);
  cursor       = new PingusCursor();
  pcounter     = new PingusCounter();
  time_display = new TimeDisplay();

  // debug_gui->set_world(server->get_world());
  button_panel->set_server(server);
  time_display->set_server(server);
  button_panel->set_client(this);
  pcounter->set_world(server->get_world());

  event->playfield = playfield;

  if (play_demo)
    server->set_demo(demo_file);
  
  if (record_demo)
    server->set_record_file(demo_file);
    
  if (verbose)
    cout << "Constructing Window Objs finished" << endl;

  if (music_enabled) {
    // Constructing Pingus Music
    music = new PingusMusic(plf);
    //  music->play();
  }

  playfield->set_clip_rect(0, 0, 
			   CL_Display::get_width(),
			   CL_Display::get_height());

  // Connect the button_panel with the playfield
  playfield->set_buttons(button_panel);
  //  playfield->set_pingu_info(pingu_info);
  playfield->set_server(server);
  playfield->set_client(this);

  if (verbose)
    cout << "Client: Entering main_loop. Startup time: " 
	 << CL_System::get_time() << " msec." << endl;

    // Adding all GuiObj's to the screen
  //  obj.push_back(pingu_info);
  obj.push_back(playfield);
  //obj.push_back(debug_gui);
  obj.push_back(pcounter);
  obj.push_back(time_display);
  obj.push_back(button_panel);

  if (cursor_enabled)
    obj.push_back(cursor);

  // Clear both buffers
  CL_Display::clear_display();
  CL_Display::flip_display();
  CL_Display::clear_display();
    
  unsigned int last_time = 0;
  int fps_count = 0;
  
  // Main Game Loop
  while (!server->is_finished()) {
    CL_System::keep_alive(); // Update all input
    
    server->let_move();
    
    if (CL_Keyboard::get_keycode(CL_KEY_O)) {

    }
    
    if (CL_Keyboard::get_keycode(CL_KEY_D)) {
      //debug_gui->toggle_debug();
      while(CL_Keyboard::get_keycode(CL_KEY_D))
	CL_System::keep_alive();
    }
    
    // Let the window move its content
    for(vector<GuiObj*>::size_type i=0; i < obj.size(); ++i) 
      obj[i]->let_move();
    
    if (!fast_forward || skip_frame >= 10) {
      skip_frame = 0;
      //    if (server->needs_redraw() && (skip_frame % 10 == 0)) {

      for(vector<GuiObj*>::size_type i=0; i < obj.size(); ++i) {
	obj[i]->draw_clipped();
      }
      CL_Display::flip_display();
      
      ++fps_count;
      // Print the fps rate every 2secs
      if (print_fps && (last_time + 1000 < CL_System::get_time())) {
	cout << "Client: " << fps_count << "fps" << endl;
	fps_count = 0;
	last_time = CL_System::get_time();
      }
    } else {
      ++skip_frame;
    }
  }
  
  // Delete all alocated objects
  //delete debug_gui;
  delete cursor;
  delete pcounter;
  delete time_display;
  delete button_panel;
  delete playfield;
  delete plf;
}

void
Client::set_fast_forward(bool value)
{
  fast_forward = value;
  skip_frame = 0;
  server->set_fast_forward(value);
}

bool
Client::get_fast_forward()
{
  return fast_forward;
}

Result
Client::get_result()
{
  return result;
}

bool
Client::replay()
{
  return do_replay;
}

void
Client::do_restart()
{
  do_replay = true;
  server->set_finished();
}

bool
Client::finished()
{
  return is_finished;
}

void
Client::set_finished()
{
  is_finished = true;
  server->set_finished();  
}

/* EOF */

