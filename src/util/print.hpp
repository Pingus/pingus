// SPDX-FileCopyrightText: 1999–2026 Ingo Ruhnke <grumbel@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef HEADER_PINGUS_UTIL_PRINT_HPP
#define HEADER_PINGUS_UTIL_PRINT_HPP

#include <cstdio>
#include <format>
#include <string>
#include <utility>

// C++23 std::print / std::println when available; otherwise a small
// std::format + fwrite polyfill (R36S / older libstdc++, etc.).
#if defined(__cpp_lib_print) && __cpp_lib_print >= 202207L
#  include <print>
#  define PINGUS_HAS_STD_PRINT 1
#else
#  define PINGUS_HAS_STD_PRINT 0
#endif

namespace pingus {

#if PINGUS_HAS_STD_PRINT

template<class... Args>
void print(std::FILE* stream, std::format_string<Args...> fmt, Args&&... args)
{
  std::print(stream, fmt, std::forward<Args>(args)...);
}

template<class... Args>
void println(std::FILE* stream, std::format_string<Args...> fmt, Args&&... args)
{
  std::println(stream, fmt, std::forward<Args>(args)...);
}

template<class... Args>
void println(std::format_string<Args...> fmt, Args&&... args)
{
  std::println(fmt, std::forward<Args>(args)...);
}

#else // polyfill

template<class... Args>
void print(std::FILE* stream, std::format_string<Args...> fmt, Args&&... args)
{
  auto s = std::format(fmt, std::forward<Args>(args)...);
  fwrite(s.data(), 1, s.size(), stream);
}

template<class... Args>
void println(std::FILE* stream, std::format_string<Args...> fmt, Args&&... args)
{
  auto s = std::format(fmt, std::forward<Args>(args)...);
  s.push_back('\n');
  fwrite(s.data(), 1, s.size(), stream);
}

template<class... Args>
void println(std::format_string<Args...> fmt, Args&&... args)
{
  println(stdout, fmt, std::forward<Args>(args)...);
}

#endif

/** Diagnostic line on stderr (always flushed). Prefer over iostreams. */
template<class... Args>
void print_err(std::format_string<Args...> fmt, Args&&... args)
{
  println(stderr, fmt, std::forward<Args>(args)...);
  fflush(stderr);
}

} // namespace pingus

#endif

/* EOF */
