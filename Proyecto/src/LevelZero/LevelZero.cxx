
#include <iostream>
#include <Laberith/BaseApplication.h>
#include <Laberith/CameraMan.h>

#include <climits>
#include <filesystem>
#include <sstream>
#include <unistd.h>

#include <Ogre.h>

/**
 */
class LevelZero
  : public Laberith::BaseApplication
{
public:
  LevelZero( const std::string& work_dir );
  virtual ~LevelZero( ) override;

protected:
  virtual void _loadScene( ) override;
};

// -------------------------------------------------------------------------
int main( int argc, char** argv )
{
  std::filesystem::path p( argv[ 0 ] );
  LevelZero app( p.parent_path( ).string( ) );
  app.go( );
  return( EXIT_SUCCESS );
}

// -------------------------------------------------------------------------
LevelZero::
LevelZero( const std::string& work_dir )
  : Laberith::BaseApplication( "LevelZero v1.0", "" )
{
  static char name_template[] = "tmp_LevelZero_XXXXXXXXX";
  char fname[ PATH_MAX ];

  std::stringstream contents;
  contents << "[General]" << std::endl;
  contents << "Zip=" << work_dir << "/LevelZero_resources.zip" << std::endl;

  std::strcpy( fname, name_template );
  int fd = mkstemp( fname );
  write( fd, contents.str( ).c_str( ), contents.str( ).size( ) );
  close( fd );
  this->m_ResourcesFile = fname;
}

// -------------------------------------------------------------------------
LevelZero::
~LevelZero( )
{
  unlink( this->m_ResourcesFile.c_str( ) );
}

// -------------------------------------------------------------------------
void LevelZero::
_loadScene( )
{
  auto* root = this->getRoot( );
  auto* root_node = this->m_SceneMgr->getRootSceneNode( );

  // Load room geometry
  auto room = this->_load_using_vtk( "LevelZero_resources/room.obj", "room" );
  auto bb = room->getBoundingBox( );
  auto cog = bb.getMaximum( ) + bb.getMinimum( );
  cog *= 0;
  std::cout<<cog<<std::endl;
  // Configure illumination
  this->m_SceneMgr->setAmbientLight( Ogre::ColourValue( 1, 1, 1 ) );

  auto light = this->m_SceneMgr->createLight( "MainLight" );
  auto lightnode = root_node->createChildSceneNode( );
  lightnode->setPosition( cog );
  lightnode->attachObject( light );

  // Configure camera
  auto cam = this->m_SceneMgr->createCamera( "MainCamera" );
  cam->setNearClipDistance( 0.005 );
  cam->setAutoAspectRatio( true );
  auto camnode = root_node->createChildSceneNode( );
  camnode->setPosition( cog );
  camnode->attachObject( cam );

  this->m_CamMan = new Laberith::CameraMan( camnode, bb );
  this->addInputListener( this->m_CamMan );

  auto vp = this->getRenderWindow( )->addViewport( cam );
  vp->setBackgroundColour( Ogre::ColourValue( 0, 0, 0 ) );

  // Meshes
  auto room_mesh = room->convertToMesh( "room", "General" );
  auto room_ent = this->m_SceneMgr->createEntity( "room" );
  this->m_SceneMgr->getRootSceneNode( )->attachObject( room_ent );

  auto table = this->_load_using_vtk( "LevelZero_resources/table.stl", "room" );
  auto table_mesh = table->convertToMesh( "table", "General" );
  auto table_ent = this->m_SceneMgr->createEntity( "table" );
  this->m_SceneMgr->getRootSceneNode( )->attachObject( table_ent );
}

// eof - $RCSfile$
