// SPDX-FileCopyrightText: 2026 Ingo Ruhnke <grumbel@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later
//
// GCC 15 headers emit references to libstdc++ / libgcc symbols that are
// absent from the ArkOS sysroot (glibc ~2.30, libstdc++ from GCC ~9).
// Minimal shims so the R36S binary links and runs without shipping a modern
// libstdc++ (which would also demand a newer glibc).

#include <charconv>
#include <cstdio>
#include <exception>
#include <new>
#include <system_error>

// ---------------------------------------------------------------------------
// libgcc_eh (static) from GCC 15 references glibc 2.35's _dl_find_object.
// Returning failure is enough for FDE lookup to fall back to other methods.
// ---------------------------------------------------------------------------
struct dl_find_object
{
  unsigned long long dlfo_flags;
  void* dlfo_map_start;
  void* dlfo_map_end;
  void* dlfo_link_map;
  void* dlfo_eh_frame;
  void* dlfo_eh_dbase;
  void* dlfo_eh_count;
  // Padding / future fields ignored by our callers on failure.
  void* dlfo_reserved[7];
};

extern "C" int _dl_find_object(void* /*address*/, struct dl_find_object* /*result*/)
{
  return -1;
}

extern "C" void __cxa_call_terminate(void*) noexcept
{
  std::terminate();
}

namespace std {

__attribute__((noreturn)) void
__throw_bad_array_new_length()
{
  throw bad_array_new_length();
}

namespace {

to_chars_result
snprintf_to_chars(char* first, char* last, char const* fmt, double value)
{
  if (first == nullptr || last == nullptr || first > last)
  {
    return {last, errc::value_too_large};
  }
  auto const n = std::snprintf(first, static_cast<size_t>(last - first), fmt, value);
  if (n < 0 || n >= static_cast<int>(last - first))
  {
    return {last, errc::value_too_large};
  }
  return {first + n, errc{}};
}

char const*
format_to_printf(chars_format fmt)
{
  switch (fmt)
  {
    case chars_format::scientific:
      return "%e";
    case chars_format::fixed:
      return "%f";
    case chars_format::hex:
      return "%a";
    case chars_format::general:
    default:
      return "%g";
  }
}

} // namespace

// Floating to_chars used by libstdc++ std::format; not in GCC ~9 libstdc++.
to_chars_result
to_chars(char* first, char* last, float value)
{
  return snprintf_to_chars(first, last, "%g", static_cast<double>(value));
}

to_chars_result
to_chars(char* first, char* last, double value)
{
  return snprintf_to_chars(first, last, "%g", value);
}

to_chars_result
to_chars(char* first, char* last, float value, chars_format fmt)
{
  return snprintf_to_chars(first, last, format_to_printf(fmt), static_cast<double>(value));
}

to_chars_result
to_chars(char* first, char* last, double value, chars_format fmt)
{
  return snprintf_to_chars(first, last, format_to_printf(fmt), value);
}

to_chars_result
to_chars(char* first, char* last, float value, chars_format fmt, int precision)
{
  char buf[16];
  // e.g. "%.6g"
  std::snprintf(buf, sizeof(buf), "%%.%d%c", precision,
                (fmt == chars_format::scientific) ? 'e' :
                (fmt == chars_format::fixed) ? 'f' :
                (fmt == chars_format::hex) ? 'a' : 'g');
  return snprintf_to_chars(first, last, buf, static_cast<double>(value));
}

to_chars_result
to_chars(char* first, char* last, double value, chars_format fmt, int precision)
{
  char buf[16];
  std::snprintf(buf, sizeof(buf), "%%.%d%c", precision,
                (fmt == chars_format::scientific) ? 'e' :
                (fmt == chars_format::fixed) ? 'f' :
                (fmt == chars_format::hex) ? 'a' : 'g');
  return snprintf_to_chars(first, last, buf, value);
}

} // namespace std
