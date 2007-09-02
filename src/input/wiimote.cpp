/*  $Id$
**   __      __ __             ___        __   __ __   __
**  /  \    /  \__| ____    __| _/_______/  |_|__|  | |  |   ____
**  \   \/\/   /  |/    \  / __ |/  ___/\   __\  |  | |  | _/ __ \
**   \        /|  |   |  \/ /_/ |\___ \  |  | |  |  |_|  |_\  ___/
**    \__/\  / |__|___|  /\____ /____  > |__| |__|____/____/\___  >
**         \/          \/      \/    \/                         \/
**  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmx.de>
**
**  This program is free software; you can redistribute it and/or
**  modify it under the terms of the GNU General Public License
**  as published by the Free Software Foundation; either version 2
**  of the License, or (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
** 
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
**  02111-1307, USA.
*/

#include <iostream>
#include <assert.h>
#include <pthread.h>
#include "math.hpp"
#include "wiimote.hpp"

Wiimote* wiimote = 0;

#ifdef HAVE_CWIID

void
Wiimote::init()
{
  if (!wiimote)
    wiimote = new Wiimote();
}

void
Wiimote::deinit()
{
  delete wiimote;
  wiimote = 0;
}

Wiimote::Wiimote()
  : m_wiimote(0),
    m_rumble(false),
    m_led_state(0),
    m_nunchuk_btns(0),
    m_nunchuk_stick_x(0),
    m_nunchuk_stick_y(0),
    m_buttons(0)
{
  pthread_mutex_init(&mutex, NULL);

  assert(wiimote == 0);
  wiimote = this;
  
  cwiid_set_err(&Wiimote::err_callback);
}

Wiimote::~Wiimote()
{
  disconnect();
  pthread_mutex_destroy(&mutex);
}

void
Wiimote::connect()
{
  assert(m_wiimote == 0);

  /* Connect to any wiimote */
  bdaddr_t bdaddr = *BDADDR_ANY;

  /* Connect to address in string WIIMOTE_BDADDR */
  /* str2ba(WIIMOTE_BDADDR, &bdaddr); */

  /* Connect to the wiimote */
  printf("Put Wiimote in discoverable mode now (press 1+2)...\n");

  if (!(m_wiimote = cwiid_connect(&bdaddr, CWIID_FLAG_MESG_IFC))) 
    {
      fprintf(stderr, "Unable to connect to wiimote\n");
    }
  else 
    {
      std::cout << "Wiimote connected: " << m_wiimote << std::endl;
      if (cwiid_set_mesg_callback(m_wiimote, &Wiimote::mesg_callback)) {
        std::cerr << "Unable to set message callback" << std::endl;
      }

      if (cwiid_command(m_wiimote, CWIID_CMD_RPT_MODE, 
                        CWIID_RPT_STATUS  |
                        CWIID_RPT_NUNCHUK |
                        CWIID_RPT_ACC     |
                        CWIID_RPT_BTN))
        {
          std::cerr << "Wiimote: Error setting report mode" << std::endl;
        }

      { // read calibration data
	uint8_t buf[7];

        if (cwiid_read(m_wiimote, CWIID_RW_EEPROM, 0x16, 7, buf))
          {
            std::cout << "Wiimote: Unable to retrieve accelerometer calibration" << std::endl;
          }
        else
          {
            wiimote_zero.x = buf[0];
            wiimote_zero.y = buf[1];
            wiimote_zero.z = buf[2];

            wiimote_one.x  = buf[4];
            wiimote_one.y  = buf[5];
            wiimote_one.z  = buf[6];
          }

        if (cwiid_read(m_wiimote, CWIID_RW_REG | CWIID_RW_DECODE, 0xA40020, 7, buf))
          {
            std::cout << "Wiimote: Unable to retrieve wiimote accelerometer calibration" << std::endl;
          }
        else
          {
            nunchuk_zero.x = buf[0];
            nunchuk_zero.y = buf[1];
            nunchuk_zero.z = buf[2];
            
            nunchuk_one.x  = buf[4];
            nunchuk_one.y  = buf[5];
            nunchuk_one.z  = buf[6];
          }

        std::cout << "Wiimote Calibration: "
                  << (int)wiimote_zero.x << ", "
                  << (int)wiimote_zero.x << ", "
                  << (int)wiimote_zero.x << " - "
                  << (int)wiimote_one.x << ", "
                  << (int)wiimote_one.x << ", "
                  << (int)wiimote_one.x << std::endl;

        std::cout << "Nunchuk Calibration: "
                  << (int)nunchuk_zero.x << ", "
                  << (int)nunchuk_zero.x << ", "
                  << (int)nunchuk_zero.x << " - "
                  << (int)nunchuk_one.x << ", "
                  << (int)nunchuk_one.x << ", "
                  << (int)nunchuk_one.x << std::endl;
          
      }
    }
}

void
Wiimote::disconnect()
{
  if (m_wiimote)
    {
      cwiid_disconnect(m_wiimote);
      m_wiimote = 0;
    }
}

