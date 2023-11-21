
#include <iostream>
#include <Laberith/BaseApplication.h>
#include <Laberith/CameraMan.h>

#include <climits>
#include <filesystem>
#include <sstream>
#include <unistd.h>

#include <btBulletDynamicsCommon.h>
#include <Ogre.h>

/**
 */
class LevelZero
  : public Laberith::BaseApplication
{
public:
  LevelZero( const std::string& work_dir );
  virtual ~LevelZero( ) override;

  // virtual bool keyPressed( const OgreBites::KeyboardEvent& evt ) override;
  // virtual bool frameStarted( const Ogre::FrameEvent& evt ) override;
protected:
  virtual void _loadScene( ) override;
protected:

  Ogre::SceneNode* walls     { nullptr };
  btRigidBody*     walls_body { nullptr };

  bool m_Simulating { false };

  btDefaultCollisionConfiguration* m_BTConf;
  btCollisionDispatcher* m_BTDispatcher;
  btBroadphaseInterface* m_BTCache;
  btSequentialImpulseConstraintSolver* m_BTSolver;
  btDiscreteDynamicsWorld* m_BTWorld;
  btAlignedObjectArray< btCollisionShape* > m_BTShapes;
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
  for( int i = this->m_BTWorld->getNumCollisionObjects( ) - 1; i >= 0; i-- )
  {
    btCollisionObject* obj = this->m_BTWorld->getCollisionObjectArray( )[ i ];
    btRigidBody* body = btRigidBody::upcast( obj );
    if( body && body->getMotionState( ) )
      delete body->getMotionState( );
    this->m_BTWorld->removeCollisionObject( obj );
    delete obj;
  } // end for

  for( int j = 0; j < this->m_BTShapes.size( ); j++ )
  {
    btCollisionShape* shape = this->m_BTShapes[ j ];
    this->m_BTShapes[ j ] = 0;
    delete shape;
  } // end for

  delete this->m_BTWorld;
  delete this->m_BTSolver;
  delete this->m_BTCache;
  delete this->m_BTDispatcher;
  delete this->m_BTConf;
  this->m_BTShapes.clear( );
  unlink( this->m_ResourcesFile.c_str( ) );
}

// -------------------------------------------------------------------------
void LevelZero::
_loadScene( )
{
  auto* root = this->getRoot( );
  auto* root_node = this->m_SceneMgr->getRootSceneNode( );

  // Configure illumination
  this->m_SceneMgr->setAmbientLight( Ogre::ColourValue( 1, 1, 1 ) );
  auto light = this->m_SceneMgr->createLight( "MainLight" );
  auto lightnode = root_node->createChildSceneNode( );
  lightnode->setPosition(  Ogre::Vector3( 0, 0, 0 )   );
  lightnode->attachObject( light );

  // Configure camera
  auto cam = this->m_SceneMgr->createCamera( "MainCamera" );
  cam->setNearClipDistance( 0.005 );
  cam->setAutoAspectRatio( true );
  auto camnode = root_node->createChildSceneNode( );
  camnode->setPosition( Ogre::Vector3( 0, 0, 0 )  );
  camnode->attachObject( cam );

  this->m_CamMan = new Laberith::CameraMan( camnode,
          Ogre::AxisAlignedBox( -100, 0, -100, 100, 5, 100 ) );
  this->addInputListener( this->m_CamMan );

  auto vp = this->getRenderWindow( )->addViewport( cam );
  vp->setBackgroundColour( Ogre::ColourValue( 0, 0, 0 ) );
  auto main_path = this->_load_using_vtk( "LevelZero_resources/laberinth_level1_1.obj", "main_path" );
  auto second_path = this->_load_using_vtk( "LevelZero_resources/laberinth_level1_2.obj", "second_path" );

  // auto bb = main_path->getBoundingBox( );
  // auto cog = bb.getMaximum( ) + bb.getMinimum( );
  // cog *= 0;
  // std::cout<<cog<<std::endl;

  // Meshes
  auto main_path_mesh = main_path->convertToMesh( "main_path", "General" );
  auto main_path_ent = this->m_SceneMgr->createEntity( "main_path" );
  this->m_SceneMgr->getRootSceneNode( )->attachObject( main_path_ent );

  auto second_path_mesh = second_path->convertToMesh( "second_path", "General" );
  auto second_path_ent = this->m_SceneMgr->createEntity( "second_path" );
  this->m_SceneMgr->getRootSceneNode( )->attachObject( second_path_ent );
}

// eof - $RCSfile$
