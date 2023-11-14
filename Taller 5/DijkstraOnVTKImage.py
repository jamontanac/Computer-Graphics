## =========================================================================
## @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
## =========================================================================

import heapq, math, vtk

def DijkstraOnVTKImage( image, s, e ):

  # Get image-as-graph properties
  W, H, D = image.GetDimensions( )
  T = [ [  None for j in range( H ) ] for i in range( W ) ]
  M = [ [ False for j in range( H ) ] for i in range( W ) ]
  si = [ 0, 0, 0 ]
  ei = [ 0, 0, 0 ]
  image.TransformPhysicalPointToContinuousIndex( [ s[ 0 ], s[ 1 ], 0 ], si )
  image.TransformPhysicalPointToContinuousIndex( [ e[ 0 ], e[ 1 ], 0 ], ei )

  # Compute spanning tree from s
  q = [ ( 0, [ int( si[ 0 ] ), int( si[ 1 ] ) ], None ) ]
  while len( q ) > 0:
      n = heapq.heappop( q )
      i = n[ 1 ]
      v = image.GetScalarComponentAsDouble( i[ 0 ], i[ 1 ], 0, 0 )
      if M[ i[ 0 ] ][ i[ 1 ] ] == False:
        M[ i[ 0 ] ][ i[ 1 ] ] = True
        T[ i[ 0 ] ][ i[ 1 ] ] = n[ 2 ]
        for d in range( 2 ):
          for o in range( -1, 2, 2 ):
            ni = [ i[ 0 ], i[ 1 ] ]
            ni[ d ] += o
            if \
                   0 <= ni[ 0 ] and ni[ 0 ] < W \
                   and \
                   0 <= ni[ 1 ] and ni[ 1 ] < H:
              w = image.GetScalarComponentAsDouble( ni[ 0 ], ni[ 1 ], 0, 0 )
              w -= v
              # w = 1.0 - math.exp( -( w * w ) )
              w = math.exp( -( w * w ) )
              heapq.heappush( q, ( n[ 0 ] + w, ni, i ) )
            # end if
          # end for
        # end for
      # end if
  # end while

  # Backtracking
  points = vtk.vtkPoints( )
  i = [ int( ei[ 0 ] ), int( ei[ 1 ] ) ]
  while not T[ i[ 0 ] ][ i[ 1 ] ] is None:
    xyz = [ 0, 0, 0 ]
    image.TransformIndexToPhysicalPoint( [ i[ 0 ], i[ 1 ], 0 ], xyz )
    xyz[2]=47
    points.InsertNextPoint( xyz )
    i = T[ i[ 0 ] ][ i[ 1 ] ]
  # end while

  line = vtk.vtkCellArray( )
  line.InsertNextCell( points.GetNumberOfPoints( ) )
  for i in range( points.GetNumberOfPoints( ) ):
    line.InsertCellPoint( i )
  # end for

  pdata = vtk.vtkPolyData( )
  pdata.SetPoints( points )
  pdata.SetLines( line )

  return pdata
# end def

## -------------------------------------------------------------------------
if __name__ == "__main__":
  import sys

  src = vtk.vtkImageNoiseSource( )
  src.SetWholeExtent( 0, 150, 0, 100, 0, 0 )
  src.SetMinimum( 0.0 )
  src.SetMaximum( 255.0 )
  src.Update( )

  path = DijkstraOnVTKImage( src.GetOutput( ), ( 10, 10 ), ( 123, 98 ) )
# end if

## eof - DijkstraOnVTKImage.py
