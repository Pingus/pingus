//  $Id: Client.cc,v 1.28 2000/06/26 06:45:59 grumbel Exp $
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

#include <config.h>
#include "globals.hh"
#include "Client.hh"
#include "FadeOut.hh"
#include "Playfield.hh"
#include "LevelInterrupt.hh"
#include "System.hh"
#include "Result.hh"
#include "algo.hh"
#include "Timer.hh"
#include "PingusResource.hh"
#include "Display.hh"
#include "PingusLevelResult.hh"
#include "PingusSound.hh"
#include "PingusError.hh"

bool Client::gui_is_init;
ButtonPanel*   Client::button_panel;
PingusCounter* Client::pcounter;
Playfield*     Client::playfield;
TimeDisplay*   Client::time_display;
SmallMap*      Client::small_map;

Client::Client(Server* s)
{

  playfield    = 0;
  button_panel = 0;
  pcounter     = 0;
  small_map    = 0;
  time_display = 0;


  player = 0;
  server = s;
  fast_forward = false;
  pause = false;
  skip_frame = 0;
  do_replay = false;
  is_finished = false;

  event = new ClientEvent;
  event->client = this;
}

Client::~Client()
{
  std::cout << "Client:~Client" << std::endl;
  deinit_display();
  delete event;
}

void
Client::start(DemoPlayer* player)
{
  assert(player);
  this->player = player;

  std::string demo_level_file = find_file(pingus_datadir, "levels/" + player->get_levelname());

  std::cout << "Demo_level_file: " << demo_level_file << std::endl;

  play_level(demo_level_file);
}

void
Client::start(std::string filename, PingusGameMode m)
{
  fast_forward = false;
  pause = false;
  do_replay = false;
  is_finished = false;
  skip_frame = 0;

  mode = m;

  play_level(filename);

  FadeOut::random();

  if (verbose) std::cout << "Displaying results..." << CL_System::get_time()  << std::flush;
  
  PingusLevelResult r(server->get_world());
  r.draw();
  
  if (verbose) std::cout << "finished " << CL_System::get_time()  << std::endl;
}

void
Client::start(std::string plf_filename, std::string psm_filename)
{
  play_level(plf_filename, psm_filename);
  do_replay = false;
  FadeOut::random();
}

void 
Client::init_display()
{
  Timer timer;
  
  timer.start();
  if (verbose) std::cout << "Client: Generating UI elements..." << std::flush;

  Display::set_cursor(CL_MouseCursorProvider::load("Cursors/cursor", PingusResource::get("game.dat")));
  Display::show_cursor();
  
  if (!playfield) playfield    = new Playfield(plf, server->get_world());
  else throw PingusError("Memory hole");
  
  button_panel = new ButtonPanel(plf);
  pcounter     = new PingusCounter();
  small_map    = new SmallMap();
  time_display = new TimeDisplay();
  gui_is_init = true;
   
  button_panel->set_server(server);
  time_display->set_server(server);
  button_panel->set_client(this);
  pcounter->set_client(this);
  small_map->set_client(this);
  
  event->playfield = playfield;

  /*  if (play_demo)
    server->set_demo(demo_file);
  */
  playfield->set_clip_rect(0, 0, 
			   CL_Display::get_width(),
			   CL_Display::get_height());

  // Connect the button_panel with the playfield
  playfield->set_buttons(button_panel);
  playfield->set_server(server);
  playfield->set_client(this);

  // Adding all GuiObj's to the screen
  obj.push_back(playfield);
  obj.push_back(pcounter);
  obj.push_back(time_display);
  obj.push_back(button_panel);
  obj.push_back(small_map);

  if (verbose) std::cout << "done " << timer.stop() << std::endl;
}

void 
Client::deinit_display()
{
  Display::hide_cursor();

  // Delete all alocated objects
  delete pcounter;
  delete time_display;
  delete button_panel;
  delete playfield;
  delete small_map;
  delete plf;
}

void
Client::resize_display()
{
}

void
Client::play_level(std::string plf_filename, std::string psm_filename)
{
  Timer timer;

  timer.start();
  std::cout << "Client::play_level(), Reading PLF..." << std::flush;
  plf          = new PLF(plf_filename);

  std::cout << "done " << timer.stop() << std::endl;

  // FIXME: dirty hack, should replace or merge the psm files
  {
    std::string filename = plf_filename.substr(0, plf_filename.size() - 4);
    
    if (verbose > 1) std::cout << "PSM: " << filename + ".psm" << std::endl;
    
    plf->set_psm_filename(filename + ".psm");
  }

  server->start(plf);

  if (!player)
    server->record_demo();

  event->register_event_handler();

  init_display(); 

  if (verbose)
    std::cout << "Client: Entering main_loop. Startup time: " 
	      << CL_System::get_time() << " msec." << std::endl;
  
  // Clear both buffers
  CL_Display::clear_display();
  Display::flip_display();
  CL_Display::clear_display();

#ifdef HAVE_LIBSDL_MIXER
  PingusSound::play_mod(find_file(pingus_datadir, "music/" + plf->get_music().res_name));
#endif

  // Main Game Loop
  while (!server->is_finished()) 
    {
      CL_System::keep_alive(); 
    
      // Let the server process a game loop
      server->let_move();

      send_next_event();
    
      // Let the window move its content
      for(std::vector<GuiObj*>::size_type i=0; i < obj.size(); ++i) 
	obj[i]->let_move();
    
      if (!fast_forward || skip_frame >= 10) 
	{
	  skip_frame = 0;

	  for(std::vector<GuiObj*>::size_type i=0; i < obj.size(); ++i) 
	    obj[i]->draw_clipped();
      
	  Display::flip_display();
      
	  count_fps();
	}
      else 
	{
	  ++skip_frame;
	}
    }
  event->unregister_event_handler();
}

void
Client::send_next_event()
{
  if (!player) 
    {
      return;
    }
 else
   {
     while(!player->empty())
       {
	 const PingusEvent& event = player->peek_event();
	 
	 if (event.game_time > GameTime::get_time()) {
	   break;
	 } else if (event.game_time == GameTime::get_time()) {
	   server->send_event(event.str);
	   player->dequeue_event();
	 } else {
	   std::cout << "Client: Demo code currupt..." << std::endl;
	 }
       }
   }
}

void
Client::count_fps()
{
  static unsigned int last_time;
  static int fps_count;
  
  ++fps_count;
  // Print the fps rate every 2secs
  if (print_fps && (last_time + 1000 < CL_System::get_time()))
    {
      //cout << "Client: " << fps_count << "fps" << std::endl;
      current_fps = fps_count;
      fps_count = 0;
      last_time = CL_System::get_time();
    }
} 

int
Client::get_fps()
{
  return current_fps;
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

void
Client::set_pause(bool value)
{
  pause = value;
  server->set_pause(value);
}

bool
Client::get_pause()
{
  return pause;
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

