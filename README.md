# TBR Picker
A simple app that helps you pick a book to read from your goodreads TBR

## How to use
Clone, upload your github csv to `assets/`, then build and run.

### Clone
`git clone --recursive https://github.com/lishacodesgames/TBR-Picker.git`

**If you forgot to use the recursive flag, run (after regular clone):** <br>
`git submodule update --init --recursive` after regular `git clone`

### Build & Run
```bash
mkdir build
cd build
cmake --preset Release -Wno-author
cmake --build --preset Release

./bin/Release/TBR
```
**Note:** <br> 
- for Windows, omit the `./` and add `.exe` at the end
- Make sure you're running from the repo directory `YourProject/`
- Make sure you have cmake and ninja in your path variables

*Check out [CMakePresets.json](CMakePresets.json) if you're confused* <br>

### Linux Dependencies
Raylib requires a lot of libraries that are built-in on Mac and Windows. <br>
No worries, Linux just requires 1 command to install them all:
```bash
sudo apt install \
    libx11-dev \
    libxcursor-dev \
    libxrandr-dev \
    libxi-dev \
    libxinerama-dev \
    libgl1-mesa-dev \
    libasound2-dev \
    libpulse-dev \
    libxkbcommon-dev
```

**Note:** I've made it so that you don't need to touch any CMakeLists.txt to run <br>
If any issues arise, lmk :)

## Updating raylib (the submodule)
```bash
cd lib/raylib
git pull
cd ../..
git add lib/raylib
git commit -m "Updated raylib submodule"
```

## Requirements
* CMake 3.20+
* Ninja (not *required*, but it's what the presets use)