import vtk
import random

def create_cube(x, y, z):
    """
    Create a cube at the specified coordinates.
    """
    cube = vtk.vtkCubeSource()
    cube.SetXLength(1)
    cube.SetYLength(1)
    cube.SetZLength(1)
    cube.SetCenter(x, y, z)
    cube.Update()
    # Clean the polydata
    cleanFilter = vtk.vtkCleanPolyData()
    cleanFilter.SetInputConnection(cube.GetOutputPort())
    cleanFilter.Update()

    return cleanFilter.GetOutput()
    # return cube.GetOutput()

def attach_new_cube(last_cube_center):
    direction = random.choice(['up', 'down', 'left', 'right', 'front', 'back'])
    x, y, z = last_cube_center
    # Adjust the displacement to ensure overlap
    displacement = 0.5  # Half the length of the cube
    if direction == 'up':
        y += displacement
    elif direction == 'down':
        y -= displacement
    elif direction == 'left':
        x -= displacement
    elif direction == 'right':
        x += displacement
    elif direction == 'front':
        z += displacement
    elif direction == 'back':
        z -= displacement
    return create_cube(x, y, z), (x, y, z)
def perform_union(polydata1, polydata2):
    """
    Perform a boolean union operation between two polydata.
    """
    booleanOperation = vtk.vtkBooleanOperationPolyDataFilter()
    booleanOperation.SetOperationToUnion()
    booleanOperation.SetInputData(0, polydata1)
    booleanOperation.SetInputData(1, polydata2)
    booleanOperation.Update()
    return booleanOperation.GetOutput()

# Parameters
num_cubes = 50  # Number of cubes in the labyrinth

# Initial cube
current_structure = create_cube(0, 0, 0)
cube_centers = [(0, 0, 0)]

# Construct the labyrinth
for _ in range(1, num_cubes):
    new_cube, new_center = attach_new_cube(cube_centers[-1])
    current_structure = perform_union(current_structure, new_cube)
    cube_centers.append(new_center)

# Set up the rendering pipeline
renderer = vtk.vtkRenderer()
renderWindow = vtk.vtkRenderWindow()
renderWindow.AddRenderer(renderer)
renderWindowInteractor = vtk.vtkRenderWindowInteractor()
renderWindowInteractor.SetRenderWindow(renderWindow)

# Create a mapper and actor
mapper = vtk.vtkPolyDataMapper()
mapper.SetInputData(current_structure)

actor = vtk.vtkActor()
actor.SetMapper(mapper)

renderer.AddActor(actor)

# Export to OBJ file
exporter = vtk.vtkOBJExporter()
exporter.SetRenderWindow(renderWindow)
exporter.SetFilePrefix("labyrinth")
exporter.Write()

# Start the interaction
renderWindow.Render()
renderWindowInteractor.Start()