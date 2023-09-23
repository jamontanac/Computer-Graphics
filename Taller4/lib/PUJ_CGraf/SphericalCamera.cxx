
#include <PUJ_CGraf/SphericalCamera.h>

#include <cmath>
#ifdef __APPLE__
#include <OpenGL/gl.h> // Use this when using mac
#include <GLUT/glut.h> // Use this when using mac
#else
#include <GL/gl.h>   //use this when in linux
#include <GL/glut.h> //use this when in linux
#endif
// -------------------------------------------------------------------------
// #include <GL/gl.h>

// -------------------------------------------------------------------------
PUJ_CGraf::SphericalCamera::
    SphericalCamera()
    : Superclass()
{
}

// -------------------------------------------------------------------------
void PUJ_CGraf::SphericalCamera::
    set_bounding_box(const TReal *bb)
{
  this->Superclass::set_bounding_box(bb);

  this->m_Radius = this->m_Z * 2.0;
  this->m_Center[0] = (bb[3] + bb[0]) * 0.5;
  this->m_Center[1] = (bb[4] + bb[1]) * 0.5;
  this->m_Center[2] = (bb[5] + bb[2]) * 0.5;
}

// -------------------------------------------------------------------------
void PUJ_CGraf::SphericalCamera::
    move_radius(float dr)
{
  this->m_Radius += dr;
}

// -------------------------------------------------------------------------
void PUJ_CGraf::SphericalCamera::
    move_delta(int dx, int dy)
{
  static const TReal _pi = TReal(4) * std::atan(TReal(1));
  static const TReal _2pi = TReal(2) * _pi;

  TReal dt = TReal(this->m_ViewPort[2] - this->m_ViewPort[0]);
  TReal dp = TReal(this->m_ViewPort[3] - this->m_ViewPort[1]);

  this->m_Theta = this->m_PrevTheta + (TReal(dx) / dt);
  this->m_Phi = this->m_PrevPhi - (TReal(dy) / dp);

  this->m_Theta = std::fmod(this->m_Theta, _2pi);
  this->m_Phi = std::fmod(this->m_Phi, _pi);
}

// -------------------------------------------------------------------------
void PUJ_CGraf::SphericalCamera::
    lookat() const
{
  TReal st = std::sin(this->m_Theta);
  TReal ct = std::cos(this->m_Theta);
  TReal sp = std::sin(this->m_Phi);
  TReal cp = std::cos(this->m_Phi);

  TReal M[16] =
      {
          st,
          ct * cp,
          ct * sp,
          0,
          -ct,
          st * cp,
          st * sp,
          0,
          0,
          -sp,
          cp,
          0,
          (this->m_Center[1] * ct) - (this->m_Center[0] * st),
          (this->m_Center[2] * sp) - (this->m_Center[1] * st * cp) - (this->m_Center[0] * ct * cp),
          -this->m_Radius - (this->m_Center[0] * ct * sp) - (this->m_Center[1] * st * sp) - (this->m_Center[2] * cp),
          1};
  glMultMatrixf(M);
}

// -------------------------------------------------------------------------
void PUJ_CGraf::SphericalCamera::
    save_state()
{
  this->m_PrevTheta = this->m_Theta;
  this->m_PrevPhi = this->m_Phi;
}

// eof - SphericalCamera.cxx
