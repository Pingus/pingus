//  $Id: PingusStream.cc,v 1.1 2002/06/05 17:51:08 grumbel Exp $
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

#include "PingusStream.hh"

const int MultiplexStreamBuffer::buffersize = 200;

MultiplexStreamBuffer::MultiplexStreamBuffer (const std::string& p)
  : prefix (p)
{
  // Set the output buffer
  setp (char_buffer, char_buffer + buffersize - 1);

  // Switch of input buffer
  setg(0, 0, 0);
}

MultiplexStreamBuffer::~MultiplexStreamBuffer ()
{
  sync ();
}

int
MultiplexStreamBuffer::overflow (int c)
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
MultiplexStreamBuffer::put_line (const std::string& line)
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
      std::cout << "[MultiplexStreamBuffer fallback stream] " << line;
    }
}

int
MultiplexStreamBuffer::sync ()
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
    
  if (!str.empty ()) // Why to we check this?
    {
      put_line (str);
    }

  setp (char_buffer, char_buffer + buffersize - 1);
  return 0;
}

void
MultiplexStreamBuffer::add (std::ostream& s)
{
  out_streams.push_back (&s);
}

MultiplexStream::MultiplexStream (const std::string& prefix)
  : std::ostream (&buffer),
    buffer (prefix)
{
}

MultiplexStream::~MultiplexStream ()
{
}

void
MultiplexStream::add (std::ostream& s)
{
  buffer.add (s);
}

/* EOF */
