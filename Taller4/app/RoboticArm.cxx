// -------------------------------------------------------------------------
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// -------------------------------------------------------------------------

#include <iostream>

#include <PUJ_CGraf/AmbientLight.h>
#include <PUJ_CGraf/Controller.h>
#include <PUJ_CGraf/SphericalCamera.h>
#include <PUJ_CGraf/SpotLight.h>
#include <PUJ_CGraf/World.h>

#include <PUJ_CGraf/Object.h>
#include <PUJ_CGraf/Cylinder.h>
#include <PUJ_CGraf/Sphere.h>

/**
 */
class RoboticArm
  : public PUJ_CGraf::Object
  {
  public:
    using Self = RoboticArm;
    using Superclass = PUJ_CGraf::Object;
    using TReal = Superclass::TReal;
    using TNatural = Superclass::TNatural;
    using VReal = Superclass::VReal;
    using VNatural = Superclass::VNatural;

  public:
    RoboticArm( unsigned int nJoints );
    virtual ~RoboticArm( ) = default;

    virtual void keypressed (unsigned char k) override
    {
      // std::cout<< "key: " << int(k) << std::endl;
      if (std::isdigit(k))
        this -> m_Articulation = k -'0';
      else if (k == 'x')
        this-> m_Joints[this -> m_Articulation].rotate(0.1,1,0,0);
      else if (k == 'X')
        this-> m_Joints[this -> m_Articulation].rotate(0.1,-1,0,0);
      else if (k == 'y')
        this-> m_Joints[this -> m_Articulation].rotate(0.1,0,1,0);
      else if (k == 'Y')
        this-> m_Joints[this -> m_Articulation].rotate(0.1,0,-1,0);
      else if (k == 'z')
        this-> m_Joints[this -> m_Articulation].rotate(0.1,0,0,1);
      else if (k == 'Z')
        this-> m_Joints[this -> m_Articulation].rotate(0.1,0,0,-1);
    }
    virtual void draw( ) const override;

  protected:
    int m_Articulation { -1 };
    std::vector< PUJ_CGraf::Sphere > m_Joints;
    std::vector< PUJ_CGraf::Cylinder > m_Arms;
};

// -------------------------------------------------------------------------
int main( int argc, char** argv )
{
  // Configure window
  PUJ_CGraf::Controller::set_shade_type_to_flat( );
  PUJ_CGraf::Controller::init( &argc, argv, "Robotic Arm", 700, 700 );

  // Configure world
  RoboticArm arm( 3 );
  PUJ_CGraf::World world;
  world.add_child( &arm );

  PUJ_CGraf::AmbientLight ambient_light;
  ambient_light.set_color( 1, 1, 1 );
  world.add_light( &ambient_light );

  PUJ_CGraf::SphericalCamera cam;
  world.set_camera( &cam );

  // Main loop
  PUJ_CGraf::Controller::set_world( &world );
  PUJ_CGraf::Controller::loop( );

  return( EXIT_SUCCESS );
}

// -------------------------------------------------------------------------
RoboticArm::
RoboticArm( unsigned int nJoints )
{
  this->m_Joints.resize( nJoints );
  this->m_Arms.resize( nJoints );
  this->m_Joints.shrink_to_fit( );
  this->m_Arms.shrink_to_fit( );

  float h = 30;
  for( unsigned int j = 0; j < nJoints; ++j )
  {
    this->m_Joints[ j ].set_radius( 5 );
    this->m_Joints[ j ].set_center( 0, 0, 0 );

    this->m_Arms[ j ].set_radius( 1.5 );
    this->m_Arms[ j ].set_height( h );
    this->m_Arms[ j ].set_center( 0, 0, 0 );

    this->m_Joints[ j ].sample( 10, 10 );
    this->m_Arms[ j ].sample( 10, 10 );

    this->m_Joints[ j ].add_child( &( this->m_Arms[ j ] ) );
    if( j > 0 )
      this->m_Arms[ j - 1 ]
        .add_child( &( this->m_Joints[ j ] ), 0, 0, h );
  } // end for

  this->m_BBox[ 0 ] = -50;
  this->m_BBox[ 1 ] = -50;
  this->m_BBox[ 2 ] = -50;
  this->m_BBox[ 3 ] =  50;
  this->m_BBox[ 4 ] =  50;
  this->m_BBox[ 5 ] =  50;
}

// -------------------------------------------------------------------------
void RoboticArm::
draw( ) const
{
  if( this->m_Joints.size( ) > 0 )
    this->m_Joints[ 0 ].draw( );
}

// eof - RoboticArm.cxx
