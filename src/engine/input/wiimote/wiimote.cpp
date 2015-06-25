//  Pingus - A free Lemmings clone
//  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmail.com>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "engine/input/wiimote/wiimote.hpp"

#include <iostream>

#include "math/math.hpp"

Wiimote* wiimote = 0;

std::string
Wiimote::id2str(int id)
{
  if      (id == WIIMOTE_A)     return "a";
  else if (id == WIIMOTE_B)     return "b";
  else if (id == WIIMOTE_LEFT)  return "left";
  else if (id == WIIMOTE_RIGHT) return "right";
  else if (id == WIIMOTE_UP)    return "up";
  else if (id == WIIMOTE_DOWN)  return "down";
  else if (id == WIIMOTE_PLUS)  return "plus";
  else if (id == WIIMOTE_MINUS) return "minus";
  else if (id == WIIMOTE_HOME)  return "home";
  else if (id == WIIMOTE_1)     return "1";
  else if (id == WIIMOTE_2)     return "2";

  else if (id == NUNCHUK_C) return "nunchuk:c";
  else if (id == NUNCHUK_Z) return "nunchuk:z";

  else if (id == CLASSIC_LEFT)  return "classic:left";
  else if (id == CLASSIC_RIGHT) return "classic:right";
  else if (id == CLASSIC_UP)    return "classic:up";
  else if (id == CLASSIC_DOWN)  return "classic:down";
  else if (id == CLASSIC_PLUS)  return "classic:plus";
  else if (id == CLASSIC_MINUS) return "classic:minus";
  else if (id == CLASSIC_HOME)  return "classic:home";
  else if (id == CLASSIC_A)     return "classic:a";
  else if (id == CLASSIC_B)     return "classic:b";
  else if (id == CLASSIC_X)     return "classic:x";
  else if (id == CLASSIC_Y)     return "classic:y";
  else if (id == CLASSIC_L)     return "classic:l";
  else if (id == CLASSIC_R)     return "classic:r";
  else if (id == CLASSIC_ZL)    return "classic:zl";
  else if (id == CLASSIC_ZR)    return "classic:zr";
  else
    return "unknown";
}

int
Wiimote::str2id(const std::string& str)
{
  if      (str == "a")     return WIIMOTE_A;
  else if (str == "b")     return WIIMOTE_B;
  else if (str == "left")  return WIIMOTE_LEFT;
  else if (str == "right") return WIIMOTE_RIGHT;
  else if (str == "up")    return WIIMOTE_UP;
  else if (str == "down")  return WIIMOTE_DOWN;
  else if (str == "plus"  || str == "+") return WIIMOTE_PLUS;
  else if (str == "minus" || str == "-") return WIIMOTE_MINUS;
  else if (str == "home")  return WIIMOTE_HOME;
  else if (str == "1")     return WIIMOTE_1;
  else if (str == "2")     return WIIMOTE_2;

  else if (str == "nunchuk:c") return NUNCHUK_C;
  else if (str == "nunchuk:z") return NUNCHUK_Z;

  else if (str == "classic:left")  return CLASSIC_LEFT;
  else if (str == "classic:right") return CLASSIC_RIGHT;
  else if (str == "classic:up")    return CLASSIC_UP;
  else if (str == "classic:down")  return CLASSIC_DOWN;
  else if (str == "classic:plus"  || str == "classic:+") return CLASSIC_PLUS;
  else if (str == "classic:minus" || str == "classic:-") return CLASSIC_MINUS;
  else if (str == "classic:home")  return CLASSIC_HOME;
  else if (str == "classic:a")     return CLASSIC_A;
  else if (str == "classic:b")     return CLASSIC_B;
  else if (str == "classic:x")     return CLASSIC_X;
  else if (str == "classic:y")     return CLASSIC_Y;
  else if (str == "classic:l")     return CLASSIC_L;
  else if (str == "classic:r")     return CLASSIC_R;
  else if (str == "classic:zl")    return CLASSIC_ZL;
  else if (str == "classic:zr")    return CLASSIC_ZR;
  else
    return UNKNOWN;
}

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

