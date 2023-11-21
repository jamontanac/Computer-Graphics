
#include <iostream>
#include <Laberith/BaseApplication.h>
#include <Laberith/CameraMan.h>

#include <climits>
#include <filesystem>
#include <sstream>
#include <unistd.h>
#include <random>
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
  virtual bool frameStarted( const Ogre::FrameEvent& evt ) override;
protected:
  virtual void _loadScene( ) override;
protected:

  // Ogre::SceneNode* Laberinth     { nullptr };
  // btRigidBody*     Laberinth_Wall { nullptr };

  Ogre::SceneNode* m_Sphere     { nullptr };
  btRigidBody*     m_SphereBody { nullptr };
  bool m_Simulating { true };

  btDefaultCollisionConfiguration* m_BTConf;
  btCollisionDispatcher* m_BTDispatcher;
  btBroadphaseInterface* m_BTCache;
  btSequentialImpulseConstraintSolver* m_BTSolver;
  btDiscreteDynamicsWorld* m_BTWorld;
  btAlignedObjectArray< btCollisionShape* > m_BTShapes;
};

// -------------------------------------------------------------------------
// bool LevelZero::
// keyPressed( const OgreBites::KeyboardEvent& evt )
// {
//   if( evt.keysym.sym == 'f' )
//   {
//     this->m_SphereBody->getWorldTransform( ).setOrigin(
//       btVector3(
//         this->m_Sphere->getPosition( )[ 0 ],
//         this->m_Sphere->getPosition( )[ 1 ],
//         this->m_Sphere->getPosition( )[ 2 ]
//         )
//       );

//     auto cam = this->m_CamMan->getCamera( );
//     this->m_SphereBody->setLinearVelocity(
//       btVector3(0,0,0)
//       // btVector3(
//       //   -10 * cam->getLocalAxes( ).GetColumn( 2 )[ 0 ],
//       //   -10 * cam->getLocalAxes( ).GetColumn( 2 )[ 1 ],
//       //   -10 * cam->getLocalAxes( ).GetColumn( 2 )[ 2 ]
//       //   )
//       );

