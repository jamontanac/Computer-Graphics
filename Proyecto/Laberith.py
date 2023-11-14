
import vtk
import matplotlib.pyplot as plt
import numpy as np
from mpl_toolkits.mplot3d import Axes3D
from mpl_toolkits.mplot3d.art3d import Poly3DCollection
import random

import vtkmodules.all as vtk
# Constants and Parameters
DICT_CARA_I = {-3:1, 1:2, 3:3, -1:4, 4:5, -4:6}
DICT_CARA_F = {-3:3, 1:4, 3:1, -1:2, 4:6, -4:5}
DICT_VERT_CARA = {1:-3, 2:1, 3:3, 4:-1, 5:4, 6:-4}
V_CUBO_UNIT = np.array([
    [0, 0, 0], [1, 0, 0], [1, 1, 0], [0, 1, 0],
    [0, 0, 1], [1, 0, 1], [1, 1, 1], [0, 1, 1]
])
CARAS = np.array([
    [0, 1, 5, 4], [1, 2, 6, 5], [2, 3, 7, 6],
    [0, 3, 7, 4], [4, 5, 6, 7], [0, 1, 2, 3]
])
LONG_LAB = 50
N_CAMINOS_CIEGOS = 20

def cara_aleatoria(Vec, thresholds=[0.05,0.1,0.15,0.2]):
    r = random.uniform(0, 1)
    for i, threshold in enumerate(thresholds):
        if r < threshold:
            return Vec[i+1]
    return Vec[0]

def random_direction():
    return random.choice([-4, -3, -1, 1, 3, 4])

def crea_camino_cubos(initial_direction, VecCubos, num_cubo, NCubos, is_blind_path=False):
    v_cubo_unit = np.array([
        [0, 0, 0], [1, 0, 0], [1, 1, 0], [0, 1, 0],
        [0, 0, 1], [1, 0, 1], [1, 1, 1], [0, 1, 1]
    ])

    direction_to_vec = {
        1: [1, 3, 4, -3, -4],
        3: [3, 1, 4, -1, -4],
        4: [4, 3, 1, -3, -1],
        -1: [-1, 3, 4, -3, -4],
        -3: [-3, 1, 4, -1, -4],
        -4: [-4, 3, 1, -3, -1]
    }

    vertice_shift = {
        1: lambda x: x[-1][1],
        -1: lambda x: x[-1][0] + [-1, 0, 0],
        -3: lambda x: x[-1][0] + [0, -1, 0],
        -4: lambda x: x[-1][0] + [0, 0, -1]
    }

    MatInfo = []

    for _ in range(NCubos):
        num_cubo += 1
        Vec = direction_to_vec[initial_direction]
        initial_direction = cara_aleatoria(Vec)
        nuevo_vertice = vertice_shift.get(initial_direction, vertice_shift[1])(VecCubos)
        MatInfo.append([num_cubo - 1, DICT_CARA_I[initial_direction], num_cubo, DICT_CARA_F[initial_direction]])
        VecCubos.append(v_cubo_unit + nuevo_vertice)

    return VecCubos, MatInfo

def create_main_path():
    # initial_direction = random_direction()
    # return crea_camino_cubos(initial_direction, [V_CUBO_UNIT], 0, LONG_LAB)
    return crea_camino_cubos(3, [V_CUBO_UNIT], 0, LONG_LAB)

def create_blind_paths(M):
    v_aleat = np.random.randint(0, LONG_LAB, size=N_CAMINOS_CIEGOS)
    VecCaminosCiegos, MatInfoCiegos = [], []

    for val in v_aleat:
        caraf_entries = M[M[:, 0] == val, 1]
        carai_entries = M[M[:, 2] == val, 3]
        
        if caraf_entries.size == 0 or carai_entries.size == 0:
            continue

        caraf, carai = caraf_entries[0], carai_entries[0]
        CaraSalida = DICT_VERT_CARA[cara_aleatoria([x for x in range(1, 7) if x not in [caraf, carai]], [0.25, 0.5, 0.75])]
        VecCaminoCiegoi, MatInfo2 = crea_camino_cubos(CaraSalida, [VecCubos[val]], len(VecCubos) + len(VecCaminosCiegos), 20, True)
        VecCaminosCiegos.extend(VecCaminoCiegoi)
        MatInfoCiegos.extend(MatInfo2)

    return VecCaminosCiegos, MatInfoCiegos
