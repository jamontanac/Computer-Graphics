#include <PUJ_CGraf/AmbientLight.h>

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
PUJ_CGraf::AmbientLight::
    AmbientLight()
    : Superclass()
{
}

// -------------------------------------------------------------------------
void PUJ_CGraf::AmbientLight::
    set_color(const TReal &r, const TReal &g, const TReal &b, const TReal &a)
{
  this->m_Color[0] = r;
  this->m_Color[1] = g;
  this->m_Color[2] = b;
  this->m_Color[3] = a;
}

// -------------------------------------------------------------------------
void PUJ_CGraf::AmbientLight::
    load(const unsigned long long &id)
{
  glLightfv(id, GL_AMBIENT, this->m_Color);
}

// eof - AmbientLight.cxx
