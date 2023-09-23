#ifndef __PUJ_CGraf__SphericalCamera__h__
#define __PUJ_CGraf__SphericalCamera__h__

#include <PUJ_CGraf/Camera.h>

namespace PUJ_CGraf
{
  /**
   */
  class SphericalCamera
      : public Camera
  {
  public:
    using Self = SphericalCamera;
    using Superclass = Camera;

    using TReal = Superclass::TReal;
    using TNatural = Superclass::TNatural;

  public:
    SphericalCamera();
    virtual ~SphericalCamera() = default;

    virtual void set_bounding_box(const TReal *bb) override;
    virtual void move_radius(float dr) override;
    virtual void move_delta(int dx, int dy) override;
    virtual void lookat() const override;
    virtual void save_state() override;

  protected:
    TReal m_Center[3]{0};
    TReal m_Radius{0};
    TReal m_Phi{0};
    TReal m_Theta{0};
    TReal m_PrevPhi{0};
    TReal m_PrevTheta{0};
  };
} // end namespace

#endif // __PUJ_CGraf__SphericalCamera__h__

// eof - SphericalCamera.h