def compute_axis_limits(vertices_list):
    all_vertices = np.vstack(vertices_list)
    x_min, y_min, z_min = np.min(all_vertices, axis=0)
    x_max, y_max, z_max = np.max(all_vertices, axis=0)
    
    padding = 5
    x_min -= padding
    y_min -= padding
    z_min -= padding
    x_max += padding
    y_max += padding
    z_max += padding

    return x_min, x_max, y_min, y_max, z_min, z_max
def plot_maze():
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    
    for cube_vertices in VecCaminosCiegos:   
        cube = Poly3DCollection(cube_vertices[CARAS], edgecolor='k', facecolor='blue')
        ax.add_collection3d(cube)    
    for cube_vertices in VecCubos:   
        cube = Poly3DCollection(cube_vertices[CARAS], edgecolor='k', facecolor='red')
        ax.add_collection3d(cube)

    
    x_min, x_max, y_min, y_max, z_min, z_max = compute_axis_limits(VecCubos + VecCaminosCiegos)
    ax.set_xlim([x_min, x_max])
    ax.set_ylim([y_min, y_max])
    ax.set_zlim([z_min, z_max])

    plt.show()
def create_vtk_cube(cube_vertices):
    # Create a cube using vtkPolyData
    cube = vtk.vtkPolyData()

    # Set points for the cube
    points = vtk.vtkPoints()
    for vertex in cube_vertices:
        points.InsertNextPoint(vertex)
    cube.SetPoints(points)

    # Define the cube's faces
    faces = [
        [0, 1, 5, 4],
        [1, 2, 6, 5],
        [2, 3, 7, 6],
        [0, 3, 7, 4],
        [4, 5, 6, 7],
        [0, 1, 2, 3]
    ]

    # Add faces to the cube
    cells = vtk.vtkCellArray()
    for face in faces:
        cells.InsertNextCell(len(face), face)
    cube.SetPolys(cells)

    return cube

def keypress_callback(obj, event, renderer, render_window):  # Add render_window as an argument
    key = obj.GetKeySym()
    camera = renderer.GetActiveCamera()

    move_speed = 0.5  # Adjust this value as needed
    if key == "Up":
        # camera.Pitch(10)

        position = camera.GetPosition()
        focal_point = camera.GetFocalPoint()
        new_position = [position[0] + (focal_point[0] - position[0]) * move_speed,
                        position[1] + (focal_point[1] - position[1]) * move_speed,
                        position[2] + (focal_point[2] - position[2]) * move_speed]
        camera.SetPosition(new_position)
    elif key == "Down":
        camera.Pitch(-10)
    elif key == "Left":
        camera.Yaw(10)
    elif key == "Right":
        camera.Yaw(-10)
    elif key == "k":
        camera.Dolly(1.1)
    elif key == "j":
        camera.Dolly(0.9)

    renderer.ResetCameraClippingRange()
    render_window.Render()


# def keypress_callback(obj, event, renderer, render_window):
#     key = obj.GetKeySym()
#     camera = renderer.GetActiveCamera()
#     move_speed = 0.5  # Adjust this value as needed

#     if key == "Up":
#         # Move forward along the camera's view direction
#         position = camera.GetPosition()
#         focal_point = camera.GetFocalPoint()
#         new_position = [position[0] + (focal_point[0] - position[0]) * move_speed,
#                         position[1] + (focal_point[1] - position[1]) * move_speed,
#                         position[2] + (focal_point[2] - position[2]) * move_speed]
        
#         # Check for collision (basic)
#         if not is_inside_wall(new_position):
#             camera.SetPosition(new_position)
#     elif key == "Down":
#         # Move backward
#         position = camera.GetPosition()
#         focal_point = camera.GetFocalPoint()
#         new_position = [position[0] - (focal_point[0] - position[0]) * move_speed,
#                         position[1] - (focal_point[1] - position[1]) * move_speed,
#                         position[2] - (focal_point[2] - position[2]) * move_speed]
        
#         # Check for collision (basic)
#         if not is_inside_wall(new_position):
#             camera.SetPosition(new_position)
#     # ... [Handle other keys]

#     renderer.ResetCameraClippingRange()
#     render_window.Render()

# def is_inside_wall(position):
#     # Basic collision detection
#     # Check if the given position is inside any of the maze walls
#     # This is a placeholder function; you'll need to implement the actual collision detection
#     return False

