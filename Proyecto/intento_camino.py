import vtkmodules.all as vtk

# Create a VTK renderer
renderer = vtk.vtkRenderer()
renderer.SetBackground(0.1, 0.1, 0.1)  # Background color

# Create a VTK render window
render_window = vtk.vtkRenderWindow()
render_window.AddRenderer(renderer)

# Create a VTK render window interactor
render_window_interactor = vtk.vtkRenderWindowInteractor()
render_window_interactor.SetRenderWindow(render_window)

# Define the VTK polydata for cubes
cube_polydata = vtk.vtkCubeSource()
cube_mapper = vtk.vtkPolyDataMapper()
cube_mapper.SetInputConnection(cube_polydata.GetOutputPort())

# Define cube positions for the main path
main_path_cubes = [
    [0, 0, 0],
    [1, 0, 0],
    [2, 0, 0],
    [3, 0, 0],
    # Add more cube positions for the main path here
]

# Define cube positions for a blind path
blind_path_cubes = [
    [0, 0, 0],
    [0, 0, 1],
    [0, 0, 2],
    [0, 0, 3],
    # Add more cube positions for a blind path here
]

# Create actors for cubes in the main path
main_path_actors = []
for cube_position in main_path_cubes:
    cube_actor = vtk.vtkActor()
    cube_actor.SetMapper(cube_mapper)
    cube_actor.SetPosition(cube_position)
    cube_actor.GetProperty().SetColor(1.0, 0.0, 0.0)  # Red color
    renderer.AddActor(cube_actor)
    main_path_actors.append(cube_actor)

# Create actors for cubes in a blind path
blind_path_actors = []
for cube_position in blind_path_cubes:
    path_actor = vtk.vtkActor()
    path_actor.SetMapper(cube_mapper)
    path_actor.SetPosition(cube_position)
    path_actor.GetProperty().SetColor(0.0, 0.0, 1.0)  # Blue color
    renderer.AddActor(path_actor)
    blind_path_actors.append(path_actor)

# Set up the VTK render window
render_window.SetWindowName("3D Labyrinth")
render_window.Render()
render_window_interactor.Start()


