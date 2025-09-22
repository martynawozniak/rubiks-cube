# Rubik's Cube Simulator

**C implementation of a Rubik’s Cube simulator that executes sequences of layer rotations on an `N x N x N` cube and prints its state.**  
Default cube size is `5x5x5`, but any `N` can be chosen at compile time.

## Notes
This project was implemented as part of an academic assignment in the course **Introduction to Programming**.

## Project Structure
```bash
├── cube.c          # Main source code
├── example1.in     # Example input and output files are provided externally
├── example1.out
├── example2.in
├── example2.out
├── example3.in
├── example3.out
└── README.md
```

## Problem Overview

The Rubik’s Cube is a 3D puzzle composed of colored square stickers arranged on six faces of a cube.  
In a solved state, each face has a uniform color. Moves are performed by rotating cube layers by multiples of 90 degrees.

This project simulates **cube rotations** starting from a solved cube and supports **printing the current state** upon request.

## Input Format

- Input is a sequence of **commands** ending with a `.` (dot).  
- Any characters after the final dot are ignored.  

### Command types:
1. **Print command** → a newline (`\n`) prints the current cube state.
2. **Rotation command** → consists of three parts:
   - **Number of layers** (optional; default = `1`)  
   - **Face letter**:
     - `u` = up  
     - `l` = left  
     - `f` = front  
     - `r` = right  
     - `b` = back  
     - `d` = down  
   - **Rotation angle**:
     - empty → 90° clockwise  
     - `'` (apostrophe) → -90° (counterclockwise)  
     - `"` (quote) → 180°  

Example rotations:
- `u` → rotate the top layer 90° clockwise  
- `2r'` → rotate the right 2 layers -90° (counterclockwise)  
- `f"` → rotate the front face 180°

## Output Format

- Cube state is printed using digits `0–5` to represent colors:
  - `0` = up, `1` = left, `2` = front, `3` = right, `4` = back, `5` = down  

- Layout of printed cube:
```bash
  u
l|f|r|b
  d
```
- Printed cube:
```bash
      00000
      00000
      00000
      00000
      00000
11111|22222|33333|44444
11111|22222|33333|44444
11111|22222|33333|44444
11111|22222|33333|44444
11111|22222|33333|44444
      55555
      55555
      55555
      55555
      55555
```

## Compilation & Usage

By default, the cube size is `5`. You can override it during compilation:

```bash
gcc -std=c17 -pedantic -Wall -Wextra -Wformat-security \
    -Wduplicated-cond -Wfloat-equal -Wshadow -Wconversion \
    -Wjump-misses-init -Wlogical-not-parentheses \
    -Wnull-dereference -Wvla -Werror \
    -fstack-protector-strong -fsanitize=undefined \
    -fno-sanitize-recover -g -fno-omit-frame-pointer -O1 \
    -DN=5 cube.c -o cube
```
Run with:

```bash
./cube < example1.in > example1.out
```

Change cube size (e.g. 3x3x3) with:

```bash
gcc -DN=3 cube.c -o cube
```
## Testing & Validation

Memory safety is checked with Valgrind:

```bash
valgrind --leak-check=full -q --error-exitcode=1 ./cube < input.in
```

Correctness is checked with diff:

```bash
./cube < example1.in | diff - example1.out
```