//     this->m_Simulating = true;
//   } // end if
//   return( this->Laberith::BaseApplication::keyPressed( evt ) );
// }
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
bool LevelZero::
frameStarted( const Ogre::FrameEvent& evt )
{
    this->m_BTWorld->stepSimulation( evt.timeSinceLastEvent, 10 );

    btTransform trans;
    if( this->m_SphereBody && this->m_SphereBody->getMotionState( ) )
      this->m_SphereBody->getMotionState( )->getWorldTransform( trans );
    else
      trans = this->m_SphereBody->getWorldTransform( );
    this->m_Sphere->setPosition(
      trans.getOrigin( ).getX( ),
      trans.getOrigin( ).getY( ),
      trans.getOrigin( ).getZ( )
      );

  return( this->Laberith::BaseApplication::frameStarted( evt ) );
}
// -------------------------------------------------------------------------
void LevelZero::
_loadScene( )
{
  std::mt19937 rng(std::random_device{}());

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
          Ogre::AxisAlignedBox( -500, -100, -500, 500, 100, 500 ) );
  this->addInputListener( this->m_CamMan );

  auto vp = this->getRenderWindow( )->addViewport( cam );
  vp->setBackgroundColour( Ogre::ColourValue( 0, 0, 0 ) );


  // load all the laberinths
  auto main_path = this->_load_using_vtk( "LevelZero_resources/laberinth_level1_1.obj", "main_path" );
  auto main_path_mesh = main_path->convertToMesh( "main_path", "General" );
  auto main_path_ent = this->m_SceneMgr->createEntity( "main_path" );
  this->m_SceneMgr->getRootSceneNode( )->attachObject( main_path_ent );
  
  auto second_path = this->_load_using_vtk( "LevelZero_resources/laberinth_level1_2.obj", "second_path" );
  auto second_path_mesh = second_path->convertToMesh( "second_path", "General" );
  auto second_path_ent = this->m_SceneMgr->createEntity( "second_path" );
  this->m_SceneMgr->getRootSceneNode( )->attachObject( second_path_ent );


  auto third_path = this->_load_using_vtk( "LevelZero_resources/laberinth_level1_3.obj", "third_path" );
  auto third_path_mesh = third_path->convertToMesh( "third_path", "General" );
  auto third_path_ent = this->m_SceneMgr->createEntity( "third_path" );
  this->m_SceneMgr->getRootSceneNode( )->attachObject( third_path_ent );

  auto sphere =
    this->_load_using_vtk( "LevelZero_resources/sphere.vtp", "sphere" );
  auto sphere_mesh = sphere->convertToMesh( "sphere", "General" );
  auto sphere_ent = this->m_SceneMgr->createEntity( "sphere" );
  this->m_Sphere = root_node->createChildSceneNode( );
  // this->m_Sphere->setPosition(
    // camnode->getPosition( ) + camnode->getLocalAxes( ).GetColumn( 2 )
    // );
  this->m_Sphere->scale(15,15,15);
  std::uniform_int_distribution<int> dist(-100, 100);
  int x_sphere = dist(rng);
  int z_sphere = dist(rng);
  this->m_Sphere->setPosition(
     Ogre::Vector3( x_sphere, 1, z_sphere )  
    // camnode->getPosition( ) - camnode->getLocalAxes( ).GetColumn( 2 )
    );
  this->m_Sphere->attachObject( sphere_ent );


  // Create "physical" world
  this->m_BTConf = new btDefaultCollisionConfiguration( );
  this->m_BTDispatcher = new btCollisionDispatcher( this->m_BTConf );
  this->m_BTCache = new btDbvtBroadphase( );
  this->m_BTSolver = new btSequentialImpulseConstraintSolver( );
  this->m_BTWorld = new btDiscreteDynamicsWorld(
    this->m_BTDispatcher, this->m_BTCache, this->m_BTSolver, this->m_BTConf
    );
  this->m_BTWorld->setGravity(btVector3(0, -9.8, 0));


  // Physical floor
  btCollisionShape* floor_shape = new btStaticPlaneShape(
    btVector3( 0, 0, 0 ), 0);
  this->m_BTShapes.push_back( floor_shape );
  btTransform floor_tr;
  floor_tr.setIdentity( );
  btVector3 floor_inertia( 0, 0, 0 );
  btDefaultMotionState* floor_state = new btDefaultMotionState( floor_tr );
  btRigidBody::btRigidBodyConstructionInfo floor_info(
    0.0, floor_state, floor_shape, floor_inertia
    );
  btRigidBody* floor_body = new btRigidBody( floor_info );
  floor_body->setRestitution( 1 );
  floor_body->setFriction( 0.7 );
  this->m_BTWorld->addRigidBody( floor_body );


  btScalar sphere_radius = sphere_ent->getBoundingRadius( );
  btCollisionShape* sphere_shape = new btSphereShape( sphere_radius );
  this->m_BTShapes.push_back( sphere_shape );

  btTransform sphere_transform;
  sphere_transform.setIdentity();
  btScalar sphere_mass( 1 );
  btVector3 sphere_inertia( 0, 0, 0 );
  sphere_shape->calculateLocalInertia( sphere_mass, sphere_inertia );

  sphere_transform.setOrigin(
    btVector3(
      this->m_Sphere->getPosition( )[ 0 ],
      this->m_Sphere->getPosition( )[ 1 ],
      this->m_Sphere->getPosition( )[ 2 ]
      )
    );

  btDefaultMotionState* sphere_state =
    new btDefaultMotionState( sphere_transform );
  btRigidBody::btRigidBodyConstructionInfo sphere_info(
    sphere_mass, sphere_state, sphere_shape, sphere_inertia
    );
  this->m_SphereBody = new btRigidBody( sphere_info );
  this->m_SphereBody->setRestitution( 1 );
  this->m_SphereBody->setFriction( 0.08 );
  this->m_SphereBody->setLinearVelocity(btVector3(0,3,0));
  this->m_BTWorld->addRigidBody( this->m_SphereBody );
}

// eof - $RCSfile$
