import numpy as np
from stl import mesh
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

# vertices, faces = create_cube(center=[0, 0, 0], size=2)
cube1_vertices, cube1_faces = create_cube(center=[0, 0, 0], size=2)
cube2_vertices, cube2_faces = create_cube(center=[2, 0, 0], size=2)

vertices = np.concatenate((cube1_vertices, cube2_vertices))
faces = np.concatenate((cube1_faces, cube2_faces + len(cube1_vertices)))

cube = mesh.Mesh(np.zeros(faces.shape[0], dtype=mesh.Mesh.dtype))
for i, f in enumerate(faces):
    for j in range(3):
        # print(vertices[f[j],:])
        cube.vectors[i][j] = vertices[f[j]]


cube.save('cube.stl')