
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <chrono>

#ifdef _MSC_VER
#  include <boost/config/compiler/visualc.hpp>
#endif
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/optional/optional.hpp>

#include <OpenGL/gl.h> // Use this when using mac
#include <GLUT/glut.h> // Use this when using mac

// #include <GL/gl.h> //use this when in linux
// #include <GL/glut.h> //use this when in linux
// -------------------------------------------------------------------------
int main_window = 0;

float world_P0[ 2 ];
float world_P1[ 2 ];

boost::property_tree::ptree solar_system;

std::chrono::high_resolution_clock::time_point t0;

double time_scale = 1.0;

int ViewPort[ 4 ];
float InvPrj[ 16 ];

// -------------------------------------------------------------------------
void circle( int mode = GL_LINE_LOOP, unsigned int samples = 30 )
{
  double off = 8.0 * std::atan( 1 ) / double( samples );

  glBegin( mode );
  for( unsigned int i = 0; i < samples; ++i )
  {
    double w = double( i ) * off;
    glVertex2f( std::cos( w ), std::sin( w ) );
  } // end for
  glEnd( );
}

// -------------------------------------------------------------------------
void draw_hierarchy( const boost::property_tree::ptree& tree )
{
  using _T = boost::property_tree::ptree;
  using _O = boost::optional< const _T& >;
  using _It = _T::const_iterator;

  for( _It i = tree.begin( ); i != tree.end( ); ++i )
  {
    // Check if this node is a celestial object
    _O size_node = i->second.get_child_optional( "size" );
    if( size_node )
    {
      _O orbit_node = i->second.get_child_optional( "orbit" );
      _O color_node = i->second.get_child_optional( "color" );
      float r = size_node.get( ).get< float >( "" );
      float o = 0;
      float cr = 1, cg = 1, cb = 1;
      if( orbit_node )
        o = orbit_node.get( ).get< float >( "" );
      if( color_node )
      {
        std::istringstream ci( color_node.get( ).get< std::string >( "" ) );
        ci >> cr >> cg >> cb;
      } // end if

      // Save parent matrix and localization transformation
      glPushMatrix( );
      glTranslatef( o, 0, 0 );
      //Aca se hace el rotado
      // como se carga la matriz de rotacion

      // Internal transformation and draw
      glPushMatrix( );
      glColor3f( cr, cg, cb );
      glScalef( r, r, 1 );
      circle( GL_POLYGON, 36 );
      glPopMatrix( );

      // Draw children
      draw_hierarchy( i->second );

      // Get parent matrix back
      glPopMatrix( );
    }
    else // Just go deeper into hierarchy
      draw_hierarchy( i->second );
  } // end for
}

