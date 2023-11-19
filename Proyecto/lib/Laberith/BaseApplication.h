#ifndef __Laberith__BaseApplication__h__
#define __Laberith__BaseApplication__h__

#include <Laberith/Export.h>
#include <OgreApplicationContext.h>
#include <OgreInput.h>

class vtkPolyData;
namespace Laberith { class CameraMan; }

namespace Laberith
{
  /**
   */
  class Laberith_EXPORT BaseApplication
    : public OgreBites::ApplicationContext,
      public OgreBites::InputListener
  {
  public:
    BaseApplication( const std::string& title, const std::string& res_file );
    virtual ~BaseApplication( ) override;

    virtual void locateResources( ) override;
    virtual void loadResources( ) override;
    virtual void setup( ) override;
    virtual bool keyPressed( const OgreBites::KeyboardEvent& evt ) override;

    virtual void go( );

  protected:
    virtual void _initSceneManager( );
    virtual void _loadScene( ) = 0;

    template< class _R, class _C >
    void _read_using_vtk( vtkPolyData** pdata, const _C& c );

    Ogre::ManualObject* _load_using_vtk( const std::string& fname, const std::string& material );

  protected:
    std::string m_ResourcesFile;

    Ogre::SceneManager*  m_SceneMgr { nullptr };
    Laberith::CameraMan* m_CamMan   { nullptr };
  };
} // end namespace

#endif // __Laberith__BaseApplication__h__

