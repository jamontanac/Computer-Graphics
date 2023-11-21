#ifndef __Laberith__CameraMan__h__
#define __Laberith__CameraMan__h__

#include <Laberith/Export.h>

#include <OgreAxisAlignedBox.h>
#include <OgreInput.h>
#include <OgreMath.h>

namespace Ogre { class SceneNode; }

namespace Laberith
{
  /**
   */
  class Laberith_EXPORT CameraMan
    : public OgreBites::InputListener
  {
  public:
    using Self = CameraMan;
    using Superclass = OgreBites::InputListener;

  public:
    CameraMan( Ogre::SceneNode* cam, const Ogre::AxisAlignedBox& bb );
    virtual ~CameraMan( ) = default;

    void setCamera( Ogre::SceneNode* cam );
    Ogre::SceneNode* getCamera( );

    const Ogre::AxisAlignedBox& getBoundingBox( ) const;

    void setTopSpeed( Ogre::Real topSpeed );
    Ogre::Real getTopSpeed( );

    virtual void frameRendered( const Ogre::FrameEvent& evt ) override;
    virtual bool keyPressed( const OgreBites::KeyboardEvent& evt ) override;
    virtual bool keyReleased( const OgreBites::KeyboardEvent& evt ) override;
    virtual bool mouseMoved( const OgreBites::MouseMotionEvent& evt ) override;
    virtual bool mouseWheelRolled( const OgreBites::MouseWheelEvent& evt ) override;
    virtual bool mousePressed( const OgreBites::MouseButtonEvent& evt ) override;
    virtual bool mouseReleased( const OgreBites::MouseButtonEvent& evt ) override;

  protected:
    Ogre::SceneNode* m_Camera { nullptr };

    Ogre::AxisAlignedBox m_BoundingBox;

    bool m_Forward   { false };
    bool m_Backwards { false };
    bool m_Left      { false };
    bool m_Right     { false };
    bool m_Fast      { false };
    bool m_Up        { false };
    bool m_Down      { false };

    Ogre::Real m_Top { 1 };
  };
} // end namespace

#endif // __Laberith__CameraMan__h__