// -------------------------------------------------------------------------
bool matrix4x4_inv( float m[ 16 ], float invOut[ 16 ] )
{
  float inv[ 16 ], det;
  int i;

  inv[ 0 ] =  m[ 5 ] * m[ 10 ] * m[ 15 ] - m[ 5 ] * m[ 14 ] * m[ 11 ] - m[ 6 ] * m[ 9 ] * m[ 15 ] + m[ 6 ] * m[ 13 ] * m[ 11 ] + m[ 7 ] * m[ 9 ] * m[ 14 ] - m[ 7 ] * m[ 13 ] * m[ 10 ];
  inv[ 1 ] = -m[ 1 ] * m[ 10 ] * m[ 15 ] + m[ 1 ] * m[ 14 ] * m[ 11 ] + m[ 2 ] * m[ 9 ] * m[ 15 ] - m[ 2 ] * m[ 13 ] * m[ 11 ] - m[ 3 ] * m[ 9 ] * m[ 14 ] + m[ 3 ] * m[ 13 ] * m[ 10 ];
  inv[ 2 ] =  m[ 1 ] * m[ 6 ] * m[ 15 ] - m[ 1 ] * m[ 14 ] * m[ 7 ] - m[ 2 ] * m[ 5 ] * m[ 15 ] + m[ 2 ] * m[ 13 ] * m[ 7 ] + m[ 3 ] * m[ 5 ] * m[ 14 ] - m[ 3 ] * m[ 13 ] * m[ 6 ];
  inv[ 3 ] = -m[ 1 ] * m[ 6 ] * m[ 11 ] + m[ 1 ] * m[ 10 ] * m[ 7 ] + m[ 2 ] * m[ 5 ] * m[ 11 ] - m[ 2 ] * m[ 9 ] * m[ 7 ] - m[ 3 ] * m[ 5 ] * m[ 10 ] + m[ 3 ] * m[ 9 ] * m[ 6 ];
  inv[ 4 ] = -m[ 4 ] * m[ 10 ] * m[ 15 ] + m[ 4 ] * m[ 14 ] * m[ 11 ] + m[ 6 ] * m[ 8 ] * m[ 15 ] - m[ 6 ] * m[ 12 ] * m[ 11 ] - m[ 7 ] * m[ 8 ] * m[ 14 ] + m[ 7 ] * m[ 12 ] * m[ 10 ];
  inv[ 5 ] =  m[ 0 ] * m[ 10 ] * m[ 15 ] - m[ 0 ] * m[ 14 ] * m[ 11 ] - m[ 2 ] * m[ 8 ] * m[ 15 ] + m[ 2 ] * m[ 12 ] * m[ 11 ] + m[ 3 ] * m[ 8 ] * m[ 14 ] - m[ 3 ] * m[ 12 ] * m[ 10 ];
  inv[ 6 ] = -m[ 0 ] * m[ 6 ] * m[ 15 ] + m[ 0 ] * m[ 14 ] * m[ 7 ] + m[ 2 ] * m[ 4 ] * m[ 15 ] - m[ 2 ] * m[ 12 ] * m[ 7 ] - m[ 3 ] * m[ 4 ] * m[ 14 ] + m[ 3 ] * m[ 12 ] * m[ 6 ];
  inv[ 7 ] =  m[ 0 ] * m[ 6 ] * m[ 11 ] - m[ 0 ] * m[ 10 ] * m[ 7 ] - m[ 2 ] * m[ 4 ] * m[ 11 ] + m[ 2 ] * m[ 8 ] * m[ 7 ] + m[ 3 ] * m[ 4 ] * m[ 10 ] - m[ 3 ] * m[ 8 ] * m[ 6 ];
  inv[ 8 ] =  m[ 4 ] * m[ 9 ] * m[ 15 ] - m[ 4 ] * m[ 13 ] * m[ 11 ] - m[ 5 ] * m[ 8 ] * m[ 15 ] + m[ 5 ] * m[ 12 ] * m[ 11 ] + m[ 7 ] * m[ 8 ] * m[ 13 ] - m[ 7 ] * m[ 12 ] * m[ 9 ];
  inv[ 9 ] = -m[ 0 ] * m[ 9 ] * m[ 15 ] + m[ 0 ] * m[ 13 ] * m[ 11 ] + m[ 1 ] * m[ 8 ] * m[ 15 ] - m[ 1 ] * m[ 12 ] * m[ 11 ] - m[ 3 ] * m[ 8 ] * m[ 13 ] + m[ 3 ] * m[ 12 ] * m[ 9 ];
  inv[ 10 ] =  m[ 0 ] * m[ 5 ] * m[ 15 ] - m[ 0 ] * m[ 13 ] * m[ 7 ] - m[ 1 ] * m[ 4 ] * m[ 15 ] + m[ 1 ] * m[ 12 ] * m[ 7 ] + m[ 3 ] * m[ 4 ] * m[ 13 ] - m[ 3 ] * m[ 12 ] * m[ 5 ];
  inv[ 11 ] = -m[ 0 ] * m[ 5 ] * m[ 11 ] + m[ 0 ] * m[ 9 ] * m[ 7 ] + m[ 1 ] * m[ 4 ] * m[ 11 ] - m[ 1 ] * m[ 8 ] * m[ 7 ] - m[ 3 ] * m[ 4 ] * m[ 9 ] + m[ 3 ] * m[ 8 ] * m[ 5 ];
  inv[ 12 ] = -m[ 4 ] * m[ 9 ] * m[ 14 ] + m[ 4 ] * m[ 13 ] * m[ 10 ] + m[ 5 ] * m[ 8 ] * m[ 14 ] - m[ 5 ] * m[ 12 ] * m[ 10 ] - m[ 6 ] * m[ 8 ] * m[ 13 ] + m[ 6 ] * m[ 12 ] * m[ 9 ];
  inv[ 13 ] =  m[ 0 ] * m[ 9 ] * m[ 14 ] - m[ 0 ] * m[ 13 ] * m[ 10 ] - m[ 1 ] * m[ 8 ] * m[ 14 ] + m[ 1 ] * m[ 12 ] * m[ 10 ] + m[ 2 ] * m[ 8 ] * m[ 13 ] - m[ 2 ] * m[ 12 ] * m[ 9 ];
  inv[ 14 ] = -m[ 0 ] * m[ 5 ] * m[ 14 ] + m[ 0 ] * m[ 13 ] * m[ 6 ] + m[ 1 ] * m[ 4 ] * m[ 14 ] - m[ 1 ] * m[ 12 ] * m[ 6 ] - m[ 2 ] * m[ 4 ] * m[ 13 ] + m[ 2 ] * m[ 12 ] * m[ 5 ];
  inv[ 15 ] =  m[ 0 ] * m[ 5 ] * m[ 10 ] - m[ 0 ] * m[ 9 ] * m[ 6 ] - m[ 1 ] * m[ 4 ] * m[ 10 ] + m[ 1 ] * m[ 8 ] * m[ 6 ] + m[ 2 ] * m[ 4 ] * m[ 9 ] - m[ 2 ] * m[ 8 ] * m[ 5 ];

  det = m[ 0 ] * inv[ 0 ] + m[ 4 ] * inv[ 1 ] + m[ 8 ] * inv[ 2 ] + m[ 12 ] * inv[ 3 ];

  if( det == 0 )
    return( false );

  det = float( 1.0 ) / det;

  for( i = 0; i < 16; i++ )
    invOut[ i ] = inv[ i ] * det;

  return( true );
}

