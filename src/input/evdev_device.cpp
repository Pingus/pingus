//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmx.de>
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

#include <assert.h>
#include <linux/input.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>
#include <iostream>
#include <errno.h>

#include "evdev_device.hpp"

#define BITS_PER_LONG (sizeof(long) * 8)
#define NBITS(x) ((((x)-1)/BITS_PER_LONG)+1)
#define OFF(x)  ((x)%BITS_PER_LONG)
#define BIT(x)  (1UL<<OFF(x))
#define LONG(x) ((x)/BITS_PER_LONG)
#define test_bit(bit, array)	((array[LONG(bit)] >> OFF(bit)) & 1)

namespace Input {

EvdevDevice::EvdevDevice(const std::string& filename)
  : device(filename)
{
  fd = open(device.c_str(), O_RDONLY | O_NONBLOCK);

  if (fd == -1)
    {
      throw std::runtime_error(strerror(errno));
    }

  if (ioctl(fd, EVIOCGVERSION, &version)) 
    {
      throw std::runtime_error("Error: EvdevDevice: Couldn't get version");
    }

  if (1)
    { // FIXME: Some versions of linux don't have these structs, use arrays there
      struct input_id id;
      ioctl(fd, EVIOCGID, &id);
      printf("Input device ID: bus 0x%x vendor 0x%x product 0x%x version 0x%x\n",
             id.bustype, id.vendor, id.product, id.vendor);
    }

  {  // Get the human readable name
    char c_name[256] = "unknown";
    ioctl(fd, EVIOCGNAME(sizeof(c_name)), c_name);
    name = c_name;
    std::cout << "Name: " << name << std::endl;
  }

  { // Read in how many buttons the device has
    unsigned long bit[EV_MAX][NBITS(KEY_MAX)];
    memset(bit, 0, sizeof(bit));
    ioctl(fd, EVIOCGBIT(0, EV_MAX), bit[0]);

    for (int i = 0; i < EV_MAX; i++)
      {
        if (test_bit(i, bit[0])) 
          {
            //printf("  Event type %d (%s)\n", i, events[i] ? events[i] : "?");
				
            if (!i) continue;

            ioctl(fd, EVIOCGBIT(i, KEY_MAX), bit[i]);
            for (int j = 0; j < KEY_MAX; j++) 
              {
                if (test_bit(j, bit[i])) 
                  {
                    if (i == EV_KEY) 
                      { 
                        keys.push_back(Key(j));
                      } 
                    else if (i == EV_ABS)
                      { 
                        // FIXME: Some Linuxes don't have these struct
                        struct input_absinfo absinfo;
                        ioctl(fd, EVIOCGABS(j), &absinfo);
                        // FIXME: we are ignoring absinfo.fuzz and
                        // absinfo.flat, not sure what they are good for
                        absolutes.push_back(Absolute(j, absinfo.minimum, absinfo.maximum, absinfo.value));
                      }
                    else if (i == EV_REL) 
                      {
                        relatives.push_back(Relative(j));
                      }
                  }
              }
          }
      }
  }
}

EvdevDevice::~EvdevDevice()
{
}

void
EvdevDevice::process_absolute(struct input_event& ev)
{
  int axis_index = get_absolute_index_by_code(ev.code);
  if (axis_index != -1)
    {
      absolutes[axis_index].pos = ev.value;

#if 0
      CL_InputEvent e; 
					
      e.device   = CL_InputDevice(this);
      e.type     = CL_InputEvent::axis_moved;
      e.id       = axis_index;
      e.axis_pos = absolutes[axis_index].get_posf();
      e.repeat_count = 0;

      sig_axis_move(e);
#endif
    }
}

void
EvdevDevice::process_relative(struct input_event& ev)
{
  int relative_index = get_relative_index_by_code(ev.code);

  relatives[relative_index].pos += ev.value;

#if 0
  CL_InputEvent e; 
				
  e.device       = CL_InputDevice(this);
  e.type         = CL_InputEvent::relative_moved;
  e.id           = relative_index;
  e.mouse_pos.x  = ev.value;
  e.mouse_pos.y  = 0;
  e.axis_pos     = 0;
  e.repeat_count = 0;

  sig_relative_move(e);
#endif
}

void
EvdevDevice::process_key(struct input_event& ev)
{
  int button_index = get_key_index_by_code(ev.code);

  keys[button_index].pressed = ev.value;

#if 0
  CL_InputEvent e; 
			
  e.device = CL_InputDevice(this);
  e.id     = button_index;
  e.repeat_count = 0;

  if (ev.value)
    {
      e.type = CL_InputEvent::pressed;
      sig_key_down(e);
    }
  else
    {
      e.type = CL_InputEvent::released;
      sig_key_up(e);
    }               
#endif
}

int
EvdevDevice::get_relative_index_by_code(int code)
{
  for(std::vector<Relative>::size_type i = 0; i != relatives.size(); ++i)
    if (relatives[i].code == code)
      return i;
  return -1;
}

int
EvdevDevice::get_key_index_by_code(int code)
{
  for(std::vector<Key>::size_type i = 0; i != keys.size(); ++i)
    if (keys[i].code == code)
      return i;
  return -1;
}

int
EvdevDevice::get_absolute_index_by_code(int code)
{
  for(std::vector<Absolute>::size_type i = 0; i != absolutes.size(); ++i)
    if (absolutes[i].code == code)
      return i;
  return -1;
}

void
EvdevDevice::update(float delta)
{
  struct input_event ev[64];
	
  int rd = read(fd, ev, sizeof(struct input_event) * 64);

  if (rd >= (int) sizeof(struct input_event))
    {
      for (int i = 0; i < rd / (int)sizeof(struct input_event); ++i)
        {
          std::cout << ev[i].type << " " << ev[i].code << " " << ev[i].value << std::endl;

          switch (ev[i].type)
            {
              case EV_ABS:
                process_absolute(ev[i]);
                break;

              case EV_REL:
                process_relative(ev[i]);
                break;

              case EV_KEY:
                process_key(ev[i]);
                break;

              default:
#if 0
                if (ev[i].type == EV_SYN) 
                  {
                    printf("Event: time %ld.%06ld, -------------- %s ------------\n",
                           ev[i].time.tv_sec, ev[i].time.tv_usec, ev[i].code ? "Config Sync" : "Report Sync" );
                  }
                else if (ev[i].type == EV_MSC && (ev[i].code == MSC_RAW || ev[i].code == MSC_SCAN)) 
                  {
                    printf("Event: time %ld.%06ld, type %d (%s), code %d (%s), value %02x\n",
                           ev[i].time.tv_sec, ev[i].time.tv_usec, ev[i].type,
                           events[ev[i].type] ? events[ev[i].type] : "?",
                           ev[i].code,
                           names[ev[i].type] ? (names[ev[i].type][ev[i].code] ? names[ev[i].type][ev[i].code] : "?") : "?",
                           ev[i].value);
                  } 
                else 
                  {
                    printf("Event: time %ld.%06ld, type %d (%s), code %d (%s), value %d\n",
                           ev[i].time.tv_sec, ev[i].time.tv_usec, ev[i].type,
                           events[ev[i].type] ? events[ev[i].type] : "?",
                           ev[i].code,
                           names[ev[i].type] ? (names[ev[i].type][ev[i].code] ? names[ev[i].type][ev[i].code] : "?") : "?",
                           ev[i].value);
                  }	
#endif                  
                break;
            }
        }
    }
}

} // namespace Input

#ifdef __TEST__
// g++ -D__TEST__ evdev_device.cpp -o evdev -Wall -Werror
int main(int argc, char** argv)
{
  if (argc != 2)
    {
      std::cout << "Usage: evdev FILENAME" << std::endl;
    }
  else
    {
      std::cout << "EvdevDevice: " << argv[1] << std::endl;

      Input::EvdevDevice dev(argv[1]);
      while(true)
        {
          dev.update(0.0f);
        }
    }
  return 0;
}

#endif

/* EOF */
