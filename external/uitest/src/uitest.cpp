// UITest - Simple testing framework for interactive tests
// Copyright (C) 2015 Ingo Ruhnke <grumbel@gmail.com>
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

#include <uitest/uitest.hpp>
#include <stdexcept>

namespace uitesting {

std::vector<std::unique_ptr<TestInfo> >& g_tests()
{
  static std::vector<std::unique_ptr<TestInfo> > s_tests;
  return s_tests;
}

TestInfo*
register_test(const std::string& _class, const std::string& _func,
              const std::function<std::unique_ptr<UITest> ()>& factory,
              const std::string& args_str, const std::string& doc)
{
  ArgInfo args = ArgInfo::from_string(args_str);

  g_tests().push_back(
    std::unique_ptr<TestInfo>(
      new TestInfo{_class + "." + _func, _class, _func, args, doc, factory}));

  return g_tests().back().get();
}

TestInfo*
find_testcase(const std::string& testcase)
{
  for(const auto& testinfo : g_tests())
  {
    if (testinfo->m_name == testcase)
    {
      return testinfo.get();
    }
  }
  return nullptr;
}

} // namespace uitesting

/* EOF */
