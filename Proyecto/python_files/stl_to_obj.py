import vtk
import sys
def convert_stl_to_obj(stl_filename, obj_filename, texture_image):
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

    # viz

    # mapper = vtk.vtkPolyDataMapper()
    # mapper.SetInputConnection(texture_coordinates.GetOutputPort())

    # actor = vtk.vtkActor()
    # actor.SetMapper(mapper)
    # actor.SetTexture(texture)

    # render = vtk.vtkRenderer()
    # renderWindow = vtk.vtkRenderWindow()
    # renderWindow.AddRenderer(render)
    # renderWindowInteractor = vtk.vtkRenderWindowInteractor()
    # renderWindowInteractor.SetRenderWindow(renderWindow)
    # render.AddActor(actor)
    # render.SetBackground(0.3,0.3,0.3)
    # renderWindowInteractor.Start()



    writer = vtk.vtkOBJWriter()
    writer.SetFileName(obj_filename)

    writer.SetInputConnection(texture_coordinates.GetOutputPort())
    writer.Write()


# stl_file = 'Laberith.stl'  
# obj_file = 'room.obj'
texture_file = '../wall.png'        
stl_file = sys.argv[1]
obj_file = sys.argv[2]
convert_stl_to_obj(stl_file, obj_file,texture_file)

