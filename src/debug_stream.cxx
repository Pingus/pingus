//  $Id: debug_stream.cxx,v 1.5 2002/08/22 02:24:59 grumbel Exp $
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

#include "pingus.hxx"
#include "debug_stream.hxx"
#include "globals.hxx"

using std::ostream;

NilStream DebugStream::nilstream;

DebugStream::Buffer::Buffer (const std::string& p)
  : prefix (p)
{
  // Set the output buffer
  setp (char_buffer, char_buffer + buffersize - 1);

  // Switch of input buffer
  setg(0, 0, 0);
}

DebugStream::Buffer::~Buffer ()
{
  sync ();
}

int
DebugStream::Buffer::overflow (int c)
{
  std::string str;
    
  for (char* ptr = pbase (); ptr != pptr (); ++ptr)
    {
      str += *ptr;
      
      if (*ptr == '\n') 
      	{
      	  put_line (prefix + str);
      	  str = "";
      	}
    }
  str += c;
  put_line (str);
    
  setp (char_buffer, char_buffer + buffersize - 1);
  return 0;
}

void
DebugStream::Buffer::put_line (const std::string& line)
{
  if (!out_streams.empty ())
    {
      for (std::vector<std::ostream*>::iterator i = out_streams.begin ();
	    i != out_streams.end (); ++i)
      	{
      	  *(*i) << line;
      	}
    }
  else
    {
      std::cout << "[DebugStream::Buffer fallback stream] " << line;
    }
}

int
DebugStream::Buffer::sync ()
{
  std::string str;
    
  // Walk through the buffer 
  for (char* c = pbase (); c != pptr (); ++c)
    {
    	str += *c;

      if (*c == '\n') 
      	{
      	  put_line (prefix + str);
      	  str = "";
      	}
    }
    
  if (!str.empty ())
    put_line (str);

  setp (char_buffer, char_buffer + buffersize - 1);
  return 0;
}

void
DebugStream::Buffer::add (ostream& s)
{
  out_streams.push_back (&s);
}


void
DebugStream::Buffer::set_prefix (const std::string & prefix_)
{
  prefix = prefix_;
}


// ----------------------------------------------------------------


DebugStream::DebugStream (const std::string& prefix)
  : ostream (&buffer),
    buffer (prefix)
{
}

DebugStream::~DebugStream ()
{
}

/// returns self if the debug flag is set, else nilstream
ostream & DebugStream::operator () (int component) {

  if (pingus_debug_flags & component) {
    return *this;
  } else {
    return nilstream;
  }  
}

void
DebugStream::add (ostream& s)
{
  buffer.add (s);
}

void
DebugStream::set_prefix (const std::string & prefix)
{
  buffer.set_prefix(prefix);
}


NilStream::NilStream ()
  : ostream(&buffer) 
{ 
}

/* EOF */
