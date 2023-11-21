
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
class LevelTwo
  : public Laberith::BaseApplication
{
public:
  LevelTwo( const std::string& work_dir );
  virtual ~LevelTwo( ) override;

  // virtual bool keyPressed( const OgreBites::KeyboardEvent& evt ) override;
  virtual bool frameStarted( const Ogre::FrameEvent& evt ) override;
protected:
  virtual void _loadScene( ) override;
protected:

  // Ogre::SceneNode* Laberinth     { nullptr };
  // btRigidBody*     Laberinth_Wall { nullptr };

  Ogre::SceneNode* m_Sphere     { nullptr };
  btRigidBody*     m_SphereBody { nullptr };
  std::vector<Ogre::SceneNode*> mSphereNodes;
  std::vector<btRigidBody*> mSphereBodies;
  bool m_Simulating { true };

  btDefaultCollisionConfiguration* m_BTConf;
  btCollisionDispatcher* m_BTDispatcher;
  btBroadphaseInterface* m_BTCache;
  btSequentialImpulseConstraintSolver* m_BTSolver;
  btDiscreteDynamicsWorld* m_BTWorld;
  btAlignedObjectArray< btCollisionShape* > m_BTShapes;
};

// -------------------------------------------------------------------------
// bool LevelTwo::
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
  LevelTwo app( p.parent_path( ).string( ) );
  app.go( );
  return( EXIT_SUCCESS );
}

// -------------------------------------------------------------------------
LevelTwo::
LevelTwo( const std::string& work_dir )
  : Laberith::BaseApplication( "LevelTwo v1.0", "" )
{
  static char name_template[] = "tmp_LevelTwo_XXXXXXXXX";
  char fname[ PATH_MAX ];

  std::stringstream contents;
  contents << "[General]" << std::endl;
  contents << "Zip=" << work_dir << "/LevelTwo_resources.zip" << std::endl;

  std::strcpy( fname, name_template );
  int fd = mkstemp( fname );
  write( fd, contents.str( ).c_str( ), contents.str( ).size( ) );
  close( fd );
  this->m_ResourcesFile = fname;
}

// -------------------------------------------------------------------------
LevelTwo::
~LevelTwo( )
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
bool LevelTwo::
frameStarted( const Ogre::FrameEvent& evt )
{
    this->m_BTWorld->stepSimulation( evt.timeSinceLastEvent, 10 );

    for (size_t i = 0; i < mSphereBodies.size(); ++i) {
        btRigidBody* sphereBody = this-> mSphereBodies[i];
        Ogre::SceneNode* sphereNode = this-> mSphereNodes[i];

        btTransform trans;
        if (sphereBody && sphereBody->getMotionState()) {
            sphereBody->getMotionState()->getWorldTransform(trans);
        } else {
            trans = sphereBody->getWorldTransform();
        }

        sphereNode->setPosition(
            trans.getOrigin().getX(),
            trans.getOrigin().getY(),
            trans.getOrigin().getZ()
        );
    }

  return( this->Laberith::BaseApplication::frameStarted( evt ) );
}
// -------------------------------------------------------------------------
void LevelTwo::
_loadScene( )
{
  std::mt19937 rng(std::random_device{}());

  std::uniform_int_distribution<int> dist(-100, 100);
  auto* root = this->getRoot( );
  auto* root_node = this->m_SceneMgr->getRootSceneNode( );
  int number_spheres=15;
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
          Ogre::AxisAlignedBox( -1500, 0, -1500, 1500, 5, 1500 ) );
  this->addInputListener( this->m_CamMan );

  auto vp = this->getRenderWindow( )->addViewport( cam );
  vp->setBackgroundColour( Ogre::ColourValue( 0, 0, 0 ) );


  // load all the laberinths
  auto main_path = this->_load_using_vtk( "LevelTwo_resources/laberinth_level3_1.obj", "main_path" );
  auto main_path_mesh = main_path->convertToMesh( "main_path", "General" );
  auto main_path_ent = this->m_SceneMgr->createEntity( "main_path" );
  this->m_SceneMgr->getRootSceneNode( )->attachObject( main_path_ent );
  
  auto second_path = this->_load_using_vtk( "LevelTwo_resources/laberinth_level3_2.obj", "second_path" );
  auto second_path_mesh = second_path->convertToMesh( "second_path", "General" );
  auto second_path_ent = this->m_SceneMgr->createEntity( "second_path" );
  this->m_SceneMgr->getRootSceneNode( )->attachObject( second_path_ent );


  auto third_path = this->_load_using_vtk( "LevelTwo_resources/laberinth_level3_3.obj", "third_path" );
  auto third_path_mesh = third_path->convertToMesh( "third_path", "General" );
  auto third_path_ent = this->m_SceneMgr->createEntity( "third_path" );
  this->m_SceneMgr->getRootSceneNode( )->attachObject( third_path_ent );


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



  //create spheres
  for (int i =0;i<number_spheres;++i){
    // Load the sphere mesh
    auto sphere = this->_load_using_vtk("LevelTwo_resources/sphere"+std::to_string(i)+".vtp", "sphere" + std::to_string(i));
    auto sphere_mesh = sphere->convertToMesh("sphere" + std::to_string(i), "General");

    // Create an entity for the sphere
    auto sphere_ent = this->m_SceneMgr->createEntity("sphere" + std::to_string(i));
    auto sphere_node = root_node->createChildSceneNode("sphereNode" + std::to_string(i));
    sphere_node->scale(15, 15, 15);

    int x_sphere = dist(rng);
    int z_sphere = dist(rng);
    // int x_sphere = i*10;
    // int z_sphere = i*10;
    sphere_node->setPosition(Ogre::Vector3(x_sphere, 0.5, z_sphere));
    sphere_node->attachObject(sphere_ent);

    // Add the node to the vector
    this-> mSphereNodes.push_back(sphere_node);

    // Physics integration
    btScalar sphere_radius = sphere_ent->getBoundingRadius();
    btCollisionShape* sphere_shape = new btSphereShape(sphere_radius);
    this->m_BTShapes.push_back(sphere_shape);

    btTransform sphere_transform;
    sphere_transform.setIdentity();
    btScalar sphere_mass(1);
    btVector3 sphere_inertia(0, 0, 0);
    sphere_shape->calculateLocalInertia(sphere_mass, sphere_inertia);

    // Use the position of the Ogre::SceneNode for the sphere
    Ogre::Vector3 ogreSpherePos = sphere_node->getPosition();
    sphere_transform.setOrigin(btVector3(ogreSpherePos.x, ogreSpherePos.y, ogreSpherePos.z));

    btDefaultMotionState* sphere_state = new btDefaultMotionState(sphere_transform);
    btRigidBody::btRigidBodyConstructionInfo sphere_info(sphere_mass, sphere_state, sphere_shape, sphere_inertia);
    btRigidBody* sphere_body = new btRigidBody(sphere_info);
    sphere_body->setRestitution(1);
    sphere_body->setFriction(0.08);
    sphere_body->setLinearVelocity(btVector3(0, 3, 0));

    this->m_BTWorld->addRigidBody(sphere_body);
    this-> mSphereBodies.push_back(sphere_body);

  }

}

// eof - $RCSfile$
