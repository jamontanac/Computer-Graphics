### User Manual for Stickman Animation in OpenGL

#### Introduction:
This OpenGL program creates a Stickman figure and implements simple animations of the articulations of the arms and legs. The user can interact with the Stickman by pressing specific keys to rotate different parts of the Stickman's body.

#### Requirements:
- A system with OpenGL installed.
- A JSON file containing the Stickman's body attributes.

### Building and Running the Stickman Animation Program

#### Pre-requisites:
- Ensure that CMake is installed on your system.
- Ensure that OpenGL and GLUT are installed on your system.
- Ensure that Boost is installed on your system.

#### Building the Program:
1. Open a terminal and navigate to the project directory where the `CMakeLists.txt` and the JSON file are located.
2. Create a new directory named `build` within the project directory:
   ```sh
   mkdir build
   ```
3. Navigate to the `build` directory:
   ```sh
   cd build
   ```
4. Run the `cmake` command to generate the Makefile:
   ```sh
   cmake ..
   ```
5. Run the `make` command to compile the program:
   ```sh
   make -j$(nproc)
   ```

#### Running the Program:
After successfully building the program, run it by providing the path to the JSON file as a command-line argument:
```sh
./Stickman ../your_json_file.json
```
Replace `your_json_file.json` with the name of your JSON file.

#### Notes:
- Ensure that the JSON file and `CMakeLists.txt` are located in the project directory, one folder up from the `build` folder.
- If you encounter any errors during the `cmake` or `make` commands, ensure that all the required packages are installed and that the `CMakeLists.txt` file is correctly configured.

#### User Interactions:
Users can interact with the Stickman using the following keys:
- **Numeric Keys (0-9):** Select the articulation to be rotated.
- **x/X:** Rotate the selected articulation around the x-axis.
- **y/Y:** Rotate the selected articulation around the y-axis.
- **z/Z:** Rotate the selected articulation around the z-axis.
- **Lowercase Letters (x, y, z):** Rotate in one direction.
- **Uppercase Letters (X, Y, Z):** Rotate in the opposite direction.

#### Example:
1. Pressing the numeric key `1` selects the first articulation.
2. Pressing the key `x` will rotate the selected articulation around the x-axis in one direction.
3. Pressing the key `X` will rotate the selected articulation around the x-axis in the opposite direction.

#### Notes:
The particular implementation done here moves a specific articulation of the arm and the articulation of the oposite leg, so if the user selects the left arm and make a rotation, the oposite leg (right in this case) will move as well rotating on the oposite direction. the idea of this implementation was to simulate the stickman dancing.

#### Stickman Components:
- **Torso and Head:** Created using `create_torso_head` function.
- **Arms:** Created using `create_arms` function.
- **Legs:** Created using `create_legs` function.
- **Joints:** Represented by spheres and created within the `create_arms` and `create_legs` functions.

#### JSON File Structure:
The JSON file should contain the Stickman's body attributes, including the radius and height of the torso, head, arms, and legs, and the number of joints in the arms and legs. Here is an example of the JSON structure:
```json
{
  "Torso": {"radius": value, "height": value},
  "Head": {"radius": value},
  "Arms": {"radius": value, "height": value, "nJoints": value},
  "Legs": {"radius": value, "height": value, "nJoints": value}
}
```
For example it should look similar to this one

```json
{
    "Torso": {
        "radius": 12,
        "height": 30,
        "children": {
            "Head": {
                "radius": 14
            },
            "Arms": {
                "radius": 3.5,
                "height": 30,
                "nJoints": 2 
            },
            "Legs": {
                "radius": 5,
                "height": 40,
                "nJoints": 2 
            }
        }
    }
}
```

#### Error Handling:
- If the JSON file path is not provided or the file is not found, an error message will be displayed, and the program will exit.
- If the JSON file is empty or cannot be read, an error message will be displayed.

This user manual provides the necessary information for interacting with the Stickman animation program in OpenGL. Users can rotate different parts of the Stickman's body using specific keys after selecting the desired articulation with numeric keys. The program requires a properly structured JSON file to run successfully.
