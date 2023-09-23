#ifndef __PUJ_CGraf__Camera__h__
#define __PUJ_CGraf__Camera__h__

namespace PUJ_CGraf
{
  /**
   */
  class Camera
  {
  public:
    using TReal = float;
    using TNatural = unsigned short;

  public:
    Camera();
    virtual ~Camera() = default;

    virtual void set_bounding_box(const TReal *bb);
    virtual void viewport(int w, int h);

    virtual void move_radius(float dr) = 0;
    virtual void move_delta(int dx, int dy) = 0;
    virtual void lookat() const = 0;
    virtual void save_state() = 0;

  protected:
    TReal m_BBox[6]{0};
    int m_ViewPort[4]{0};
    TReal m_Fovy{45};
    float m_Z{0};
  };
} // end namespace

#endif // __PUJ_CGraf__Camera__h__

// eof - Camera.h
