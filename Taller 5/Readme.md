## Manual for the Dijkstra's Pathfinding Visualization Script

This script provides a visualization tool for the Dijkstra's shortest path algorithm applied to images. It uses the Visualization Toolkit (VTK) to display the image and compute the path.

### Requirements:

- VTK library (Visualization Toolkit) and its Python bindings
- `DijkstraOnVTKImage.py` module

### Script Overview:

1. **Setup & Configuration:**
    - Load an image (in PNG format) specified by the user.
    - Configure and display the loaded image using `vtkImageViewer2`.

2. **User Interaction:**
    - Allows the user to interactively set two points (start and end) on the displayed image using a seeding widget.

3. **Pathfinding:**
    - Once the two points are set, the script computes the shortest path between them using the Dijkstra's algorithm.

4. **Visualization:**
    - Visualizes the computed path on the image.

### Detailed Breakdown:

1. **Image Loading:**

    ```python
    image = vtk.vtkPNGReader( )
    image.SetFileName( sys.argv[ 1 ] )
    ```

    The script expects the path to a PNG image as the first command-line argument and loads it using `vtkPNGReader`.

2. **Image Display Configuration:**

    ```python
    viewer = vtk.vtkImageViewer2( )
    viewer.SetInputConnection( image.GetOutputPort( ) )
    viewer.SetColorWindow( 256 )
    viewer.SetColorLevel( 128 )
    viewer.GetRenderWindow( ).SetSize( 512, 512 )
    viewer.GetRenderer( ).SetBackground( 1, 0, 0 )
    viewer.GetRenderWindow( ).SetWindowName( 'Dijkstra' )
    ```

    The image is set to be displayed with a specific color window, level, size, and window name. The background color of the renderer is set to red.

3. **User Interaction Setup:**

    ```python
    iren = vtk.vtkRenderWindowInteractor( )
    viewer.SetupInteractor( iren )
    hrep = vtk.vtkPointHandleRepresentation2D( )
    hrep.GetProperty( ).SetColor( 0, 1, 0 )
    wrep = vtk.vtkSeedRepresentation( )
    wrep.SetHandleRepresentation( hrep )
    widget = vtk.vtkSeedWidget( )
    widget.SetInteractor( iren )
    widget.SetRepresentation( wrep )
    widget.On( )
    ```

    This sets up the interactive seeding widget, allowing the user to place two points on the image.

4. **Visualization Start:**

    ```python
    viewer.Render( )
    iren.Initialize( )
    iren.Start( )
    ```

    This initiates the viewer, rendering the image and starts the interactor, waiting for the user to set the seeds.

5. **Pathfinding & Visualization:**

    - After the two seeds (points) are set, the Dijkstra's algorithm computes the shortest path between them.
    - The script then creates a parametric spline representation of the path.
    - Finally, the path is displayed over the image.

### Usage:

1. **Command Line Execution:**

    ```
    python dijkstra_clase.py path_to_image.png
    ```

    Replace `dijkstra_clase.py` with the script's filename and `path_to_image.png` with the path to your target PNG image.

2. **Interactivity:**

    - Once the image is displayed, place two points (start and end) on it.
    - The script will compute and display the shortest path between the two points using the Dijkstra's algorithm.
