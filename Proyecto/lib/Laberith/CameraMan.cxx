
#include <Laberith/CameraMan.h>
#include <Ogre.h>

// -------------------------------------------------------------------------
Laberith::CameraMan::
CameraMan( Ogre::SceneNode* cam, const Ogre::AxisAlignedBox& bb )
  : Superclass( ),
    m_BoundingBox( bb )
{
  this->setCamera( cam );
}

// -------------------------------------------------------------------------
void  Laberith::CameraMan::
setCamera( Ogre::SceneNode* cam )
{
  this->m_Camera = cam;
}

// -------------------------------------------------------------------------
Ogre::SceneNode* Laberith::CameraMan::
getCamera( )
{
  return( this->m_Camera );
}

// -------------------------------------------------------------------------
const Ogre::AxisAlignedBox& Laberith::CameraMan::
getBoundingBox( ) const
{
  return( this->m_BoundingBox );
}

// -------------------------------------------------------------------------
void Laberith::CameraMan::
setTopSpeed( Ogre::Real topSpeed )
{
  this->m_Top = topSpeed;
}

// -------------------------------------------------------------------------
Ogre::Real Laberith::CameraMan::
getTopSpeed( )
{
  return( this->m_Top );
}

// -------------------------------------------------------------------------
void Laberith::CameraMan::
frameRendered( const Ogre::FrameEvent& evt )
{
  if( this->m_Forward || this->m_Backwards || this->m_Left || this->m_Right )
  {
    Ogre::Vector3 v = this->m_Camera->getLocalAxes( ).GetColumn( 2 );
    v[ 1 ] = 0;
    v.normalise( );

    Ogre::Real n = this->m_Top * Ogre::Real( evt.timeSinceLastFrame );
    n *= Ogre::Real( ( this->m_Fast )? 40: 15 );

    Ogre::Real na = 0, nb = 0;
    if( this->m_Forward || this->m_Backwards )
      na = n * Ogre::Real( ( this->m_Forward )? -1: 1 );
    if( this->m_Left || this->m_Right )
      nb = n * Ogre::Real( ( this->m_Left )? 1: -1 );

    Ogre::Vector3 p(
      ( na * v[ 0 ] ) - ( nb * v[ 2 ] ),
      Ogre::Real( 0 ),
      ( na * v[ 2 ] ) + ( nb * v[ 0 ] )
      );
    p += this->m_Camera->getPosition( );
    if( this->m_BoundingBox.intersects( p ) )
      this->m_Camera->setPosition( p );
  } // end if
}

// -------------------------------------------------------------------------
bool Laberith::CameraMan::
keyPressed( const OgreBites::KeyboardEvent& evt )
{
  switch( evt.keysym.sym )
  {
  case 'w': case OgreBites::SDLK_UP   : this->m_Forward   = true; break;
  case 's': case OgreBites::SDLK_DOWN : this->m_Backwards = true; break;
  case 'a': case OgreBites::SDLK_LEFT : this->m_Left      = true; break;
  case 'd': case OgreBites::SDLK_RIGHT: this->m_Right     = true; break;
  case OgreBites::SDLK_LSHIFT         : this->m_Fast      = true; break;
  default: break;
  } // end switch
  return( this->Superclass::keyPressed( evt ) );
}

// -------------------------------------------------------------------------
bool Laberith::CameraMan::
keyReleased( const OgreBites::KeyboardEvent& evt )
{
  switch( evt.keysym.sym )
  {
  case 'w': case OgreBites::SDLK_UP   : this->m_Forward   = false; break;
  case 's': case OgreBites::SDLK_DOWN : this->m_Backwards = false; break;
  case 'a': case OgreBites::SDLK_LEFT : this->m_Left      = false; break;
  case 'd': case OgreBites::SDLK_RIGHT: this->m_Right     = false; break;
  case OgreBites::SDLK_LSHIFT         : this->m_Fast      = false; break;
  default: break;
  } // end switch
  return( this->Superclass::keyPressed( evt ) );
}

// -------------------------------------------------------------------------
bool Laberith::CameraMan::
mouseMoved( const OgreBites::MouseMotionEvent& evt )
{
  this->m_Camera->yaw(
    Ogre::Degree( -evt.xrel * 0.15f ), Ogre::Node::TS_PARENT
    );
  this->m_Camera->pitch( Ogre::Degree( -evt.yrel * 0.15f ) );
  return( this->Superclass::mouseMoved( evt ) );
}

// -------------------------------------------------------------------------
bool Laberith::CameraMan::
mouseWheelRolled( const OgreBites::MouseWheelEvent& evt )
{
  return( this->Superclass::mouseWheelRolled( evt ) );
}

// -------------------------------------------------------------------------
bool Laberith::CameraMan::
mousePressed( const OgreBites::MouseButtonEvent& evt )
{
  return( this->Superclass::mousePressed( evt ) );
}

// -------------------------------------------------------------------------
bool Laberith::CameraMan::
mouseReleased( const OgreBites::MouseButtonEvent& evt )
{
  return( this->Superclass::mouseReleased( evt ) );
}

