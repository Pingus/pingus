# AGENTS.md — Pingus

## Project Overview

**Pingus** is a free Lemmings™ clone with penguins. It is a puzzle game written
in C++ using SDL2, CMake, and a Nix flake for reproducible builds. The project
targets GNU/Linux, Windows (cross-compiled), and other platforms supported by
SDL2.

| Path | Role |
|------|------|
| `src/` | Game engine, screens, actions, world objects, editor, util |
| `data/` | Levels, graphics, sounds, music, fonts, icons |
| `extra/` | Optional extra tools / demos (built when `BUILD_EXTRA=ON`) |
| `test/`, `tests/`, `uitest/` | Unit / interactive tests |
| `tools/` | Helper utilities |
| `mk/cmake/` | CMake modules (TinyCMMC integration) |
| `doc/` | Man pages and documentation |

License: **GPL-3.0-or-later**. Prefer REUSE-style SPDX headers on new or
substantially modified source files:

```cpp
// SPDX-FileCopyrightText: 1999–2026 Ingo Ruhnke <grumbel@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later
```

(Adjust year/author when contributing new original work. Existing files may
use older copyright notices; do not mass-rewrite them without reason.)

Repository: https://github.com/Pingus/pingus  
Homepage: https://pingus.github.io/

Port / flake progress checklist: **`TODO.md`** (keep it current).

---

## Goals

1. Keep Pingus a **fun, solid Lemmings-style puzzle game** that builds and runs
   cleanly on modern systems.
2. Prefer **clean design over hacks**. Document workarounds. Prefer debug
   logging when stuck.
3. Maintain **reproducible builds** via the Nix flake while remaining friendly
   to distro packaging (system libraries when appropriate).
4. Support both **native Linux** and **Windows cross builds** from the same
   tree.
5. Keep the dependency surface manageable. Related small C++ libraries
   (`tinycmmc`, `argpp`, `geomcpp`, `logmich`, `priocpp`, `strutcpp`,
   `tinygettext`, `xdgcpp`, `wstsound`, `uitest`, …) are currently flake inputs;
   longer-term they may move under `external/` as git subtrees.

---

## Architecture Snapshot

| Area | Location / notes |
|------|------------------|
| Engine (display, input, sound, GUI, resources) | `src/engine/` |
| Game logic (actions, colliders, worldobjs, screens, worldmap) | `src/pingus/` |
| Math / util | `src/math/`, `src/util/` |
| Level editor | `src/editor/` |
| Main entry | `src/main.cpp` (+ Win32 helpers under `src/win32/`) |
| CMake bootstrap | `mk/cmake/TinyCMMC.cmake` + `tinycmmc` |
| Data (levels, gfx, audio) | `data/` (installed under `$datadir/pingus`) |

CMake drives the build. Dependencies are discovered via `tinycmmc_find_dependency`
(or system `find_package` / pkg-config). The Nix flake wires the same
libraries as packages and passes them into `pingus.nix`.

---

## Coding Standards

### Language & style

- Modern C++ (C++17 / C++20 as used by the current toolchain and dependencies).
  Prefer clear, readable code over cleverness.
- Prefer value types and clear ownership (RAII, smart pointers where needed).
- Avoid exceptions across public library / engine boundaries for ordinary
  errors when the surrounding style already uses other error handling.
- Logging: use the project’s logging facilities (`logmich` etc.). When
  diagnosing hard bugs, **add debug logs** — do not paper over with silent
  catches.
- Keep platform-specific code isolated (`#ifdef`, dedicated `src/win32/`
  files, or CMake conditionals).

### What not to do

- Do **not** introduce new heavy third-party dependencies without discussion.
- Do **not** embed low-level platform or filesystem logic deep inside game
  screens when a utility or engine layer already exists.
- Do **not** invent workarounds without understanding the root cause.
- Do **not** break the Nix flake or the non-Nix CMake build path without
  updating both.

### Documentation (required)

- Keep `README.md`, `NEWS`, and any active TODO / issue notes accurate when
  behaviour or build requirements change.
- When adding or changing user-visible features, levels, or controls, update
  the relevant documentation or man page if one exists.

### Git commit messages

After every coherent series of changes, leave a **detailed suggested commit
message** for the human (subject ≤ ~72 chars, body explaining why and what).

---

## Build & environment

- Primary build system: **CMake** + Ninja (or Make).
- Dev shell / packaging / Windows cross builds: **Nix flake** (`flake.nix`,
  `pingus.nix`).
- Typical Nix usage:

  ```sh
  nix run github:Pingus/pingus          # or .# for local
  nix build .#pingus
  nix build .#pingus-win32-x64          # Windows cross (host = Linux)
  nix build .#pingus-win32-x86
  ```

- Non-Nix builds: configure with CMake, ensure dependencies (SDL2, SDL2_image,
  libpng, OpenGL, fmt, boost bits as needed, and the small C++ helper
  libraries) are available via pkg-config / CMake config packages or under
  `external/`.

Target for day-to-day development: **Linux**. Keep Windows and other
platform code isolated.

Experimental ports (WASM, Android, R36S) live under `mk/` + `nix/` and are
wired as host-system flake packages (`pingus-wasm`, `pingus-android`,
`pingus-r36s`). Treat them as work-in-progress until GLES2/Emscripten and
NDK builds of `external/` are solid.

---

## Dependencies & Nix flake

