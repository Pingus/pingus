/*
**  Windstille - A Sci-Fi Action-Adventure Game
**  Copyright (C) 2018 Ingo Ruhnke <grumbel@gmail.com>
**
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef HEADER_WSTSOUND_FWD_HPP
#define HEADER_WSTSOUND_FWD_HPP

#include <memory>

namespace wstsound {

class Effect;
class EffectSlot;
class Filter;
class FilteredSoundFile;
class Listener;
class OggSoundFile;
class OpenALBuffer;
class OpenALDevice;
class OpenALLoopbackDevice;
class OpenALRealDevice;
class OpenALSoundSource;
class OpenALSystem;
class OpenalContext;
class OpusSoundFile;
class ProceduralSoundFile;
class SoundChannel;
class SoundFile;
class SoundManager;
class SoundSource;
class WavSoundFile;

enum class FadeState;
enum class SoundSourceType;

using OpenALBufferPtr = std::shared_ptr<OpenALBuffer>;
using EffectPtr = std::shared_ptr<Effect>;
using EffectSlotPtr = std::shared_ptr<EffectSlot>;
using FilterPtr = std::shared_ptr<Filter>;
using SoundSourcePtr = std::shared_ptr<SoundSource>;
using SoundSourceWPtr = std::weak_ptr<SoundSource>;

} // namespace wstsound

#endif

/* EOF */
