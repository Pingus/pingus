//  $Id: Config.cc,v 1.10 2000/06/27 06:32:54 grumbel Exp $
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

#include <cstdio>
#include <iostream>

#include "globals.hh"
#include "audio.hh"
#include "PingusError.hh"
#include "Config.hh"

struct ConfigParserEOF {};

// Create a PLF object and start parsing the given file
ConfigParser::ConfigParser()
{
}

// Destroy all data
ConfigParser::~ConfigParser()
{
}

void
ConfigParser::init(std::string filename)
{
  // Init local vars
  last_atom = ' ';
  lineno = 1;

  try {
    // Start parsing
    open(filename);
    parse();
  }

  catch (PingusError err) {
    std::cout << err.message << std::endl;
  }
}

// Open the file and do some error checking.
void
ConfigParser::open(std::string filename)
{
  in.open(filename.c_str());
  eof = false;

  if (!in) 
    throw PingusError("Couldn't open: " + filename);
    
  if (verbose > 1)
    std::cout << "Opened plf file successfully" << std::endl;
}

// Return the next char from file and check for eof.
char
ConfigParser::get_char(void)
{
  int c;

  if (eof) {
    if (verbose > 1) std::cout << "ConfigParser: Result of get_char() will be undefined" << std::endl;
    throw PingusError("");
  }

  c = in.get();

  if (c == EOF) {
    if (verbose > 1) std::cout << "PLF::get_char(): Found EOF!" << std::endl;
    eof = true;
    throw ConfigParserEOF();
  }

  if (c == '\n')
    ++lineno;

  return c;
}

// Return the next char from file, remove all comments and spaces before.
char
ConfigParser::get_raw_atom(void)
{
  char c;
  char temp_c;
  c = get_char();

  if (c == '#')
    {
      while(get_char() != '\n'); // Ignoring until EOL

      if (eof)
	return ' ';
      
      return get_atom();
    }

  if (isspace(c)) 
    {
      temp_c = c;
      while (isspace(c = get_char()));
      in.putback(c);
      if (isspace(last_atom)) 
	return get_atom();
      return temp_c;
    }

  return c;
}

// Return the next atom and keep it.
char
ConfigParser::get_atom(void)
{
  last_atom = get_raw_atom();
  
  return last_atom;
}

// Return the name of the value identiver.
std::string
ConfigParser::get_valueid(void)
{
  std::string ret_val;
  char   atom;
  
  jump();

  while(true) 
    {
      atom = get_atom();
      
      if (isgraph(atom) && atom != '=')
	{
	  ret_val += atom;
	} 
      else if (isspace(atom)) 
	{
	  return ret_val;
	}
      else if (atom == '=') 
	{
	  in.putback(atom);
	  return ret_val;
	}
      else 
	{
	  syntax_error(std::string("Unexpected char: '") + atom + "'");
	}
    } 
  
  return ret_val;
}

std::string
ConfigParser::get_value(void)
{
  std::string ret_val;
  char   atom;

  jump();

  while(true) {
    atom = get_atom();

    if (atom == '"' && ret_val == "") {
      while((atom = get_char()) != '"') {
	ret_val += atom;
      }
      return ret_val;
    }

    if (atom == ';') {
      in.putback(atom);
      return ret_val;
    }
   
    if (!isalnum(atom) && atom != '-' && atom != '_') {
      if (isspace(atom)){
	return ret_val;
      } else {
	syntax_error(std::string("Unexpected char '") + atom + "'");
      }
    }
    
    ret_val += atom;
  }
  
  return ret_val;
}

// Jumps to the position of the next token after 'a', no other char's
// then spaces are allowed before 'a'.
void
ConfigParser::jump_after(char c)
{
  char atom;
  
  jump();

  atom = get_atom();
  if (isspace(atom) || atom == c) 
    {
      if (atom == c) {
	return;
      } else {
	atom = get_atom();
	if (atom == c)
	  return;
      }
    } 
  syntax_error(std::string("jump_after(): Expected '") + c + "', got '" + atom + "'" );
}