The flake pins a full dependency set (SDL2 family, OpenGL, fmt, the various
`*cpp` helper libraries, etc.).

- Prefer the versions provided by the flake for official / reproducible
  builds.
- For distro packaging, CMake should still be able to find **system**
  libraries when the vendored / flake-provided copies are absent (or when an
  explicit “use system libs” option is set). Do not hard-require the exact
  flake pins for every downstream build.
- Do **not** add runtime “try tool A then tool B” fallbacks for libraries or
  binaries the build already guarantees. Missing dependencies are a packaging
  bug; fail clearly at configure or run time.

Helper libraries live under `external/` (git subtrees). CMake resolves them
via `tinycmmc_find_dependency` (`find_package` first, then
`add_subdirectory(external/…)`). The Nix flake builds those same trees with
`callPackage` and no longer pulls them in as separate flake inputs. System
packages may still satisfy `find_package` for distro builds.

---

## Working with data and levels

- Game data lives under `data/`. Levels, sprites, and audio are first-class
  content; treat them with the same care as code.
- Do not commit generated or temporary files (build dirs, editor autosaves,
  etc.).
- New levels or assets should be clearly attributed when they come from
  external contributors.

---

## Git workflow (for agents / contributors)

Repository: https://github.com/Pingus/pingus.git

- Make every coherent change a **separate commit**. Prefer small, reviewable
  commits; do not bulk-reformat unrelated code in the same commit as a
  functional change.
- After each coherent series, leave a **detailed suggested commit message**
  (subject ≤ ~72 chars, body explaining why and what).
- Update documentation (`README.md`, man pages, `NEWS`, …) in the same series
  when user-visible behaviour or build requirements change.

### History is append-only (agents)

- **Never rewrite, reset, rebase, or re-root history** against `origin/master`
  or an earlier tip unless the human **explicitly** asks for a rollback of a
  failed change (e.g. “drop the last commit”, “revert this series”).
- **Never** `git reset --hard` to `origin`, re-clone over a lost tip and
  “rebuild” prior commits with new SHAs, or ship a bundle whose parent is an
  older commit when the consumer already applied a newer tip.
- If the agent workspace lost local objects, **recover the consumer tip first**
  (`git fetch` / `git pull` the remote or the last applied bundle) and continue
  from that tip. Do not invent a parallel history from an older base.
- Fix mistakes with **new commits** (or `git revert`) on top of the current tip,
  not by replacing earlier commits.

### Commit author and AI attribution

- **Author** (who owns the commit for `git blame`, GitHub, and bug reports):
  the human maintainer — currently **Ingo Ruhnke <grumbel@gmail.com>**.
  Agents must **not** use a placeholder or agent-only identity as `user.email`.
- **AI assistance** is recorded with a standard Git trailer at the end of the
  commit message body (not as the author):

  ```
  Co-authored-by: Grok <grok@x.ai>
  ```

  Example:

  ```
  engine: fix OpenGL context loss on window resize

  Recreate the GL state after SDL window events that invalidate the
  context on some drivers.

  Co-authored-by: Grok <grok@x.ai>
  ```

- Do not put `grok@x.ai` (or similar) in the author/committer email field so
  automated mail and bug tooling stay aimed at a real person.

### Handoff: git bundle only

Agent handoffs use **`git bundle` + `git pull` only**.

**Forbidden for handoffs (never suggest, never use):**

- `git fetch` (of a handoff bundle or as a substitute for stacking)
- `git cherry-pick`
- `git format-patch` / `.mbox` / `git am`
- `git rebase` onto an older parent to “fix” a non-stacking bundle
- Any workflow that applies commits out of order or rewrites the consumer tip

If a bundle does not fast-forward onto the consumer’s current tip, the
**producer** was wrong — do not invent consumer-side workarounds. Produce a
new bundle whose required parent is exactly the tip the consumer already has.

Bundles carry real commits and chain cleanly **only** when each bundle is based
on the consumer’s current tip.

**Bundles must stack.** Each handoff bundle’s required parent is the tip the
consumer already has (last applied bundle or `origin/master` after they push).
Successive bundles form one linear history: `… → tip₀ → bundle₁ → tip₁ →
bundle₂ → tip₂ → …`. Do not produce a bundle that re-applies older work from
under an already-applied tip unless the human explicitly requested a rollback.

**Bundle filenames must be numbered.** Use a zero-padded three-digit prefix
(`%03d`) so apply order is obvious, e.g.:

    pingus-001-short-description.bundle
    pingus-002-next-change.bundle

Numbering is monotonic within the current handoff series. **Restart at
`001`** when the human declares a new upstream/base commit.

**Producer** (commits not yet on the consumer’s tip; base = last applied tip):

```sh
# already-applied-tip = consumer HEAD (e.g. last bundle tip or origin/master)
git bundle create changes.bundle <already-applied-tip>..HEAD
git bundle verify changes.bundle
```

State the required parent SHA in the handoff note so the consumer can confirm
before pulling. The parent **must** be the SHA the consumer reported (or the
tip of the previous handoff bundle), never an older `origin/master` when a
newer tip already exists.

**Consumer (only this):**

```sh
git pull changes.bundle HEAD
```

That is the entire apply path. No `git fetch`, no `git cherry-pick`, no side
branch. If `git pull` does not fast-forward, stop and ask for a correctly
stacked bundle.
