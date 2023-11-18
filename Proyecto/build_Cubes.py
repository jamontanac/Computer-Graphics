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
        [center[0] - size / 2, center[1] - size / 2, center[2] - size / 2],
        [center[0] + size / 2, center[1] - size / 2, center[2] - size / 2],
        [center[0] + size / 2, center[1] + size / 2, center[2] - size / 2],
        [center[0] - size / 2, center[1] + size / 2, center[2] - size / 2],
        [center[0] - size / 2, center[1] - size / 2, center[2] + size / 2],
        [center[0] + size / 2, center[1] - size / 2, center[2] + size / 2],
        [center[0] + size / 2, center[1] + size / 2, center[2] + size / 2],
        [center[0] - size / 2, center[1] + size / 2, center[2] + size / 2]])

    # Define the 12 triangles composing the cube
    faces = np.array([
        [0, 3, 1],
        [1, 3, 2],
        [4, 5, 6],
        [4, 6, 7],
        [0, 4, 7],
        [0, 7, 3],
        [5, 1, 2],
        [5, 2, 6],
        [2, 3, 6],
        [3, 7, 6],
        [0, 1, 5],
        [0, 5, 4]])

    return vertices, faces

def create_random_cubes(start_center, size, n):
    """
    Create 'n' cubes, each with edge length 'size'.
    Each new cube is placed in a random direction relative to the previous cube.
    Returns the combined vertices and faces of all cubes.
    """
    all_vertices = []
    all_faces = []
    directions = {"x":(1, 0, 0), "-x":(-1, 0, 0), "y":(0, 1, 0), "-y": (0, -1, 0), "z":(0, 0, 1), "-z":(0, 0, -1)}
    cube_directions = {}

    for i in range(n):
        vertices, faces = create_cube(start_center, size)
        all_vertices.append(vertices)
        all_faces.append(faces + (len(all_vertices)  - 1)*8)
        cube_directions[i] = []
        if i>0:
            opposite_direction = {v:k for k,v in directions.items()}[tuple(direction)]
            cube_directions[i-1].append(opposite_direction)
        if i< n-1:
            available_directions = {k:v for k, v in directions.items() if k not in cube_directions[i]}
            direction_key = random.choice(list(available_directions.keys()))
            direction = available_directions[direction_key]
        
            start_center = np.array(start_center) + np.array(direction)*size

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