void
ConfigParser::jump(void)
{
  char atom;
  atom = get_atom();

  if (isspace(atom)) {
    return;
  } else {
    in.putback(atom);
  }
}

// Print a error message with lineno and filename.
void
ConfigParser::syntax_error(std::string error = "")
{
  std::string error_str;
  char tmp[256];

  sprintf(tmp, "%d\n", lineno);
  
  error_str = std::string("PLF: Syntax Error at line ") + tmp;

  if (error != "")
    error_str += "PLF:" + error + "\n";

  throw PingusError(error_str);
}

// Parse the file and fill all structs with the values.
void
ConfigParser::parse(void)
{
  std::string valueid;
  std::string value;

  try 
    {
      while(!eof) 
	{
	  valueid = get_valueid();   
	  jump_after('=');
	  value   = get_value();
	  jump_after(';'); 
	  set_value(valueid, value);
	}
    }
  catch (ConfigParserEOF a) {}
}

Config::Config()
{
  
}

Config::Config(std::string f)
{
  filename = f;
  init(filename);
}

void
Config::set_value(std::string valueid,
		  std::string value)
{
  if (valueid == "fullscreen")
    {
      fullscreen_enabled = str_to_bool(value);
    }
  else if (valueid == "tile_size")
    {
      tile_size = str_to_int(value);
    }
  else if (valueid == "game_speed")
    {
      game_speed = str_to_int(value);
    }
  else if (valueid == "print_fps")
    {
      print_fps = str_to_bool(value);
    }
  else if (valueid == "show-intro")
    {
      intro_disabled = !str_to_bool(value);
    }
  else if (valueid == "fast_mode")
    {
      fast_mode = str_to_bool(value);
    }
  else if (valueid == "maintainer_mode")
    {
      maintainer_mode = str_to_bool(value);
    }
  else if (valueid == "unlimited_actions")
    {
      unlimited_actions = str_to_bool(value);
    }
  else if (valueid == "quick_play")
    {
      quick_play = str_to_bool(value);
    }
  else if (valueid == "cursor_enabled")
    {
      cursor_enabled = str_to_bool(value);
    }
  else if (valueid == "auto_scrolling")
    {
      auto_scrolling = str_to_bool(value);
    }
  else if (valueid == "horizontal_button_panel")
    {
      horizontal_button_panel = str_to_bool(value);
    }
  else if (valueid == "verbose")
    {
      verbose = str_to_int(value);
    }
  else if (valueid == "width")
    {
      screen_width = str_to_int(value);
    }
  else if (valueid == "height")
    {
      screen_height = str_to_int(value);
    }
  else if (valueid == "preload_data")
    {
      preload_data = str_to_bool(value);
    }
  else if (valueid == "audio-format")
    {
      if (value == "8")
	pingus_audio_format = AUDIO_S8;
      else
	pingus_audio_format = AUDIO_S16;
    }
  else if (valueid == "audio-rate")
    {
      pingus_audio_rate = str_to_int(value);
    }
  else if (valueid == "audio-channels")
    {
      pingus_audio_buffers = str_to_int(value);      
    }
  else if (valueid == "audio-buffers")
    {
      pingus_audio_buffers = str_to_int(value);
    }
  else if (valueid == "swcursor-enabled")
    {
      swcursor_enabled = str_to_bool(value);
    }
  else if (valueid == "username")
    {
      global_username = valueid;
    }
  else if (valueid == "email")
    {
      global_email = valueid;
    }
  else
    {
      throw PingusError("Config: Unknown valueid: " + valueid);
    }
}

bool
Config::str_to_bool(const std::string& str)
{
  if (str == "true" || str == "1")
    {
      return true;
    }
  else if (str == "false" || str == "0")
    {
      return false;
    }
  else
    {
      throw PingusError("Config: value: " + str + " is not of type bool.");
    }
}

int
Config::str_to_int(const std::string& str)
{
  int ret_val;

  if (sscanf(str.c_str(), "%d", &ret_val) != 1) 
    {
      throw PingusError("Config: Couldn't covert std::string to integer: " + str);
    }

  return ret_val;
}

/* EOF */
