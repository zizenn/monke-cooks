# monke-cooks

## this is the dev branch for the actual game


## description
This game is about cooking at your peak and serving as many customers as possible while making sure your food stays at an immaculate standard to maintain a high review.

## current stage
currently we are at a VERY BASIC level of development. currently the core game mechanics like cooking and stuff are still under HEAVY work and are not ready at all for a release.

## build and run
This repo is set up to build from local project files only.

- `include/` is for project headers only.
- raylib/raygui headers and raylib libraries live under `lib/`.
- **Windows:** uses local `lib/libraylib.a`.
- **Linux:** uses local `lib/linux/libraylib.a`.
- **macOS:** uses local `lib/macos/libraylib.a`.
- The Makefile does not download raylib. If a platform library is missing, `make` stops with an error telling you which file to add.

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