// -------------------------------------------------------------------------
// void pickPoint( float& x, float& y, float m[ 16 ], int i, int j )
// {
//   x = m[ 0 ] * float( i ) + m[ 1 ] * float( j ) + m[ 3 ];
//   y = m[ 4 ] * float( i ) + m[ 5 ] * float( j ) + m[ 7 ];
// }

// -------------------------------------------------------------------------
void base( )
{
  glBegin( GL_LINES );

  glColor3f( 1, 0, 0 );
  glVertex2f( -0.1, 0 );
  glVertex2f( 1, 0 );

  glColor3f( 0, 1, 0 );
  glVertex2f( 0, -0.1 );
  glVertex2f( 0, 1 );

  glEnd( );
}

// -------------------------------------------------------------------------
// void field( int mode = GL_LINE_LOOP )
// {
//   glBegin( mode );
//   glVertex2f( world_P0[ 0 ], world_P0[ 1 ] );
//   glVertex2f( world_P1[ 0 ], world_P0[ 1 ] );
//   glVertex2f( world_P1[ 0 ], world_P1[ 1 ] );
//   glVertex2f( world_P0[ 0 ], world_P1[ 1 ] );
//   glEnd( );
// }


// -------------------------------------------------------------------------
void reshape( int width, int height )
{
  // Set the viewport to cover the new window size
  ViewPort[ 0 ] = 0;
  ViewPort[ 1 ] = 0;
  ViewPort[ 2 ] = width;
  ViewPort[ 3 ] = height;
  glViewport( ViewPort[ 0 ], ViewPort[ 1 ], ViewPort[ 2 ], ViewPort[ 3 ] );

  // Compute projection matrix: aspect
  float w = float( ViewPort[ 2 ] - ViewPort[ 0 ] );
  float h = float( ViewPort[ 3 ] - ViewPort[ 1 ] );
  float aw = ( w < h )? 1: ( w / ( ( h != 0 )? h: 1 ) );
  float ah = ( h < w )? 1: ( h / ( ( w != 0 )? w: 1 ) );

  // Compute projection matrix: un-normalized scene size
  double off = 1.01;
  double dx = world_P1[ 0 ] - world_P0[ 0 ];
  double dy = world_P1[ 1 ] - world_P0[ 1 ];
  double s = 2.0 / ( off * ( ( dx < dy )? dy: dx ) );

  // Reset world-to-viewport transformation
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity( );
  glScalef( s / aw, s / ah, 1.0 );

  // Prepare retro-projection matrix (pickup points)
  glGetFloatv( GL_PROJECTION_MATRIX, InvPrj );
  matrix4x4_inv( InvPrj, InvPrj );

  InvPrj[  3 ] += InvPrj[ 1 ] - InvPrj[ 0 ];
  InvPrj[  7 ] += InvPrj[ 5 ] - InvPrj[ 4 ];
  InvPrj[ 11 ] += InvPrj[ 9 ] - InvPrj[ 8 ];
  InvPrj[ 0 ] *=  2.0 / w;
  InvPrj[ 4 ] *=  2.0 / w;
  InvPrj[ 8 ] *=  2.0 / w;
  InvPrj[ 1 ] *= -2.0 / h;
  InvPrj[ 5 ] *= -2.0 / h;
  InvPrj[ 9 ] *= -2.0 / h;
}

