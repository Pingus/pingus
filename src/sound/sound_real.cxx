//  $Id: sound_real.cxx,v 1.11 2003/10/21 11:01:52 grumbel Exp $
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

#include "../globals.hxx"
#include "../debug.hxx"
#include "sound_res_mgr.hxx"
#include "sound_real.hxx"

namespace Sound {

PingusSoundReal::PingusSoundReal ()
  : music_sample (0), music_session(0)
{
  pout(PINGUS_DEBUG_SOUND) << "Initializing ClanLib-Sound" << std::endl;

  sound_output  = new CL_SoundOutput(44100);
  CL_SetupSound::init();

  pout(PINGUS_DEBUG_SOUND) << "Initializing ClanLib-MikMod" << std::endl;

#ifdef HAVE_LIBCLANVORBIS
  CL_SetupVorbis::init();
#endif

#ifdef HAVE_LIBCLANMIKMOD
  CL_SetupMikMod::init();
#endif
}

PingusSoundReal::~PingusSoundReal()
{
  real_stop_music();
  SoundResMgr::free_sound_map();

#ifdef HAVE_LIBCLANMIKMOD
  CL_SetupMikMod::deinit();
#endif

#ifdef HAVE_LIBCLANVORBIS
  CL_SetupVorbis::deinit();
#endif

  CL_SetupSound::deinit();
	sound_output->stop_all();
	delete sound_output;
}

struct sound_is_finished
{
  bool operator()(CL_SoundBuffer_Session& sess) {
    return !sess.is_playing();
  }
};

void
PingusSoundReal::real_play_sound(const std::string& name, float volume, float panning)
{
  if (!sound_enabled)
    return;

  SoundHandle buffer;
  CL_SoundBuffer_Session sess;

  try {
    buffer = SoundResMgr::load(name);
    sess   = buffer->prepare();
  } catch (const CL_Error & e) {
    perr(PINGUS_DEBUG_SOUND) << "Can't open sound '" << name << "' -- skipping\n"
			     << "  CL_Error: " << e.message << std::endl;
    return;
  }

  sess.set_volume(volume);
  sess.set_pan(panning);
  sess.set_looping(false);
  sess.play();
}

void
PingusSoundReal::real_stop_music ()
{
  if (music_session)
  {
    music_session->stop();
    delete music_session;
    music_session = 0;

    if (music_sample)
    {
      delete music_sample;
      music_sample = NULL;
    }
  }
}

void
PingusSoundReal::real_play_music (const std::string & arg_filename, float volume)
{
  std::string filename;

  filename = arg_filename;

  if (!music_enabled)
    return;

  pout(PINGUS_DEBUG_SOUND) << "PingusSoundReal: Playing music: " << filename << std::endl;

  real_stop_music();

  music_sample = 0;

  if (filename.substr(filename.size()-4, 4) == ".ogg")
    {
      #ifdef HAVE_LIBCLANVORBIS
      music_sample = new CL_SoundBuffer (filename.c_str());
      #endif
    }
  else if (filename.substr(filename.size()-4, 4) == ".wav")
    {
      music_sample = new CL_SoundBuffer (filename.c_str());
    }
  else
    {  // MikMod should support the rest...
      #ifdef HAVE_LIBCLANMIKMOD
      music_sample = new CL_SoundBuffer (filename.c_str());
      #endif
    }

  if (music_sample)
    {
      music_session = new CL_SoundBuffer_Session(music_sample->prepare());
      music_session->set_volume(volume * 0.5f); // FIXME: music_volume
      music_session->set_looping(true);
      music_session->play();
    }
}

} // namespace Sound

/* EOF */

