import vtk

def convert_stl_to_obj(stl_filename, obj_filename):
    # Read the STL file
    reader = vtk.vtkSTLReader()
    reader.SetFileName(stl_filename)
    
    # Write to an OBJ file
    writer = vtk.vtkOBJWriter()
    writer.SetFileName(obj_filename)
    writer.SetInputConnection(reader.GetOutputPort())
    
    # Perform the conversion
    writer.Write()

# Example usage
stl_file = 'Laberith.stl'  # Replace with your STL file path
obj_file = 'room.obj'        # Desired output OBJ file path

convert_stl_to_obj(stl_file, obj_file)
