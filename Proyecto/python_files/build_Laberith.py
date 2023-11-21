import numpy as np
from stl import mesh
import random
import sys
import vtk
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
    Determine the indices of faces to be removed for a given cube and direction.
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
    Each new cube is placed in a random direction from the previous one.
    Returns the combined vertices and faces of all cubes, and the faces to be removed.
    """
    all_vertices = []
    all_faces = []
    faces_to_remove = []
    directions = {"x":(1, 0, 0), "-x":(-1, 0, 0),
                   "y":(0, 1, 0), "-y": (0, -1, 0),
                  "z":(0, 0, 1), "-z":(0, 0, -1)}
    cube_directions = {}
    center_cubes = {}
    cubes_center = {}
    for i in range(n):
        vertices, faces = create_cube(start_center, size)
        all_vertices.append(vertices)
        all_faces.append(faces + (len(all_vertices)  - 1)*8)

        cubes_center[i] = list(start_center)
        center_cubes[tuple(start_center)] = i
        if i>0:
            for k_direction in list(directions.keys()):
                neighbour = start_center + np.array(directions[k_direction]) *size
                if tuple(neighbour) in center_cubes:
                    #we remove the face of cube i in direction k
                    faces_to_remove.extend(get_faces_to_remove(i,k_direction))
                    # then we need to remove the face of cube j in direction -k
                    opposite_direction = {"x": "-x", "-x": "x", "y": "-y", "-y": "y", "z": "-z", "-z": "z"}[k_direction]
                    faces_to_remove.extend(get_faces_to_remove(center_cubes[tuple(neighbour)],opposite_direction))
            
        
        if i< n-1:
            available_directions = list(directions.keys())
            if i in cube_directions:
                available_directions.remove({"x": "-x", "-x": "x", "y": "-y", "-y": "y", "z": "-z", "-z": "z"}[cube_directions[i]])
            direction_key = random.choices(available_directions,weights=[0.3,0.2,0,0,0.3,0.2],k=1)[0]
            # direction_key = random.choice(available_directions)
            cube_directions[i] = direction_key
            direction = directions[direction_key]

            start_center = np.array(start_center) + np.array(direction)*size

    combined_vertices = np.concatenate(all_vertices)
    combined_faces = np.concatenate(all_faces)
    print(cubes_center)
    combined_faces = np.delete(combined_faces,faces_to_remove,axis=0)
    return combined_vertices, combined_faces

def write_initial_stl(name,center,n):
    vertices, faces = create_random_cubes(start_center=center, size=15, n=n)


    cube = mesh.Mesh(np.zeros(faces.shape[0], dtype=mesh.Mesh.dtype))
    for i, f in enumerate(faces):
        for j in range(3):
            cube.vectors[i][j] = vertices[f[j]]

    # cube.save('Laberith.stl')
    cube.save(name)

def improve_stl(stl_filename, texture_image):
    reader = vtk.vtkSTLReader()
    reader.SetFileName(stl_filename)
    reader.Update()
    # compute normals
    normals = vtk.vtkPolyDataNormals()
    normals.SetInputConnection(reader.GetOutputPort())
    # normals.GetComputePointNormals()
    normals.ComputePointNormalsOff()
    normals.ComputeCellNormalsOn()
    normals.Update()
    inverted_normals = vtk.vtkPolyDataNormals()
    inverted_normals.SetInputConnection(reader.GetOutputPort())
    inverted_normals.ComputePointNormalsOff()
    inverted_normals.ComputeCellNormalsOn()
    inverted_normals.FlipNormalsOn()
    inverted_normals.Update()
    combined_normals = vtk.vtkAppendPolyData()
    combined_normals.AddInputData(normals.GetOutput())
    combined_normals.AddInputData(inverted_normals.GetOutput())
    combined_normals.Update()

    # surface.SetInputConnection( reader.GetOutputPort( ) )
    # surface.SetValue( 0, 0.0 )

    

    #texture
    texture_reader = vtk.vtkPNGReader()
    texture_reader.SetFileName(texture_image)
    texture_reader.Update()

    texture = vtk.vtkTexture()
    texture.SetInputConnection(texture_reader.GetOutputPort())
    texture_coordinates = vtk.vtkTextureMapToPlane()
    texture_coordinates.SetInputConnection(combined_normals.GetOutputPort())
    texture_coordinates.AutomaticPlaneGenerationOn()
    texture_coordinates.Update()

    writer = vtk.vtkSTLWriter()
    writer.SetFileName(stl_filename)

    writer.SetInputConnection(texture_coordinates.GetOutputPort())
    writer.Write()

name = sys.argv[1]
center = [int(i) for i in sys.argv[2].split(",") ]
n = int(sys.argv[3])  # Number of cubes
texture_file = '/Users/montajos/Dropbox/Computacion Grafica/Proyecto/src/LevelZero/resources/wall.png'
write_initial_stl(name,center,n)
# improve_stl(name, texture_file)
# print([int(i) for i in center.split(",")])