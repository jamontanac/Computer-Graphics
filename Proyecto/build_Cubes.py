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
# def create_multiple_cubes(n, size, spacing):
#     all_vertices = []
#     all_faces = []

#     for i in range(n):
#         # Calculate the center of each cube
#         center = [i * (size + spacing), 0, 0]

#         vertices, faces = create_cube(center, size)
#         faces += len(all_vertices)  # Offset the face indices

#         all_vertices.append(vertices)
#         all_faces.append(faces)

#     # Combine vertices and faces
#     all_vertices = np.concatenate(all_vertices)
#     all_faces = np.concatenate(all_faces)

#     return all_vertices, all_faces
def create_multiple_cubes(start_center, size, n, offset):
    """
    Create 'n' cubes, each with edge length 'size'.
    Cubes are offset from 'start_center' by 'offset' units along the x-axis.
    Returns the combined vertices and faces of all cubes.
    """
    all_vertices = []
    all_faces = []

    for i in range(n):
        # Calculate the center for the current cube
        center = start_center + np.array([i * offset, 0, 0])
        # Create the cube
        vertices, faces = create_cube(center, size)

        # Append the new vertices and update faces indices
        all_vertices.append(vertices)
        all_faces.append(faces + (len(all_vertices)  - 1)*8)

    # Combine all vertices and faces
    combined_vertices = np.concatenate(all_vertices)
    combined_faces = np.concatenate(all_faces)

    return combined_vertices, combined_faces

# Example usage
n = 2  # Number of cubes
vertices, faces = create_multiple_cubes(start_center=[0, 0, 0], size=2, n=n, offset=2)
print(vertices)
print("-"*10)
print(faces)

# n = 3  # Number of cubes
# size = 2  # Size of each cube
# spacing = 1  # Spacing between each cube
# vertices, faces = create_multiple_cubes(n, size, spacing)
# print(vertices)
# print(faces)

cube = mesh.Mesh(np.zeros(faces.shape[0], dtype=mesh.Mesh.dtype))
for i, f in enumerate(faces):
    for j in range(3):
        cube.vectors[i][j] = vertices[f[j]]


cube.save('cube.stl')