
#include <Laberith/BaseApplication.h>

#include <Ogre.h>
#include <Laberith/CameraMan.h>

// -------------------------------------------------------------------------
Laberith::BaseApplication::
BaseApplication( const std::string& title, const std::string& res_file )
  : OgreBites::ApplicationContext( title ),
    OgreBites::InputListener( )
{
  this->m_ResourcesFile = res_file;
}

// -------------------------------------------------------------------------
Laberith::BaseApplication::
~BaseApplication( )
{
  if( this->m_CamMan != nullptr )
    delete this->m_CamMan;
}

// -------------------------------------------------------------------------
void Laberith::BaseApplication::
locateResources( )
{
}

// -------------------------------------------------------------------------
void Laberith::BaseApplication::
loadResources( )
{
  this->enableShaderCache( );
  Ogre::ConfigFile cf = Ogre::ConfigFile( );
  cf.loadDirect( this->m_ResourcesFile );

  auto res_mgr = Ogre::ResourceGroupManager::getSingletonPtr( );
  auto settings = cf.getSettingsBySection( );
  for( auto sIt = settings.begin( ); sIt != settings.end( ); ++sIt )
    for( auto fIt = sIt->second.begin( ); fIt != sIt->second.end( ); ++fIt )
      res_mgr->addResourceLocation( fIt->second, fIt->first, sIt->first );
  try
  {
    res_mgr->initialiseAllResourceGroups( );
    res_mgr->loadResourceGroup( "General" );
  }
  catch( ... )
  {
    // Do nothing
  }
}

// -------------------------------------------------------------------------
void Laberith::BaseApplication::
setup( )
{
  this->_initSceneManager( );
  this->setWindowGrab( true );
  this->_loadScene( );
}

// -------------------------------------------------------------------------
bool Laberith::BaseApplication::
keyPressed( const OgreBites::KeyboardEvent& evt )
{
  if( evt.keysym.sym == OgreBites::SDLK_ESCAPE )
    this->getRoot( )->queueEndRendering( );
  return( true );
}

// -------------------------------------------------------------------------
void Laberith::BaseApplication::
go( )
{
  this->initApp( );
  this->getRoot( )->startRendering( );
  this->closeApp( );
}

// -------------------------------------------------------------------------
void Laberith::BaseApplication::
_initSceneManager( )
{
  this->OgreBites::ApplicationContext::setup( );
  this->addInputListener( this );

  auto* root = this->getRoot( );
  this->m_SceneMgr = root->createSceneManager( "DefaultSceneManager" );

  auto* shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr( );
  shadergen->addSceneManager( this->m_SceneMgr );
}

// eof - $RCSfile$
