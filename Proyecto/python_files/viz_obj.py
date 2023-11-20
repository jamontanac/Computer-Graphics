
import vtk
import sys

name_of_file = str(sys.argv[1])


# Create a reader for STL files
stlReader = vtk.vtkOBJReader()
stlReader.SetFileName(name_of_file)

# Create a mapper for the STL data
mapper = vtk.vtkPolyDataMapper()
mapper.SetInputConnection(stlReader.GetOutputPort())

# Create an actor to represent the STL object
actor = vtk.vtkActor()
actor.SetMapper(mapper)

# Create a renderer and add the STL actor to it
renderer = vtk.vtkRenderer()
renderer.AddActor(actor)
renderer.SetBackground(0.1, 0.2, 0.4)  # Set a nice background color

# Create a render window
renderWindow = vtk.vtkRenderWindow()
renderWindow.AddRenderer(renderer)
renderWindow.SetSize(800, 600)  # Set the window size

# Create a render window interactor to handle user input
renderWindowInteractor = vtk.vtkRenderWindowInteractor()
renderWindowInteractor.SetRenderWindow(renderWindow)

# Add the custom interaction style (if necessary)
style = vtk.vtkInteractorStyleTrackballCamera()
renderWindowInteractor.SetInteractorStyle(style)

# Initialize the interactor and start the rendering loop
renderWindowInteractor.Initialize()
renderWindow.Render()
renderWindowInteractor.Start()