// -------------------------------------------------------------------------
int init( const std::string& title, int width, int height )
{
  glutInitWindowSize( width, height );
  int wnd = glutCreateWindow( title.c_str( ) );

  // Clear all rendering objects
  glClearColor( 0, 0, 0, 0 );
  reshape( width, height );
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity( );

  return( wnd );
}

// -------------------------------------------------------------------------
void simulate( int msecs )
{
}

// -------------------------------------------------------------------------
void click( int button, int state, int x, int y )
{

}

// -------------------------------------------------------------------------
void passive_move( int i, int j )
{
}

// -------------------------------------------------------------------------
void keyboard( unsigned char key, int x, int y )
{
  if( key == 27 /*ESC*/ )
    if( main_window != 0 )
      glutDestroyWindow( main_window );
}

// -------------------------------------------------------------------------
void draw( )
{
  // Clear framebuffer and model transform
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity( );

  glColor3f( 0, 0, 0 );
  draw_hierarchy( solar_system );
  
  // Prepare next frame
  glutSwapBuffers( );
}

// -------------------------------------------------------------------------
void idle( )
{
  glutPostRedisplay( );
}

// -------------------------------------------------------------------------
int main( int argc, char** argv )
{
  std::string in_fname = argv[ 1 ];

  // Read file buffer into memory
  std::ifstream in_file( in_fname.c_str( ) );
  std::istringstream in_str(
    std::string( std::istreambuf_iterator< char >{ in_file }, { } )
    );
  in_file.close( );

  // Read JSON data
  try
  {
    boost::property_tree::read_json( in_str, solar_system );
  }
  catch( const std::exception& err )
  {
    std::cerr << "Error caught: " << err.what( ) << std::endl;
    return( EXIT_FAILURE );
  } // end try

  world_P0[ 0 ] = -400; 
  world_P0[ 1 ] = -400; 
  world_P1[ 0 ] =  400;
  world_P1[ 1 ] =  400;

  // Configure visualization
  glutInit( &argc, argv );
  glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE );
  main_window = init( "Movimiento planetario!", 700, 700 );

  // View functions
  glutReshapeFunc( reshape );
  glutDisplayFunc( draw );
  glutIdleFunc( idle );

  // Interaction functions
  glutKeyboardFunc( keyboard );
  glutMouseFunc( click );
  glutPassiveMotionFunc( passive_move );

  // Go!
  glutMainLoop( );

  return( EXIT_SUCCESS );
}

// eof - example_read_json.cxx

