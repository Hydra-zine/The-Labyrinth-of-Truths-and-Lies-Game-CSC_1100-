# The Labyrinth of Truths and Lies
A GUI-based maze game built in C++ with SDL2 for CSC 1100.
 
Navigate through the labyrinth by determining whether statements made by sentries are true or false. Watch out, sentries may not always be telling the truth!
 
---
 
## Gameplay
- Use **WASD** to move your character
- Each room presents a statement from a sentry
- **Go right** if you think the statement is true
- **Go left** if you think the statement is false
- You have **3 lives** — wrong answers cost you one
- Clear all 5 rooms to escape the labyrinth!
 
---
 
## Built With
- C++17
- [SDL2](https://www.libsdl.org/) - window, rendering, and input
- [SDL2_ttf](https://wiki.libsdl.org/SDL2_ttf/FrontPage) — font rendering
- CMake — cross-platform build system
 
---
 
## Running The Game
 
### Windows
A prebuilt `.exe` is available in the [Releases](../../releases) section.
Just unzip and double click `maze.exe` — no installs needed.
 
### Mac
```bash
brew install sdl2 sdl2_ttf
git clone [YOUR REPO LINK HERE]
cd [YOUR REPO NAME]
mkdir build && cd build
cmake ..
make
./maze
```
 
### Linux
```bash
sudo apt install libsdl2-dev libsdl2-ttf-dev
git clone [YOUR REPO LINK HERE]
cd [YOUR REPO NAME]
mkdir build && cd build
cmake ..
make
./maze
```
 
---
 
## Project Structure
```
├── src/
│   ├── main.cpp
│   ├── Player.h / Player.cpp
│   ├── NPC.h / NPC.cpp
│   ├── DialogueBox.h / DialogueBox.cpp
│   └── Sentry.h / Sentry.cpp
├── assets/
│   └── fonts/
│       └── BigBlueTerm437NerdFont-Regular.ttf
├── CMakeLists.txt
└── README.md
```
