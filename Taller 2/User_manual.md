

## User Manual for Parabolic Motion Simulator
----
### Introduction

This C++ program utilizes OpenGL to simulate the parabolic movement of a particle. You can visualize a circle moving across the screen, with options to control the direction and velocity of the movement using your mouse.

### Requirements

- C++ Compiler with OpenGL support
- GLUT Library

### Compilation

Compile the program with the following command (Take into account that this program was developed in MACOS so the flags of compilations and the name of the libraries in the program could change):

```bash
g++ -std=c++17 Ball_Dynamics.cpp -o program.out -framework OpenGL -framework GLUT
```


### Execution

Run the compiled program with the following command:

```bash
./program.out W H g_x g_y
```

Where:
- `W`: The width of the scene in meters
- `H`: The height of the scene in meters
- `g_x`: The horizontal component of gravity
- `g_y`: The vertical component of gravity

### Using the Simulator

Once the program is running, you'll see a window displaying the simulation. You can interact with the program using your mouse:

1. **Locate the Circle**: Click the left mouse button on the desired position to set the location of the circle within the window.
2. **Configure Initial Velocity**: Hold down the left mouse button and move the cursor away from the circle to set the direction and magnitude of the initial velocity.
3. **Launch the Object**: Release the left mouse button to launch the object. The circle will move following the laws of parabolic motion, reflecting off the walls of the scene.
4. **Exiting the Program**: Press the `ESC` key to close the program.

### Understanding the Simulation

- **Particle Struct**: The program utilizes a structure to define the position and velocity of the particle.
- **UpdateParticle**: This implementation uses the leap-frog integration method to simulate the dynamics of the ball in the presence of certain forces.
- **Collisions**: The simulation detects collisions with the walls, inverting the corresponding velocity component and tracking the number of collisions.
- **Circle Drawing**: A function `circle` is used to draw the particle on the screen.

### Limitations

- The program will terminate if the particle continues to bounce more than 40 times.

### Remarks 

This user-friendly program simulates parabolic motion with OpenGL and offers an interactive experience to understand the underlying physics.