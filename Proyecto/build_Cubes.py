import numpy as np
from stl import mesh
import random
def create_cube(center, size):
    """
    Create a cube centered at 'center' with edge length 'size'.
    Returns the vertices and faces of the cube.
    """
    # Define the 8 vertices of the cube relative to the center
    vertices = np.array([
        [center[0] - size / 2, center[1] - size / 2, center[2] - size / 2], # 0
        [center[0] + size / 2, center[1] - size / 2, center[2] - size / 2], # 1
        [center[0] + size / 2, center[1] + size / 2, center[2] - size / 2], # 2
        [center[0] - size / 2, center[1] + size / 2, center[2] - size / 2], # 3
        [center[0] - size / 2, center[1] - size / 2, center[2] + size / 2], # 4
        [center[0] + size / 2, center[1] - size / 2, center[2] + size / 2], # 5
        [center[0] + size / 2, center[1] + size / 2, center[2] + size / 2], # 6
        [center[0] - size / 2, center[1] + size / 2, center[2] + size / 2]]) # 7

    # Define the 12 triangles composing the cube
    # faces = np.array([
    #     [0, 3, 1], [1, 3, 2], # Back
    #     [4, 5, 6], [4, 6, 7], # Front
    #     [0, 4, 7], [0, 7, 3], # Left
    #     [5, 1, 2], [5, 2, 6], # Right
    #     [2, 3, 6], [3, 7, 6], # Top
    #     [0, 1, 5], [0, 5, 4]]) # Buttom
    faces = np.array([
        [0, 3, 1],  # -z face (0)
        [1, 3, 2],  # -z face (1)
        [4, 5, 6],  # +z face (2)
        [4, 6, 7],  # +z face (3)
        [0, 4, 7],  # -x face (4)
        [0, 7, 3],  # -x face (5)
        [5, 1, 2],  # +x face (6)
        [5, 2, 6],  # +x face (7)
        [2, 3, 6],  # +y face (8)
        [3, 7, 6],  # +y face (9)
        [0, 1, 5],  # -y face (10)
        [0, 5, 4]]) # -y face (11)

    return vertices, faces

def get_faces_to_remove(cube_index, direction):
    """
    Remove the faces that are internal (in contact) based on the direction.
    """
    face_directions = {
        "x": [6, 7],  # +x face indices
        "-x": [4, 5],  # -x face indices
        "y": [8, 9],  # +y face indices
        "-y": [10, 11],  # -y face indices
        "z": [2, 3],  # +z face indices
        "-z": [0, 1] # -z face indices
    }

    faces_to_remove = [index +12*cube_index for index in face_directions[direction]]
    return faces_to_remove 

def create_random_cubes(start_center, size, n):
    """
    Create 'n' cubes, each with edge length 'size'.
    Each new cube is placed in a random direction relative to the previous cube.
    Returns the combined vertices and faces of all cubes.
    """
    all_vertices = []
    all_faces = []
    faces_to_remove = []
    directions = {"x":(1, 0, 0), "-x":(-1, 0, 0),
                  "y":(0, 1, 0), "-y": (0, -1, 0),
                  "z":(0, 0, 1), "-z":(0, 0, -1)}
    cube_directions = {}
    for i in range(n):
        vertices, faces = create_cube(start_center, size)
        all_vertices.append(vertices)
        all_faces.append(faces + (len(all_vertices)  - 1)*8)

        if i>0:
            previous_direction = cube_directions[i-1]
            faces_to_remove.extend(get_faces_to_remove(i-1, previous_direction))

            opposite_direction = {"x": "-x", "-x": "x", "y": "-y", "-y": "y", "z": "-z", "-z": "z"}[previous_direction]
            faces_to_remove.extend(get_faces_to_remove(i,opposite_direction))
            # faces = remove_internal_faces(faces, opposite_direction,i)
            # previous_faces = remove_internal_faces(all_faces[-1], previous_direction,i-1)
            # all_faces[-1] = previous_faces
            # cube_directions[i-1].append(opposite_direction)
            # faces = remove_internal_faces(faces,opposite_direction)
        
        # cube_directions[i] = []
        if i< n-1:
            # available_directions = {k:v for k, v in directions.items() if k not in cube_directions[i]}
            available_directions = list(directions.keys())
            if i in cube_directions:
                # Ensure not to select the opposite direction of the last cube
                available_directions.remove({"x": "-x", "-x": "x", "y": "-y", "-y": "y", "z": "-z", "-z": "z"}[cube_directions[i]])
            direction_key = random.choice(available_directions)
            cube_directions[i]=direction_key
            direction = directions[direction_key]
            start_center = np.array(start_center) + np.array(direction)*size
    print(cube_directions)
    combined_vertices = np.concatenate(all_vertices)
    combined_faces = np.concatenate(all_faces)

    return combined_vertices, combined_faces

n = 10  # Number of cubes
vertices, faces = create_random_cubes(start_center=[0, 0, 0], size=2, n=n)


cube = mesh.Mesh(np.zeros(faces.shape[0], dtype=mesh.Mesh.dtype))
for i, f in enumerate(faces):
    for j in range(3):
        cube.vectors[i][j] = vertices[f[j]]


cube.save('cube.stl')