Wiimote::Wiimote() :
  mutex(),
  m_wiimote(0),
  m_rumble(false),
  m_led_state(0),
  m_nunchuk_btns(0),
  m_nunchuk_stick_x(0),
  m_nunchuk_stick_y(0),
  m_buttons(0),
  wiimote_zero(),
  wiimote_one(),
  nunchuk_zero(),
  nunchuk_one(),
  events()
{
  pthread_mutex_init(&mutex, NULL);

  assert(wiimote == 0);
  wiimote = this;

  cwiid_set_err(&Wiimote::err_callback_c);
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
  bdaddr_t bdaddr = {{0, 0, 0, 0, 0, 0}};

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
    if (cwiid_set_mesg_callback(m_wiimote, &Wiimote::mesg_callback_c))
    {
      std::cerr << "Unable to set message callback" << std::endl;
    }

    // FIXME: Could init this depending on what events are actually bound
    if (cwiid_command(m_wiimote, CWIID_CMD_RPT_MODE,
                      CWIID_RPT_STATUS  |
                      CWIID_RPT_NUNCHUK |
                      CWIID_RPT_ACC     |
                      CWIID_RPT_IR      |
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

  set_led(static_cast<unsigned char>(new_led_state));
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

  CHECK_BTN(CWIID_BTN_A, WIIMOTE_A);
  CHECK_BTN(CWIID_BTN_B, WIIMOTE_B);

  CHECK_BTN(CWIID_BTN_LEFT,  WIIMOTE_LEFT);
  CHECK_BTN(CWIID_BTN_RIGHT, WIIMOTE_RIGHT);
  CHECK_BTN(CWIID_BTN_UP,    WIIMOTE_UP);
  CHECK_BTN(CWIID_BTN_DOWN,  WIIMOTE_DOWN);

  CHECK_BTN(CWIID_BTN_PLUS,  WIIMOTE_PLUS);
  CHECK_BTN(CWIID_BTN_HOME,  WIIMOTE_HOME);
  CHECK_BTN(CWIID_BTN_MINUS, WIIMOTE_MINUS);

  CHECK_BTN(CWIID_BTN_1,  WIIMOTE_1);
  CHECK_BTN(CWIID_BTN_2,  WIIMOTE_2);
}

void
Wiimote::on_acc(const cwiid_acc_mesg& msg)
{
  //printf("Acc Report: x=%d, y=%d, z=%d\n", msg.acc[0], msg.acc[1], msg.acc[2]);

  add_acc_event(0, 0,
                static_cast<float>(msg.acc[0] - wiimote_zero.x) / static_cast<float>(wiimote_one.x - wiimote_zero.x),
                static_cast<float>(msg.acc[1] - wiimote_zero.y) / static_cast<float>(wiimote_one.y - wiimote_zero.y),
                static_cast<float>(msg.acc[2] - wiimote_zero.z) / static_cast<float>(wiimote_one.z - wiimote_zero.z));
}

void
Wiimote::on_ir(const cwiid_ir_mesg& msg)
{
  bool is_valid = false;
  for (int i = 0; i < CWIID_IR_SRC_COUNT; ++i)
    is_valid = is_valid || msg.src[i].valid;

  if (is_valid)
  {
    std::cout << "IR Report: ";
    for (int i = 0; i < CWIID_IR_SRC_COUNT; ++i)
    {
      if (msg.src[i].valid) {
        std::cout << "(" <<  msg.src[i].pos[0] << ", " <<  msg.src[i].pos[1] << ") ";
      }
    }
    std::cout << std::endl;
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
    return Math::clamp(-1.0f, -static_cast<float>(center - value) / static_cast<float>(center - min), 1.0f);
  }
  else if (value > center)
  {
    return Math::clamp(-1.0f, static_cast<float>(value - center) / static_cast<float>(max - center), 1.0f);
  }
  else
  {
    return 0.0f;
  }
}

void
Wiimote::on_nunchuk(const cwiid_nunchuk_mesg& msg)
{
  uint8_t changes = m_nunchuk_btns ^ msg.buttons;
  m_nunchuk_btns  = msg.buttons;

#define CHECK_NCK_BTN(btn, num) if (changes & btn) add_button_event(0, num, m_nunchuk_btns & btn)

  CHECK_NCK_BTN(CWIID_NUNCHUK_BTN_Z, NUNCHUK_Z);
  CHECK_NCK_BTN(CWIID_NUNCHUK_BTN_C, NUNCHUK_C);

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
                static_cast<float>(msg.acc[0] - nunchuk_zero.x) / static_cast<float>(nunchuk_one.x - nunchuk_zero.x),
                static_cast<float>(msg.acc[1] - nunchuk_zero.y) / static_cast<float>(nunchuk_one.y - nunchuk_zero.y),
                static_cast<float>(msg.acc[2] - nunchuk_zero.z) / static_cast<float>(nunchuk_one.z - nunchuk_zero.z));
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
Wiimote::err_callback(cwiid_wiimote_t* w, const char *s, va_list ap)
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
Wiimote::mesg_callback(cwiid_wiimote_t* w, int mesg_count, union cwiid_mesg mesg[])
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
        wiimote->on_nunchuk(mesg[i].nunchuk_mesg);
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
Wiimote::err_callback_c(cwiid_wiimote_t* w, const char *s, va_list ap)
{
  wiimote->err_callback(w, s, ap);
}

void
Wiimote::mesg_callback_c(cwiid_wiimote_t* w, int mesg_count, union cwiid_mesg mesg[], timespec*)
{
  wiimote->mesg_callback(w, mesg_count, mesg);
}

/* EOF */
