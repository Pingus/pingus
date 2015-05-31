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

#ifndef HEADER_PINGUS_ENGINE_INPUT_WIIMOTE_WIIMOTE_HPP
#define HEADER_PINGUS_ENGINE_INPUT_WIIMOTE_WIIMOTE_HPP

#include <string>
#include <vector>

#include "cwiid.h"

struct WiimoteButtonEvent
{
  int  device;
  int  button;
  bool down;
};

struct WiimoteAxisEvent
{
  int   device;
  int   axis;
  float pos;
};

struct WiimoteAccEvent
{
  int   device;
  int   accelerometer;
  float x;
  float y;
  float z;
};

struct WiimoteEvent
{
  enum { WIIMOTE_AXIS_EVENT, WIIMOTE_ACC_EVENT, WIIMOTE_BUTTON_EVENT } type;
  union {
    WiimoteAxisEvent   axis;
    WiimoteButtonEvent button;
    WiimoteAccEvent    acc;
  };
};

struct AccCalibration
{
  uint8_t x;
  uint8_t y;
  uint8_t z;
};

class Wiimote
{
public:
  static void err_callback_c(cwiid_wiimote_t*, const char *s, va_list ap);
  static void mesg_callback_c(cwiid_wiimote_t*, int mesg_count, union cwiid_mesg mesg[], timespec*);

  static void init();
  static void deinit();

private:
  pthread_mutex_t  mutex;
  cwiid_wiimote_t* m_wiimote;
  bool             m_rumble;
  unsigned char    m_led_state;
  uint8_t          m_nunchuk_btns;
  float            m_nunchuk_stick_x;
  float            m_nunchuk_stick_y;
  uint16_t         m_buttons;

  AccCalibration wiimote_zero;
  AccCalibration wiimote_one;

  AccCalibration nunchuk_zero;
  AccCalibration nunchuk_one;

  std::vector<WiimoteEvent> events;

  void add_button_event(int device, int button, bool down);
  void add_axis_event(int device, int axis, float pos);
  void add_acc_event(int device, int accelerometer, float x, float y, float z);

public:
  enum {
    // Wiimote
    WIIMOTE_A,
    WIIMOTE_B,
    WIIMOTE_LEFT,
    WIIMOTE_RIGHT,
    WIIMOTE_UP,
    WIIMOTE_DOWN,
    WIIMOTE_PLUS,
    WIIMOTE_HOME,
    WIIMOTE_MINUS,
    WIIMOTE_1,
    WIIMOTE_2,

    // Nunchuk
    NUNCHUK_C,
    NUNCHUK_Z,

    // Classic
    CLASSIC_LEFT,
    CLASSIC_RIGHT,
    CLASSIC_UP,
    CLASSIC_DOWN,

    CLASSIC_PLUS,
    CLASSIC_HOME,
    CLASSIC_MINUS,
    CLASSIC_A,
    CLASSIC_B,
    CLASSIC_X,
    CLASSIC_Y,
    CLASSIC_L,
    CLASSIC_R,
    CLASSIC_ZL, // FIXME: This is an axis instead of a button?
    CLASSIC_ZR,

    UNKNOWN
  };

  static std::string id2str(int id);
  static int str2id(const std::string& str);

  Wiimote();
  ~Wiimote();

  void connect();
  void disconnect();

  void set_led(int num, bool state);
  void set_led(unsigned char led_state);
  unsigned char get_led() const { return m_led_state; }

  void set_rumble(bool t);
  bool get_rumble() const { return m_rumble; }

  std::vector<WiimoteEvent> pop_events();

  bool is_connected() const { return m_wiimote != 0; }

  // Callback functions
  void on_status  (const cwiid_status_mesg& msg);
  void on_error   (const cwiid_error_mesg& msg);
  void on_button  (const cwiid_btn_mesg& msg);
  void on_acc     (const cwiid_acc_mesg& msg);
  void on_ir      (const cwiid_ir_mesg& msg);
  void on_nunchuk(const cwiid_nunchuk_mesg& msg);
  void on_classic (const cwiid_classic_mesg& msg);

  void mesg_callback(cwiid_wiimote_t*, int mesg_count, union cwiid_mesg mesg[]);
  void err_callback(cwiid_wiimote_t*, const char *s, va_list ap);

private:
  Wiimote(const Wiimote&);
  Wiimote& operator=(const Wiimote&);
};

class Wiimote;
extern Wiimote* wiimote;

#endif

/* EOF */
