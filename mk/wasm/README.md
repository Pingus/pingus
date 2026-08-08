# WebAssembly packaging

- **`shell.html`** — Emscripten HTML shell (`{{{ SCRIPT }}}`, `@versionFull@` …).
- **`scripts/build-sdl2.sh`**, **`build-sdl2-image.sh`**, **`build-sdl2-mixer.sh`** — static deps (project-agnostic).
- **`scripts/build-zlib.sh`**, **`build-app.sh`**, **`serve.sh`**.

Nix glue: `nix/wasm.nix`.

Preferred: `nix build .#pingus-wasm`
