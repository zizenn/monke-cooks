# monke-cooks

## description
This game is about cooking at your peak and serving as many customers as possible while making sure your food stays at an immaculate standard to maintain a high review.

## current stage
currently we are at a VERY BASIC level of development. currently the core game mechanics like cooking and stuff are still under HEAVY work and are not ready at all for a release.

## build and run
This repo is set up to build from local project files only.

- `include/` is for project headers only.
- raylib/raygui headers and raylib libraries live under `lib/`.
- **Windows:** includes local `lib/libraylib.a`, so `make` then `make run`.
- **Linux/macOS:** on first `make`, raylib is cloned into `third_party/raylib` and built locally, then the game links against `lib/linux/libraylib.a` or `lib/macos/libraylib.a`.

After that, anyone can run:

```bash
make
make run
```

## team
1. Sakif (Lead Dev / Founder)
2. Marcus (Co-Founder / Artist)
3. Eaton (Dev)
4. Sameer (Lead Artist)
5. Ryan (Lead Musician)

# Wanted Features
1. cooking (ofc)
2. campaign mode
3. multiplayer
4. review system
5. upgrading
6. bosses
7. food quality system
