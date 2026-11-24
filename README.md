# Asteroid Destroyer

Final project for CSC-335. An interactive 3D arcade game built with C++ and
openFrameworks: the player pilots a spaceship, fires missiles with a limited
lifetime, destroys asteroids and can switch between four camera viewports.
It includes lighting, textures and sound effects.

## Controls

- `W` / `S` — up and down
- Arrow keys — forward, backwards, left, right
- `Space` — shoot
- `R` — reset

## Files

- `main.cpp`, `ofApp.h`, `ofApp.cpp` — application and game loop
- `asteroid.h`, `asteroid.cpp` — asteroids
- `Missile.h` — missiles
- `CMakeLists.txt` — CMake build

## Note on assets

The original project uses third-party 3D models, textures and sounds. Those
asset files are not included in this repository because of their size and
licence. You need an openFrameworks 0.12.1 setup to build and run the code.

The executable reads model and sound files from fixed relative paths inside the
working directory. Only place trusted assets in those locations; a modified
model or audio file can crash the third-party loaders. Missing assets now
produce a warning in the console instead of failing silently.
