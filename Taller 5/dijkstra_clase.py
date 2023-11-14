import sys, vtk
from DijkstraOnVTKImage import *

image = vtk.vtkPNGReader( )
image.SetFileName( sys.argv[ 1 ] )

viewer = vtk.vtkImageViewer2( )
viewer.SetInputConnection( image.GetOutputPort( ) )
## viewer.SetZSlice( 0 )
viewer.SetColorWindow( 256 )
viewer.SetColorLevel( 128 )
viewer.GetRenderWindow( ).SetSize( 512, 512 )
viewer.GetRenderer( ).SetBackground( 1, 0, 0 )
viewer.GetRenderWindow( ).SetWindowName( 'Dijkstra' )

# Create the RenderWindowInteractor.
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
viewer.Render( )
iren.Initialize( )
iren.Start( )

if wrep.GetNumberOfSeeds( ) > 1:
  s = [ 0, 0, 0 ]
  e = [ 0, 0, 0 ]
  wrep.GetSeedWorldPosition( 0, s )
  wrep.GetSeedWorldPosition( 1, e )
  print( s )
  print( e )
  
  dmap = vtk.vtkImageEuclideanDistance( )
  dmap.SetInputConnection( image.GetOutputPort( ) )
  dmap.Update( )
  
  path = DijkstraOnVTKImage( dmap.GetOutput( ), s, e )
  print( path.GetNumberOfPoints( ) )

  numberOfPoints=path.GetNumberOfPoints()

  xSpline=vtk.vtkKochanekSpline()
  xSpline.SetDefaultTension(0)
  xSpline.SetDefaultContinuity(0)
  xSpline.SetDefaultBias(0)
  ySpline=vtk.vtkKochanekSpline()
  ySpline.SetDefaultTension(0)
  ySpline.SetDefaultContinuity(0)
  ySpline.SetDefaultBias(0)
  zSpline=vtk.vtkKochanekSpline()
  zSpline.SetDefaultTension(0)
  zSpline.SetDefaultContinuity(0)
  zSpline.SetDefaultBias(0)
  spline = vtk.vtkParametricSpline()
  spline.SetXSpline(xSpline)
  spline.SetYSpline(ySpline)
  spline.SetZSpline(zSpline)
  spline.SetPoints(path.GetPoints() )

  functionSource= vtk.vtkParametricFunctionSource()
  functionSource.SetParametricFunction(spline)
  functionSource.SetUResolution(50 * numberOfPoints)
  functionSource.SetVResolution(50 * numberOfPoints)
  functionSource.SetWResolution(50 * numberOfPoints)
  functionSource.Update()

  path_mapper = vtk.vtkPolyDataMapper( )
  # path_mapper.SetInputConnection( functionSource.GetOutputPort() )
  # path_mapper.SetInputData( functionSource.GetOutputPort() )

  path_mapper.SetInputData(path)

  
  path_actor = vtk.vtkActor( )
  path_actor.SetMapper( path_mapper )
  path_actor.GetProperty( ).SetColor( 0, 0, 1 )
  path_actor.GetProperty( ).SetLineWidth( 3.0 )

  viewer = vtk.vtkImageViewer2( )
  viewer.SetInputConnection( image.GetOutputPort( ) )
  ##viewer.SetZSlice( 0 )
  viewer.SetColorWindow( 256 )
  viewer.SetColorLevel( 128 )
  viewer.GetRenderWindow( ).SetSize( 512, 512 )
  viewer.GetRenderer( ).SetBackground( 1, 0, 0 )
  viewer.GetRenderWindow( ).SetWindowName( 'Dijkstra' )

  # Create the RenderWindowInteractor.
  iren = vtk.vtkRenderWindowInteractor( )
  viewer.SetupInteractor( iren )
  
  viewer.GetRenderer( ).AddActor( path_actor )

  viewer.Render( )
  iren.Initialize( )
  iren.Start( )
# end if

## eof 
