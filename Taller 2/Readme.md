# Instructions

### Objective

Implement a simulator, using OpenGL in C++, of parabolic motion.

### Description

Implement an algorithm (in your preferred programming language) that takes the following parameters through the command line:

`W H g_x g_y`

Where:

- `W` is the width, in meters, of the scene.
- `H` is the height, in meters, of the scene.
- `g_x` is the horizontal component of the gravity in the scene.
- `g_y` is the vertical component of the gravity in the scene.

The idea is to generate a visualization of a circle that will move across the screen following the laws of parabolic motion.

To achieve a level of interactivity, you must use the mouse to:

- Locate the circle.
- Set the direction and magnitude of the system's initial velocity.
- Launch the object.