import vtk

def convert_stl_to_obj(stl_filename, obj_filename, texture_image):
    reader = vtk.vtkSTLReader()
    reader.SetFileName(stl_filename)
    # compute normals
    normals = vtk.vtkPolyDataNormals()
    normals.SetInputConnection(reader.GetOutputPort())
    normals.Update()
    #texture
    texture_reader = vtk.vtkPNGReader()
    texture_reader.SetFileName(texture_image)
    texture_reader.Update()

    texture = vtk.vtkTexture()
    texture.SetInputConnection(texture_reader.GetOutputPort())
    texture_coordinates = vtk.vtkTextureMapToPlane()
    texture_coordinates.SetInputConnection(normals.GetOutputPort())
    texture_coordinates.AutomaticPlaneGenerationOn()
    texture_coordinates.Update()

    mapper = vtk.vtkPolyDataMapper()
    mapper.SetInputConnection(texture_coordinates.GetOutputPort())

    actor = vtk.vtkActor()
    actor.SetMapper(mapper)
    actor.SetTexture(texture)

    render = vtk.vtkRenderer()
    renderWindow = vtk.vtkRenderWindow()
    renderWindow.AddRenderer(render)
    renderWindowInteractor = vtk.vtkRenderWindowInteractor()
    renderWindowInteractor.SetRenderWindow(renderWindow)
    render.AddActor(actor)
    render.SetBackground(0.9,0,0)
    renderWindow.Render()
    renderWindowInteractor.Start()

    # renderWindow.SetRender(renderWindow)


    writer = vtk.vtkOBJWriter()
    writer.SetFileName(obj_filename)

    writer.SetInputConnection(texture_coordinates.GetOutputPort())
    writer.Write()


stl_file = 'Laberith.stl'  
obj_file = 'room.obj'
texture_file = 'wall.png'        

convert_stl_to_obj(stl_file, obj_file,texture_file)

