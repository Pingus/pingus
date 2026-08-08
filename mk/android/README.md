# Android packaging

- **`app/`** — AndroidManifest, `res/`, `jni/` (game-specific `Android.mk`).
- **`scripts/`** — build recipes (SDL ndk-build, APK package); usable outside Nix with a normal SDK.
- **`keystore/`** — signing keystore (`debug.keystore` by default).

Nix glue: `nix/android.nix`. Flake: `appDir = ./mk/android/app`.

Preferred: `nix build .#pingus-android`
