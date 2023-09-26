// -------------------------------------------------------------------------
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// -------------------------------------------------------------------------

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#ifdef _MSC_VER
#  include <boost/config/compiler/visualc.hpp>
#endif
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

#include <OpenGL/gl.h>
#include <GLUT/glut.h>

// -------------------------------------------------------------------------
void print_tree(
  const boost::property_tree::ptree& tree,
  const std::string& indent = ""
  )
{
  for(
    boost::property_tree::ptree::const_iterator i = tree.begin( );
    i != tree.end( );
    ++i
    )
  {
    std::cout
      << indent
      << i->first << ": " << i->second.get_value< std::string >( )
      << std::endl;
    print_tree( i->second, indent + "\t" );
  } // end for
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
  boost::property_tree::ptree solar_system;
  try
  {
    boost::property_tree::read_json( in_str, solar_system );
  }
  catch( const std::exception& err )
  {
    std::cerr << "Error caught: " << err.what( ) << std::endl;
    return( EXIT_FAILURE );
  } // end try

  // Show tree contents
  print_tree( solar_system );

  return( EXIT_SUCCESS );
}

// eof - example_read_json.cxx
