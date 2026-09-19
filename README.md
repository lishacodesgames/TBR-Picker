# TBR Picker
A simple app that helps you pick a book to read from your goodreads TBR

## How to use

### 1. Clone
`git clone --recursive https://github.com/lishacodesgames/TBR-Picker.git`

**If you forgot to use the recursive flag, run (after regular clone):** <br>
`git submodule update --init --recursive` after regular `git clone`

### 2. Export
Go to [the Goodreads export page](https://www.goodreads.com/review/import) and click `Export Library`. <br>
Once it's done exporting, it will show a hyperlink below the `Export` button with something like "Your export from date - time". <br>

Click that and save it to `assets/` inside the directory this repository is in. The `assets/` folder already exists.

Make sure the final filepath of the exported `.csv` file is `/path/to/repo/assets/goodreads_library_export.csv` ***exactly***.

### 3. Build & Run
```bash
mkdir build
cd build
cmake --preset Release -Wno-author
cmake --build --preset Release

./bin/Release/TBR
```
**Note:** <br> 
- for Windows, omit the `./` and add `.exe` at the end.
- Make sure you're running from the directory you cloned the repo to.
- Make sure you have cmake and ninja in your path variables.

*Check out [CMakePresets.json](CMakePresets.json) if you're confused* <br>

## Requirements
* CMake 3.20+
* Ninja (not *required*, but it's what the presets use)
