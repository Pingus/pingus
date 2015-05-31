// Pingus - A free Lemmings clone
// Copyright (C) 1998-2015 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "json_file_writer.hpp"

JsonFileWriter::JsonFileWriter()
{
}

void
JsonFileWriter::begin_collection(const char* name)
{
}

void
JsonFileWriter::end_collection()
{
}

void
JsonFileWriter::begin_mapping(const char* name)
{
}

void
JsonFileWriter::end_mapping()
{
}

void
JsonFileWriter::write_int(const char* name, int)
{
}

void
JsonFileWriter::write_float(const char* name, float)
{
}

void
JsonFileWriter::write_colorf(const char* name, const Color&)
{
}

void
JsonFileWriter::write_colori(const char* name, const Color&)
{
}

void
JsonFileWriter::write_bool(const char* name, bool)
{
}

void
JsonFileWriter::write_string(const char* name, const std::string&)
{
}

void
JsonFileWriter::write_vector(const char* name, const Vector3f&)
{
}

void
JsonFileWriter::write_size(const char* name, const Size&)
{
}

void
JsonFileWriter::write_vector2i(const char* name, const Vector2i&)
{
}

void
JsonFileWriter::write_path(const char* name, const Pathname&)
{
}

/* EOF */
