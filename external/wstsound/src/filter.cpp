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

#include "filter.hpp"

namespace wstsound {

Filter::Filter(int filter_type) :
  m_handle()
{
  alGenFilters(1, &m_handle);
  alFilteri(m_handle, AL_FILTER_TYPE, filter_type);
}

Filter::~Filter()
{
  alDeleteFilters(1, &m_handle);
}


void
Filter::seti(int param, int value) // NOLINT
{
  alFilteri(m_handle, param, value);
}

void
Filter::setf(int param, float value) // NOLINT
{
  alFilterf(m_handle, param, value);
}


} // namespace wstsound

/* EOF */
