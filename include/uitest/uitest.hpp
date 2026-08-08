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

#ifndef HEADER_UITEST_HPP
#define HEADER_UITEST_HPP

#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <uitest/arg_info.hpp>

namespace uitesting {

class UITest
{
public:
  UITest() {}
  virtual ~UITest() {}
  virtual void TestBody(const std::vector<std::string>& args,
                        const std::vector<std::string>& rest) = 0;
};

class TestInfo
{
public:
  std::string m_name;
  std::string m_class;
  std::string m_function;
  ArgInfo m_args;
  std::string m_doc;
  std::function<std::unique_ptr<UITest> ()> m_factory;
};

std::vector<std::unique_ptr<TestInfo> >& g_tests();

TestInfo*
register_test(const std::string& _class, const std::string& _func,
              const std::function<std::unique_ptr<UITest> ()>& factory,
              const std::string& args_str = {}, const std::string& doc = {});

TestInfo*
find_testcase(const std::string& testcase);

} // namespace uitesting

// Some macro magic to make _arg and _doc argument optional, see:
// http://stackoverflow.com/a/3048361/28113
#define UITEST(...) UITEST_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

#define UITEST_GET_5TH_ARG(arg1, arg2, arg3, arg4, arg5, ...) arg5

#define UITEST_MACRO_CHOOSER(...)                                       \
  UITEST_GET_5TH_ARG(__VA_ARGS__,                                       \
                     UITEST_4_ARGS,                                     \
                     UITEST_3_ARGS,                                     \
                     UITEST_2_ARGS,                                     \
                     UITEST_1_ARGS)

#define UITEST_1_ARGS

#define UITEST_2_ARGS(_class, _func) UITEST_4_ARGS(_class, _func, "", "")

#define UITEST_3_ARGS(_class, _func, _arg) UITEST_4_ARGS(_class, _func, _arg, "")

#define UITEST_4_ARGS(_class, _func, _arg, _doc)                        \
  class _class##_##_func : public ::uitesting::UITest                   \
  {                                                                     \
  public:                                                               \
    static ::uitesting::TestInfo* const s_uitest_info __attribute__ ((unused));  \
    void TestBody(const std::vector<std::string>& args,                 \
                  const std::vector<std::string>& rest) override;       \
  };                                                                    \
  ::uitesting::TestInfo* const _class##_##_func::s_uitest_info          \
  = ::uitesting::register_test(                                         \
    #_class, #_func,                                                    \
    []() {                                                              \
    return std::unique_ptr<_class##_##_func>(new _class##_##_func);     \
    },                                                                  \
    _arg, _doc);                                                        \
  void _class##_##_func::TestBody(const std::vector<std::string>& args, \
                                  const std::vector<std::string>& rest)

// Some macro magic to make _arg and _doc argument optional, see:
// http://stackoverflow.com/a/3048361/28113
#define UITEST_S(...) UITEST_S_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

#define UITEST_S_GET_5TH_ARG(arg1, arg2, arg3, arg4, arg5, ...) arg5

#define UITEST_S_MACRO_CHOOSER(...)                                     \
  UITEST_S_GET_5TH_ARG(__VA_ARGS__,                                     \
                       UITEST_S_4_ARGS,                                 \
                       UITEST_S_3_ARGS,                                 \
                       UITEST_S_2_ARGS)

#define UITEST_S_2_ARGS

#define UITEST_S_3_ARGS(_class, _func, _arg) UITEST_S_4_ARGS(_class, _func, _arg, "")

#define UITEST_S_4_ARGS(_class, _func, _arg, _doc)                      \
  class _class##_##_func : public ::uitesting::UITest                   \
  {                                                                     \
  public:                                                               \
    static ::uitesting::TestInfo* const s_uitest_info __attribute__ ((unused));  \
    void TestBody(const std::vector<std::string>& args,                 \
                  const std::vector<std::string>& rest) override        \
    {                                                                   \
      for(const auto& arg : rest)                                       \
      {                                                                 \
        TestBodySimple(arg);                                            \
      }                                                                 \
    }                                                                   \
    void TestBodySimple(const std::string& arg);                        \
  };                                                                    \
  ::uitesting::TestInfo* const _class##_##_func::s_uitest_info          \
  = ::uitesting::register_test(                                         \
    #_class, #_func,                                                    \
    []() {                                                              \
        return std::unique_ptr<_class##_##_func>(new _class##_##_func); \
    },                                                                  \
    _arg "...",                                                         \
    _doc);                                                              \
  void _class##_##_func::TestBodySimple(const std::string& arg)

#endif

/* EOF */