void
Wiimote::set_led(unsigned char led_state)
{
  if (m_led_state != led_state)
    {
      //std::cout << "Wiimote: " << (int)m_led_state << std::endl;
      m_led_state = led_state;

      if (cwiid_command(m_wiimote, CWIID_CMD_LED, m_led_state)) {
        fprintf(stderr, "Error setting LEDs \n");
      }
    }
}

void
Wiimote::set_led(int num, bool state)
{
  assert(num >= 1 && num <= 4);

  int new_led_state = m_led_state;
  if (state)
    new_led_state |= (1 << (num-1));
  else // (!state)
    new_led_state &= ~(1 << (num-1));

  set_led(new_led_state);
}

void
Wiimote::set_rumble(bool r)
{
  if (r != m_rumble)
    {
      m_rumble = r;

      if (cwiid_command(m_wiimote, CWIID_CMD_RUMBLE, m_rumble)) {
        std::cerr << "Error setting rumble" << std::endl;
      }
    }
}

void
Wiimote::add_button_event(int device, int button, bool down)
{
  // std::cout << "Wiimote::add_button_event: " << device << " " << button << " " << down << std::endl;
  WiimoteEvent event;

  event.type = WiimoteEvent::WIIMOTE_BUTTON_EVENT;
  event.button.device = 0;
  event.button.button = button;
  event.button.down   = down;

  events.push_back(event);
}

void
Wiimote::add_axis_event(int device, int axis, float pos)
{
  //std::cout << "Wiimote::add_axis_event: " << device << " " << axis << " " << pos << std::endl;

  WiimoteEvent event;

  event.type = WiimoteEvent::WIIMOTE_AXIS_EVENT;
  event.axis.device = 0;
  event.axis.axis = axis;
  event.axis.pos  = pos;

  events.push_back(event); 
}

void
Wiimote::add_acc_event(int device, int accelerometer, float x, float y, float z)
{
  WiimoteEvent event;

  event.type = WiimoteEvent::WIIMOTE_ACC_EVENT;
  event.acc.device = 0;
  event.acc.accelerometer = accelerometer;
  event.acc.x = x;
  event.acc.y = y;
  event.acc.z = z;

  events.push_back(event);  
}


void
Wiimote::on_status(const cwiid_status_mesg& msg)
{
  printf("Status Report: battery=%d extension=", msg.battery);
  switch (msg.ext_type)
    {
    case CWIID_EXT_NONE:
      printf("none");
      break;

    case CWIID_EXT_NUNCHUK:
      printf("Nunchuk");
      break;

    case CWIID_EXT_CLASSIC:
      printf("Classic Controller");
      break;

    default:
      printf("Unknown Extension");
      break;
    }
  printf("\n");
}

void
Wiimote::on_error(const cwiid_error_mesg& msg)
{
  std::cout << "On Error" << std::endl;

  if (m_wiimote)
    {
      if (cwiid_disconnect(m_wiimote)) 
        {
          fprintf(stderr, "Error on wiimote disconnect\n");
          m_wiimote = 0;
        }
    }
}

void
Wiimote::on_button(const cwiid_btn_mesg& msg)
{
#define CHECK_BTN(btn, num) if (changes & btn) add_button_event(0, num, m_buttons & btn)

  uint16_t changes = m_buttons ^ msg.buttons;
  m_buttons = msg.buttons;
 
  CHECK_BTN(CWIID_BTN_A, 0);
  CHECK_BTN(CWIID_BTN_B, 1);

  CHECK_BTN(CWIID_BTN_LEFT,  2);
  CHECK_BTN(CWIID_BTN_RIGHT, 3);
  CHECK_BTN(CWIID_BTN_UP,    4);
  CHECK_BTN(CWIID_BTN_DOWN,  5);

  CHECK_BTN(CWIID_BTN_PLUS,  6);
  CHECK_BTN(CWIID_BTN_HOME,  7);
  CHECK_BTN(CWIID_BTN_MINUS, 8);

  CHECK_BTN(CWIID_BTN_1,  9);
  CHECK_BTN(CWIID_BTN_2, 10);
}

void
Wiimote::on_acc(const cwiid_acc_mesg& msg)
{
  //printf("Acc Report: x=%d, y=%d, z=%d\n", msg.acc[0], msg.acc[1], msg.acc[2]);

  add_acc_event(0, 0, 
                (msg.acc[0] - wiimote_zero.x) / float(wiimote_one.x - wiimote_zero.x),
                (msg.acc[1] - wiimote_zero.y) / float(wiimote_one.y - wiimote_zero.y),
                (msg.acc[2] - wiimote_zero.z) / float(wiimote_one.z - wiimote_zero.z));
}

void
Wiimote::on_ir(const cwiid_ir_mesg& msg)
{
  printf("IR Report: ");
  for (int i = 0; i < CWIID_IR_SRC_COUNT; ++i)
    {
      if (msg.src[i].valid) {
        printf("(%d,%d) ", msg.src[i].pos[0], msg.src[i].pos[1]);
      }
    }
}

