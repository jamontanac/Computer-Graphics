import vtk

def convert_and_visualize_stl(stl_filename, obj_filename):
    reader = vtk.vtkSTLReader()
    reader.SetFileName(stl_filename)

    writer = vtk.vtkOBJWriter()
    writer.SetFileName(obj_filename)
    writer.SetInputConnection(reader.GetOutputPort())
    writer.Write()

    mapper = vtk.vtkPolyDataMapper()
    mapper.SetInputConnection(reader.GetOutputPort())

    actor = vtk.vtkActor()
    actor.SetMapper(mapper)

    renderer = vtk.vtkRenderer()
    renderWindow = vtk.vtkRenderWindow()
    renderWindow.AddRenderer(renderer)

    renderWindowInteractor = vtk.vtkRenderWindowInteractor()
    renderWindowInteractor.SetRenderWindow(renderWindow)

    renderer.AddActor(actor)
    renderer.SetBackground(0.1, 0.2, 0.3)  

    renderWindow.Render()
    renderWindowInteractor.Start()

stl_file = 'Laberith.stl'  
obj_file = 'room.obj'        

convert_and_visualize_stl(stl_file, obj_file)

