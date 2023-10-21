
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
LONG_LAB = 10
N_CAMINOS_CIEGOS = 5

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
    initial_direction = random_direction()
    return crea_camino_cubos(initial_direction, [V_CUBO_UNIT], 0, LONG_LAB)

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

def visualize_maze(VecCubos, VecCaminosCiegos):

    # Create a mapper
    mapper = vtk.vtkPolyDataMapper()

    # Append all cubes to a single polydata object
    appendFilter = vtk.vtkAppendPolyData()
    for cube_vertices in VecCubos:
        appendFilter.AddInputData(create_vtk_cube(cube_vertices))
    for cube_vertices in VecCaminosCiegos:
        appendFilter.AddInputData(create_vtk_cube(cube_vertices))
    mapper.SetInputConnection(appendFilter.GetOutputPort())


    # Create an actor for the main path
    main_path_actor = vtk.vtkActor()
    main_path_actor.SetMapper(mapper)
    main_path_actor.GetProperty().SetColor(1.0, 0.0, 0.0)  # Red color

    # Create a VTK renderer
    renderer = vtk.vtkRenderer()
    renderer.SetBackground(0.3, 0.3, 0.3)  # Background color

    # Create a VTK render window
    render_window = vtk.vtkRenderWindow()
    render_window.AddRenderer(renderer)
    render_window.SetWindowName("Laberith")

    # Create a VTK render window interactor
    render_window_interactor = vtk.vtkRenderWindowInteractor()
    render_window_interactor.SetRenderWindow(render_window)
    renderer.AddActor(main_path_actor)

    # Create an actor for the blind paths
    blind_path_actor = vtk.vtkActor()
    blind_path_actor.SetMapper(mapper)
    blind_path_actor.GetProperty().SetColor(0.0, 0.0, 1.0)  # Blue color
    renderer.AddActor(blind_path_actor)


    # Start the visualization
    render_window.Render()
    render_window_interactor.Start()
# Main Execution
VecCubos, MatInfo = create_main_path()
VecCaminosCiegos, MatInfoCiegos = create_blind_paths(np.array(MatInfo))
# plot_maze()

visualize_maze(VecCubos, VecCaminosCiegos)
