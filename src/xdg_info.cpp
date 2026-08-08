// Copyright (C) 2018 Ingo Ruhnke <grumbel@gmail.com>
//
// This library is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <iostream>

#include <xdg.h>

int main()
{
    std::cout << "xdg::data().home():\n  " << xdg::data().home() << std::endl;
    std::cout << std::endl;

    std::cout << "xdg::data().dirs():" << std::endl;
    for(auto const& dir : xdg::data().dirs())
        std::cout << "  " << dir << std::endl;
    std::cout << std::endl;

    std::cout << "xdg::config().home():\n  " << xdg::config().home() << std::endl;
    std::cout << std::endl;

    std::cout << "xdg::config().dirs():" << std::endl;
    for(auto const& dir : xdg::config().dirs())
        std::cout << "  " << dir << std::endl;
    std::cout << std::endl;

    std::cout << "xdg::state().home():\n  " << xdg::state().home() << std::endl;
    std::cout << std::endl;

    std::cout << "xdg::cache().home():\n  " << xdg::cache().home() << std::endl;
    std::cout << std::endl;

    std::cout << "xdg::runtime().dir():\n  " << xdg::runtime().dir() << std::endl;

    return 0;
}
