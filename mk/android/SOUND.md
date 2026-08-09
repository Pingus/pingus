# Android audio — wstsound + OpenAL Soft

Status: **not enabled** (`PINGUS_NO_SOUND=1` in `app/jni/Android.mk`).
Checklist lives in the repo-root **`TODO.md`** (Android audio section).

## Goal

Same stack as desktop / wasm:

| Layer | Choice |
|-------|--------|
| Game | `src/engine/sound/sound_real.cpp` |
| Mixer | **wstsound** (`external/wstsound`) |
| Output | **OpenAL Soft** |
| Codecs | **libmodplug** (`.it` / `.s3m` music) + **WAV** SFX |

Stock `data/music` and `data/sounds` do not need vorbis/opus/mpg123.

## Work items

1. **OpenAL Soft** — CMake + NDK toolchain per `APP_ABI`; install
   `libopenal.so` (or static) + `AL/` headers into the APK ndk-build tree
   as a `PREBUILT_*` module.
2. **libmodplug** — static lib per ABI (ndk-build `Android.mk` or CMake);
   headers under `external_includes` / module includes.
3. **wstsound** — stage `external/wstsound/src` into `jni/src/deps/wstsound`
   with the same filters as wasm (`EMSCRIPTEN OR ANDROID` in
   `external/wstsound/CMakeLists.txt`: modplug on, EFX off, no
   vorbis/opus/mpg123 sources).
4. **Asset I/O** — `SoundFile::from_file` uses `ifstream` and cannot see
   APK assets. Prefer `SoundFile::from_stream` fed by
   `System::read_file` / `SDL_RWFromFile` in the Pingus sound backend.
5. **Enable** — remove `PINGUS_NO_SOUND`, stop filtering `sound_real.cpp`,
   link OpenAL + modplug; test menu music + SFX; pause on `onPause`.

## Non-goals

- SDL2_mixer as the primary API (optional alternative only)
- OpenAL EFX on mobile
- Full codec matrix

## Reference

- wasm path: `nix/wasm.nix` (`modplugWasm`, `wstsoundWasm`),
  `PINGUS_ENABLE_SOUND` in CMake
- wstsound: `external/wstsound/`
