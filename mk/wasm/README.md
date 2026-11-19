# WebAssembly packaging

- **`shell.html`** — Emscripten HTML shell (`{{{ SCRIPT }}}`, `@versionFull@` …).
- **`scripts/build-sdl3.sh`**, **`build-sdl3-image.sh`** — static SDL3 / SDL3_image from nixpkgs' sources (`nix build .#wasm-sdl3 .#wasm-sdl3-image`).
- **`scripts/build-zlib.sh`**, **`build-app.sh`**, **`serve.sh`**.

Nix glue: `nix/wasm.nix`.

Preferred: `nix build .#pingus-wasm`
