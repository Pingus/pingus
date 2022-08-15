// Copyright (c) 2013, 2016 Daniel Tillett
//               2022 Ingo Ruhnke <grumbel@gmail.com>
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// Based on:
// https://www.tillett.info/2013/05/13/how-to-create-a-windows-program-that-works-as-both-as-a-gui-and-console-application/

#include <assert.h>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

bool win32_redirect_stdio()
{
  if (AttachConsole(ATTACH_PARENT_PROCESS))
  {
    // Redirect unbuffered STDOUT to the console
    HANDLE consoleHandleOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (consoleHandleOut != INVALID_HANDLE_VALUE) {
      freopen("CONOUT$", "w", stdout);
      setvbuf(stdout, nullptr, _IONBF, 0);
    } else {
      return false;
    }

    // Redirect unbuffered STDERR to the console
    HANDLE consoleHandleError = GetStdHandle(STD_ERROR_HANDLE);
    if (consoleHandleError != INVALID_HANDLE_VALUE) {
      freopen("CONOUT$", "w", stderr);
      setvbuf(stderr, nullptr, _IONBF, 0);
    } else {
      return false;
    }

    return true;
  } else {
    freopen("stdout.txt", "w", stdout);
    freopen("stderr.txt", "w", stderr);

    return true;
  }
}

/* EOF */
