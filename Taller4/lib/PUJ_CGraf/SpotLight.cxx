
#include <PUJ_CGraf/SpotLight.h>

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
PUJ_CGraf::SpotLight::
    SpotLight()
    : Superclass()
{
}

// -------------------------------------------------------------------------
void PUJ_CGraf::SpotLight::
    set_position(const TReal &x, const TReal &y, const TReal &z)
{
  this->m_Position[0] = x;
  this->m_Position[1] = y;
  this->m_Position[2] = z;
  this->m_Position[3] = 0;
}

// -------------------------------------------------------------------------
void PUJ_CGraf::SpotLight::
    set_color(const TReal &r, const TReal &g, const TReal &b, const TReal &a)
{
  this->m_Diffuse[0] = this->m_Specular[0] = r;
  this->m_Diffuse[1] = this->m_Specular[1] = g;
  this->m_Diffuse[2] = this->m_Specular[2] = b;
  this->m_Diffuse[3] = this->m_Specular[3] = a;
}

// -------------------------------------------------------------------------
void PUJ_CGraf::SpotLight::
    load(const unsigned long long &id)
{
  glLightfv(id, GL_POSITION, this->m_Position);
  glLightfv(id, GL_DIFFUSE, this->m_Diffuse);
  glLightfv(id, GL_SPECULAR, this->m_Specular);
  glLightfv(id, GL_SPOT_CUTOFF, &this->m_CutOff);
}

// eof - SpotLight.cxx