/** Convert value to float while taking calibration data, left/center/right into account */
inline float to_float(uint8_t min, 
                      uint8_t center, 
                      uint8_t max, 
                      uint8_t value)
{
  if (value < center)
    {
      return Math::clamp(-1.0f, -(center - value) / float(center - min), 1.0f);
    }
  else if (value > center)
    {
      return Math::clamp(-1.0f, (value - center) / float(max - center), 1.0f);
    }
  else 
    {
      return 0.0f;
    }
}

void
Wiimote::on_nunchuck(const cwiid_nunchuk_mesg& msg)
{
  uint8_t changes = m_nunchuk_btns ^ msg.buttons;
  m_nunchuk_btns  = msg.buttons;

#define CHECK_NCK_BTN(btn, num) if (changes & btn) add_button_event(0, num, m_nunchuk_btns & btn)
      
  CHECK_NCK_BTN(CWIID_NUNCHUK_BTN_Z, 11);
  CHECK_NCK_BTN(CWIID_NUNCHUK_BTN_C, 12);
  
  // FIXME: Read real calibration data, instead of hardcoded one
  float nunchuk_stick_x =  to_float(37, 129, 231, msg.stick[0]);
  float nunchuk_stick_y = -to_float(22, 119, 213, msg.stick[1]);

  if (m_nunchuk_stick_x != nunchuk_stick_x)
    {
      m_nunchuk_stick_x = nunchuk_stick_x;
      add_axis_event(0, 0, m_nunchuk_stick_x);
    } 

  if (m_nunchuk_stick_y != nunchuk_stick_y)
    {
      m_nunchuk_stick_y = nunchuk_stick_y;
      add_axis_event(0, 1, m_nunchuk_stick_y);
    }

  add_acc_event(0, 1, 
                (msg.acc[0] - nunchuk_zero.x) / float(nunchuk_one.x - nunchuk_zero.x),
                (msg.acc[1] - nunchuk_zero.y) / float(nunchuk_one.y - nunchuk_zero.y),
                (msg.acc[2] - nunchuk_zero.z) / float(nunchuk_one.z - nunchuk_zero.z));
  if (0)
    printf("Nunchuk Report: btns=%.2X stick=(%3d,%3d) (%5.2f, %5.2f) acc.x=%d acc.y=%d acc.z=%d\n", 
           msg.buttons,
           msg.stick[0], msg.stick[1], 
           m_nunchuk_stick_x,
           m_nunchuk_stick_y,
           msg.acc[0], msg.acc[1], msg.acc[2]);
}

void
Wiimote::on_classic(const cwiid_classic_mesg& msg)
{
  printf("Classic Report: btns=%.4X l_stick=(%d,%d) r_stick=(%d,%d) "
         "l=%d r=%d\n", msg.buttons,
         msg.l_stick[0], msg.l_stick[1],
         msg.r_stick[0], msg.r_stick[1],
         msg.l, msg.r);
}

std::vector<WiimoteEvent>
Wiimote::pop_events()
{
  pthread_mutex_lock(&mutex);
  std::vector<WiimoteEvent> ret = events;
  events.clear();
  pthread_mutex_unlock(&mutex);
  return ret;
}

// Callback function that get called by the Wiimote thread
void
Wiimote::err(cwiid_wiimote_t* w, const char *s, va_list ap)
{
  pthread_mutex_lock(&mutex);

  if (w) 
    printf("%d:", cwiid_get_id(w));
  else 
    printf("-1:");

  vprintf(s, ap);
  printf("\n");  

  pthread_mutex_unlock(&mutex);
}

void
Wiimote::mesg(cwiid_wiimote_t* w, int mesg_count, union cwiid_mesg mesg[])
{
  pthread_mutex_lock(&mutex);

  //std::cout << "StatusCallback: " << w << " " << mesg_count << std::endl;
  for (int i=0; i < mesg_count; i++)
    {
      switch (mesg[i].type) 
        {
        case CWIID_MESG_STATUS:
          wiimote->on_status(mesg[i].status_mesg);
          break;

        case CWIID_MESG_BTN:
          wiimote->on_button(mesg[i].btn_mesg);
          break;

        case CWIID_MESG_ACC:
          wiimote->on_acc(mesg[i].acc_mesg);
          break;

        case CWIID_MESG_IR:
          wiimote->on_ir(mesg[i].ir_mesg);
          break;

        case CWIID_MESG_NUNCHUK:
          wiimote->on_nunchuck(mesg[i].nunchuk_mesg);
          break;

        case CWIID_MESG_CLASSIC:
          wiimote->on_classic(mesg[i].classic_mesg);
          break;

        case CWIID_MESG_ERROR:
          wiimote->on_error(mesg[i].error_mesg);
          break;

        default:
          printf("Wiimote: Unknown Report");
          break;
        }
    }

  pthread_mutex_unlock(&mutex);
}

// static callback functions
  
void
Wiimote::err_callback(cwiid_wiimote_t* w, const char *s, va_list ap)
{
  wiimote->err(w, s, ap);
}

void
Wiimote::mesg_callback(cwiid_wiimote_t* w, int mesg_count, union cwiid_mesg mesg[])
{
  wiimote->mesg(w, mesg_count, mesg);
}

#endif // HAVE_CWIID

/* EOF */
