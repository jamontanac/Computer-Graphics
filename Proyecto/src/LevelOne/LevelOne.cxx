
#include <iostream>
#include <Laberith/BaseApplication.h>
#include <Laberith/CameraMan.h>

#include <climits>
#include <filesystem>
#include <sstream>
#include <unistd.h>

#include <Ogre.h>
#include <btBulletDynamicsCommon.h>

/**
 */
class LevelOne
  : public Laberith::BaseApplication
{
public:
  LevelOne( const std::string& work_dir );
  virtual ~LevelOne( ) override;

  virtual bool keyPressed( const OgreBites::KeyboardEvent& evt ) override;
  virtual bool frameStarted( const Ogre::FrameEvent& evt ) override;

protected:
  virtual void _loadScene( ) override;

protected:
  Ogre::SceneNode* m_Sphere     { nullptr };
  btRigidBody*     m_SphereBody { nullptr };

  Ogre::SceneNode* m_Cube1     { nullptr };
  btRigidBody*     m_Cube1Body { nullptr };

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
  LevelOne app( p.parent_path( ).string( ) );
  app.go( );
  return( EXIT_SUCCESS );
}

// -------------------------------------------------------------------------
LevelOne::
LevelOne( const std::string& work_dir )
  : Laberith::BaseApplication( "LevelOne v1.0", "" )
{
  static char name_template[] = "tmp_LevelOne_XXXXXXXXX";
  char fname[ PATH_MAX ];

  std::stringstream contents;
  contents << "[General]" << std::endl;
  contents
    << "Zip=" << work_dir
    << "/LevelOne_resources.zip" << std::endl;

  std::strcpy( fname, name_template );
  int fd = mkstemp( fname );
  write( fd, contents.str( ).c_str( ), contents.str( ).size( ) );
  close( fd );
  this->m_ResourcesFile = fname;
}

// -------------------------------------------------------------------------
LevelOne::
~LevelOne( )
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
bool LevelOne::
keyPressed( const OgreBites::KeyboardEvent& evt )
{
  if( evt.keysym.sym == 'f' )
  {
    this->m_SphereBody->getWorldTransform( ).setOrigin(
      btVector3(
        this->m_Sphere->getPosition( )[ 0 ],
        this->m_Sphere->getPosition( )[ 1 ],
        this->m_Sphere->getPosition( )[ 2 ]
        )
      );

    auto cam = this->m_CamMan->getCamera( );
    this->m_SphereBody->setLinearVelocity(
      btVector3(
        -10 * cam->getLocalAxes( ).GetColumn( 2 )[ 0 ],
        -10 * cam->getLocalAxes( ).GetColumn( 2 )[ 1 ],
        -10 * cam->getLocalAxes( ).GetColumn( 2 )[ 2 ]
        )
      );

    this->m_Simulating = true;
  } // end if
  return( this->Laberith::BaseApplication::keyPressed( evt ) );
}

// -------------------------------------------------------------------------
bool LevelOne::
frameStarted( const Ogre::FrameEvent& evt )
{
  if( this->m_Simulating )
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

    if( this->m_Cube1Body && this->m_Cube1Body->getMotionState( ) )
      this->m_Cube1Body->getMotionState( )->getWorldTransform( trans );
    else
      trans = this->m_Cube1Body->getWorldTransform( );
    btQuaternion rot = trans.getRotation( );
    this->m_Cube1->setOrientation( Ogre::Quaternion( rot.getW( ), rot.getX( ), rot.getY( ), rot.getZ( ) ) );
    this->m_Cube1->setPosition(
      trans.getOrigin( ).getX( ),
      trans.getOrigin( ).getY( ),
      trans.getOrigin( ).getZ( )
      );
  }
  else
  {
    auto cam = this->m_CamMan->getCamera( );
    this->m_Sphere->setPosition(
      cam->getPosition( ) - cam->getLocalAxes( ).GetColumn( 2 )
      );
  } // end if
  return( this->Laberith::BaseApplication::frameStarted( evt ) );
}

