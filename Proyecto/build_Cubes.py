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

def create_multiple_cubes(start_center, size, n):
    """
    Create 'n' cubes, each with edge length 'size'.
    Each new cube is placed in a random direction relative to the previous cube.
    Returns the combined vertices and faces of all cubes.
    """
    all_vertices = []
    all_faces = []
    last_directions = []

    for i in range(n):
        if i == 0:
            center = start_center # we center it where we want
        else:
            possible_directions = ['x', '-x', 'y', '-y', 'z', '-z']
            for direction in last_directions:
                possible_directions.remove(direction) # we make sure we do not select the same direction than before
            #here we never go backwards

            direction = random.choice(possible_directions)# we randomly select a direction
            last_directions = [direction]

            #calculate the center for the new cube given the direction
            if direction == 'x':
                offset = np.array([size, 0, 0])
            elif direction == '-x':
                offset = np.array([-size, 0, 0])
            elif direction == 'y':
                offset = np.array([0, size, 0])
            elif direction == '-y':
                offset = np.array([0, -size, 0])
            elif direction == 'z':
                offset = np.array([0, 0, size])
            elif direction == '-z':
                offset = np.array([0, 0, -size])

            center = all_vertices[-1][0]+ size/2 + offset
        vertices, faces = create_cube(center, size)
        all_vertices.append(vertices)
        all_faces.append(faces + (len(all_vertices) - 1 )*8)

    combined_vertices = np.concatenate(all_vertices)
    combined_faces = np.concatenate(all_faces)

    return combined_vertices, combined_faces

n = 10  # Number of cubes
vertices, faces = create_multiple_cubes(start_center=[0, 0, 0], size=2, n=n)


cube = mesh.Mesh(np.zeros(faces.shape[0], dtype=mesh.Mesh.dtype))
for i, f in enumerate(faces):
    for j in range(3):
        cube.vectors[i][j] = vertices[f[j]]


cube.save('cube.stl')