def set_initial_camera_view(renderer, start_position):
    # Get the active camera
    camera = renderer.GetActiveCamera()
    
    # Set the camera's position just above the start of the maze
    camera.SetPosition(start_position[0], start_position[1], start_position[2] + 5)  # +5 to be above the maze
    
    # Set the focal point to the start of the maze
    camera.SetFocalPoint(start_position)
    
    # Set the up direction to be the Z-axis
    camera.SetViewUp(0, 0, 1)
    
    # Adjust the view angle for a wider perspective if needed
    # camera.SetViewAngle(45)
    
    # Update the renderer's viewport
    renderer.ResetCamera()

def visualize_maze(VecCubos, VecCaminosCiegos):
    # Create a VTK renderer
    renderer = vtk.vtkRenderer()
    renderer.SetBackground(0.1, 0.1, 0.1)  # Background color
    # Create a VTK render window
    render_window = vtk.vtkRenderWindow()
    render_window.AddRenderer(renderer)

    render_window_interactor = vtk.vtkRenderWindowInteractor()
    render_window_interactor.SetRenderWindow(render_window)
    # render_window_interactor.AddObserver("KeyPressEvent", keypress_callback)
    # render_window_interactor.AddObserver("KeyPressEvent", lambda obj, event: keypress_callback(obj, event, renderer)) 
    render_window_interactor.AddObserver("KeyPressEvent", lambda obj, event: keypress_callback(obj, event, renderer, render_window)) 
    xor_function = vtk.vtkImplicitBoolean()
    # xor_function.SetOperationTypeToXor()

    for cube_vertices in VecCaminosCiegos:
        xor_function.AddFunction(create_vtk_cube(cube_vertices))
    for cube_vertices in VecCubos:
        xor_function.AddFunction(create_vtk_cube(cube_vertices))

    xor_mapper = vtk.vtkPolyDataMapper()
    xor_mapper.SetInputConnection(xor_function.GetOutputPort())

    xor_actor = vtk.vtkActor()
    xor_actor.SetMapper(xor_mapper)
    xor_actor.SetProperty().SetColor(1.0,1.0,0)
    
    renderer.AddActor(xor_actor)
    # # Create a mapper for the blind paths
    # blind_path_mapper = vtk.vtkPolyDataMapper()
    # appendFilterBlind = vtk.vtkAppendPolyData()

    # filter = vtk.vtkBooleanOperationPolyDataFilter()
    # filter.SetOperationToDifference()
    # for cube_vertices in VecCaminosCiegos:
    #     xor_function.AddFunction(create_vtk_cube(cube_vertices))
    # filter.SetInputData(0, appendFilterBlind.GetOutput())
    # # filter.SetInputData()
    # main_path_mapper.SetInputConnection(filter.GetOutputPort())
    # blind_path_mapper.SetInputConnection(appendFilterBlind.GetOutputPort())

    # # Create an actor for the blind paths
    # blind_path_actor = vtk.vtkActor()
    # blind_path_actor.SetMapper(blind_path_mapper)
    # blind_path_actor.GetProperty().SetColor(0.0, 0.0, 1.0)  # Blue color
    # renderer.AddActor(blind_path_actor)

    # # Create a mapper for the main path
    # main_path_mapper = vtk.vtkPolyDataMapper()
    # appendFilterMain = vtk.vtkAppendPolyData()
    # filter = vtk.vtkBooleanOperationPolyDataFilter()
    # filter.SetOperationToDifference()
    # for cube_vertices in VecCubos:
    #     appendFilterMain.AddInputData(create_vtk_cube(cube_vertices))

    # filter.SetInputData(appendFilterMain.GetOutput())
    # main_path_mapper.SetInputConnection(filter.GetOutputPort())

    # # Create an actor for the main path
    # main_path_actor = vtk.vtkActor()
    # main_path_actor.SetMapper(main_path_mapper)
    # main_path_actor.GetProperty().SetColor(1.0, 0.0, 0.0)  # Red color
    # renderer.AddActor(main_path_actor)
    # # Set the initial camera view
    # # start_position = VecCubos[0][0]  # Assuming the first cube's first vertex is the start
    # # set_initial_camera_view(renderer, start_position)
    # # Start the visualization

    # renderer.GetActiveCamera().Azimuth(30)
    # renderer.GetActiveCamera().Elevation(30)
    # renderer.GetActiveCamera().Zoom(1.0)
    render_window.Render()
    render_window_interactor.Start()

# Main Execution
VecCubos, MatInfo = create_main_path()
VecCaminosCiegos, MatInfoCiegos = create_blind_paths(np.array(MatInfo))
# plot_maze()

visualize_maze(VecCubos, VecCaminosCiegos)