// -------------------------------------------------------------------------
void LevelOne::
_loadScene( )
{
  auto* root = this->getRoot( );
  auto* root_node = this->m_SceneMgr->getRootSceneNode( );

  // Configure camera
  auto cam = this->m_SceneMgr->createCamera( "MainCamera" );
  cam->setNearClipDistance( 0.005 );
  cam->setAutoAspectRatio( true );
  auto camnode = root_node->createChildSceneNode( );
  camnode->setPosition( Ogre::Vector3( 0, 1.7, 20 ) );
  camnode->attachObject( cam );

  this->m_CamMan =
    new Laberith::CameraMan(
      camnode,
      Ogre::AxisAlignedBox( -1000, 0, -1000, 1000, 2, 1000 )
      );
  this->addInputListener( this->m_CamMan );

  auto vp = this->getRenderWindow( )->addViewport( cam );
  vp->setBackgroundColour( Ogre::ColourValue( 0, 0, 0 ) );

  // Configure illumination
  this->m_SceneMgr->setAmbientLight( Ogre::ColourValue( 1, 1, 1 ) );

  auto light = this->m_SceneMgr->createLight( "MainLight" );
  auto lightnode = root_node->createChildSceneNode( );
  lightnode->setPosition( Ogre::Vector3( 0, 10, 0 ) );
  lightnode->attachObject( light );

  // Load floor
  auto floor =
    this->_load_using_vtk( "LevelOne_resources/floor.obj", "floor" );
  auto floor_mesh = floor->convertToMesh( "floor", "General" );
  auto floor_ent = this->m_SceneMgr->createEntity( "floor" );
  root_node->attachObject( floor_ent );

  // Load sphere
  auto sphere =
    this->_load_using_vtk( "LevelOne_resources/sphere.vtp", "sphere" );
  auto sphere_mesh = sphere->convertToMesh( "sphere", "General" );
  auto sphere_ent = this->m_SceneMgr->createEntity( "sphere" );
  this->m_Sphere = root_node->createChildSceneNode( );
  this->m_Sphere->setPosition(
    camnode->getPosition( )
    -
    camnode->getLocalAxes( ).GetColumn( 2 )
    );
  this->m_Sphere->attachObject( sphere_ent );

  // Load some cubes
  auto cube1 =
    this->_load_using_vtk( "LevelOne_resources/cube.vtp", "cube1" );
  auto cube_mesh = cube1->convertToMesh( "cube", "General" );
  auto cube_ent = this->m_SceneMgr->createEntity( "cube" );
  this->m_Cube1 = root_node->createChildSceneNode( );
  this->m_Cube1->setPosition( 0, 0.5, 0 );
  this->m_Cube1->attachObject( cube_ent );

  // Create "physical" world
  this->m_BTConf = new btDefaultCollisionConfiguration( );
  this->m_BTDispatcher = new btCollisionDispatcher( this->m_BTConf );
  this->m_BTCache = new btDbvtBroadphase( );
  this->m_BTSolver = new btSequentialImpulseConstraintSolver( );
  this->m_BTWorld = new btDiscreteDynamicsWorld(
    this->m_BTDispatcher, this->m_BTCache, this->m_BTSolver, this->m_BTConf
    );
  this->m_BTWorld->setGravity( btVector3( 0, -9.8, 0 ) );

  // Physical floor
  btCollisionShape* floor_shape = new btStaticPlaneShape(
    btVector3( 0, 1, 0 ), 0
    );
  this->m_BTShapes.push_back( floor_shape );

  btTransform floor_tr;
  floor_tr.setIdentity( );

  btVector3 floor_inertia( 0, 0, 0 );

  btDefaultMotionState* floor_state = new btDefaultMotionState( floor_tr );
  btRigidBody::btRigidBodyConstructionInfo floor_info(
    0.0, floor_state, floor_shape, floor_inertia
    );

  btRigidBody* floor_body = new btRigidBody( floor_info );
  floor_body->setRestitution( 0.5 );
  floor_body->setFriction( 0.7 );
  this->m_BTWorld->addRigidBody( floor_body );

  // Physical sphere
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
  this->m_SphereBody->setRestitution( 0.9 );
  this->m_SphereBody->setFriction( 0.08 );
  this->m_BTWorld->addRigidBody( this->m_SphereBody );

  // Physical cube1
  Ogre::Vector3 cube1_size = cube_ent->getBoundingBox( ).getHalfSize( );

  btCollisionShape* cube1_shape = new btBoxShape(
    btVector3(
      cube1_size[ 0 ],
      cube1_size[ 1 ],
      cube1_size[ 2 ]
      )
    );
  this->m_BTShapes.push_back( cube1_shape );

  btTransform cube1_transform;
  cube1_transform.setIdentity( );

  btScalar cube1_mass( 1 );

  btVector3 cube1_inertia( 0, 0, 0 );
  cube1_shape->calculateLocalInertia( cube1_mass, cube1_inertia );

  cube1_transform.setOrigin(
    btVector3(
      this->m_Cube1->getPosition( )[ 0 ],
      this->m_Cube1->getPosition( )[ 1 ],
      this->m_Cube1->getPosition( )[ 2 ]
      )
    );

  btDefaultMotionState* cube1_state =
    new btDefaultMotionState( cube1_transform );
  btRigidBody::btRigidBodyConstructionInfo cube1_info(
    cube1_mass, cube1_state, cube1_shape, cube1_inertia
    );
  this->m_Cube1Body = new btRigidBody( cube1_info );
  this->m_Cube1Body->setRestitution( 0.9 );
  this->m_Cube1Body->setFriction( 0.8 );
  this->m_BTWorld->addRigidBody( this->m_Cube1Body );
}

