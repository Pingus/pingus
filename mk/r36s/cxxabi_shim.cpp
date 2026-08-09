// SPDX-FileCopyrightText: 2026 Ingo Ruhnke <grumbel@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later
//
// GCC 15 headers can emit references to libstdc++ symbols that are absent
// from ArkOS's older (GCC ~9-era) libstdc++. Provide minimal shims so the
// R36S binary can link against the sysroot runtime without shipping a
// modern libstdc++ (which would also demand a newer glibc).

#include <exception>
#include <new>

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

} // namespace std
