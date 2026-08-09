# Android audio — wstsound + OpenAL Soft

## Status

Wiring is in tree. Sound turns on when `AUDIO_ANDROID_LIBS` provides per-ABI
`libopenal.a` and `libmodplug.a` (nix `audioAndroidLibs`).

| Piece | Location |
|-------|----------|
| OpenFunc (assets) | `src/engine/sound/sound_real.cpp` |
| NDK build | `mk/android/scripts/build-audio-libs.sh` |
| Nix | `audioAndroidLibs` in `nix/android.nix` |
| Staging | `mk/android/scripts/build-apk.sh` |
| Link flags | `mk/android/app/jni/Android.mk` (`ENABLE_ANDROID_SOUND`) |

## Stack

`sound_real` → **wstsound** → **OpenAL Soft** (OpenSL ES) + **libmodplug** + WAV.

## Notes

- Empty datadir: paths like `music/foo.it` open via AssetManager.
- If OpenAL Soft fetch hash fails, update `openalSrc` in `nix/android.nix`.
