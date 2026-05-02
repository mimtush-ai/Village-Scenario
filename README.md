# Village-Scenario
A real-time 2D OpenGL scene simulating a Bengali riverside village with dynamic weather, day/night cycle, animated boats, and interactive elements.
## Village Scenario & Camping — OpenGL 2D Scene

A real-time interactive 2D graphics project built with OpenGL (GLUT) in C++, 
depicting a scenic Bengali riverside village and camping environment.

### Features

- **Day / Sunset / Night cycle** with smooth sky color transitions, 
  rising sun and moon animations
- **Dynamic weather** — toggleable rain with lightning flash effects
- **River with 3 boats**
  - Sailing boat (keyboard controlled speed/direction)
  - Realistic wooden boat (independently controlled)
  - Perspective crossing boat — crosses the river with zoom in/out 
    effect simulating depth, other boats pause to avoid collision
- **Animated elements** — flying birds with wing flap, drifting clouds, 
  flickering campfire, airplane flyby
- **Detailed scene** — mountains, forest, houses, tent, stone riverbanks, 
  grass and flowers, road
- **Keyboard-driven interaction** for controlling boats, weather, 
  time of day, and airplane

### Built With
- C++ 
- OpenGL / GLUT
- Math (trigonometry for circles, arcs, bird wings)

### Controls
| Key | Action |
|-----|--------|
| `m` | Start sunset |
| `n` | Start night |
| `d` | Return to day |
| `r` | Start rain |
| `1` | Stop rain |
| `t` | Lightning on |
| `2` | Lightning off |
| `a/x` | Boat 1 run/stop |
| `f/s/z` | Boat 1 fast/slow/normal |
| `g/h` | Boat 2 run/stop |
| `j/k/l` | Boat 2 fast/slow/normal |
| `[` | Boat 3 cross river |
| `]` | Boat 3 return |
| `p/o` | Airplane left-to-right / right-to-left |
