//  $Id: fade_out.hxx,v 1.6 2003/04/19 10:23:17 torangan Exp $
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

#ifndef HEADER_PINGUS_FADE_OUT_HXX
#define HEADER_PINGUS_FADE_OUT_HXX

#include "color.hxx"

///
class FadeOut
{
private:
  /** The time in seconds until the fadeout is complete */
  float complete_time;

  /** The time that passed since the fadeout was started */
  float passed_time;

protected:
  Color color;
public:
  /** @return seconds number of seconds until the fadeout is
      complete */
  FadeOut (float seconds = 1.0f, Color color = Color ());
  virtual ~FadeOut ();

  // Reset the fadeout to the start
  virtual void reset ();

  virtual void  draw () =0;
  virtual void  update (float delta);
  virtual void  set_progress (float progress);
  virtual float get_progress ();
  virtual bool  finished ();

  ///
  static void random(void);
  ///
  static void black_rect(int steps = 20);
  ///
  static void fade_to_black(int steps = 20);
  ///
  static void clear(void);

private:
  FadeOut (const FadeOut&);
  FadeOut& operator= (const FadeOut&);
};

class EnlargingRectFadeOut : public FadeOut
{
private:
public:
  EnlargingRectFadeOut (float seconds = 1.0f, Color color_ = Color())
  : FadeOut (seconds, color_) {}
  ~EnlargingRectFadeOut () {}

  void draw ();

private:
  EnlargingRectFadeOut (const EnlargingRectFadeOut&);
  EnlargingRectFadeOut& operator= (const EnlargingRectFadeOut&);
};

#endif

/* EOF */
