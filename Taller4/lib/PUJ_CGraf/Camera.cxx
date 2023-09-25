#define GL_SILENCE_DEPRECATION
#include <PUJ_CGraf/Camera.h>

#include <cmath>
#include <cstring>

#ifdef __APPLE__
#include <OpenGL/gl.h> // Use this when using mac
#include <GLUT/glut.h> // Use this when using mac
#else
#include <GL/gl.h>   //use this when in linux
#include <GL/glut.h> //use this when in linux
#endif
// -------------------------------------------------------------------------
// #include <GL/gl.h>
// #include <GL/glu.h>

// -------------------------------------------------------------------------
PUJ_CGraf::Camera::
    Camera()
{
}

// -------------------------------------------------------------------------
void PUJ_CGraf::Camera::
    set_bounding_box(const TReal *bb)
{
  std::memcpy(this->m_BBox, bb, sizeof(TReal) * 6);

  this->m_Z = (bb[3] - bb[0]) * (bb[3] - bb[0]);
  this->m_Z += (bb[4] - bb[1]) * (bb[4] - bb[1]);
  this->m_Z += (bb[5] - bb[2]) * (bb[5] - bb[2]);
  this->m_Z = std::sqrt(this->m_Z);
}

// -------------------------------------------------------------------------
void PUJ_CGraf::Camera::
    viewport(int w, int h)
{
  // Set the viewport to cover the new window size
  this->m_ViewPort[0] = 0;
  this->m_ViewPort[1] = 0;
  this->m_ViewPort[2] = w;
  this->m_ViewPort[3] = h;
  glViewport(
      this->m_ViewPort[0],
      this->m_ViewPort[1],
      this->m_ViewPort[2],
      this->m_ViewPort[3]);

  // Reset world-to-viewport transformation
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  TReal a = TReal(this->m_ViewPort[2] - this->m_ViewPort[0]);
  TReal d = TReal(this->m_ViewPort[3] - this->m_ViewPort[1]);
  a /= (this->m_ViewPort[3] != this->m_ViewPort[1]) ? d : TReal(1);
  gluPerspective(this->m_Fovy, a, this->m_Z * 1e-3, this->m_Z * 100);
}

// eof - Camera.cxx
