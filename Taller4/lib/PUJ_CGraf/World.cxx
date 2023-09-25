#define GL_SILENCE_DEPRECATION
#include <PUJ_CGraf/World.h>
#include <PUJ_CGraf/Camera.h>
#include <PUJ_CGraf/Light.h>

#include <limits>

#ifdef __APPLE__
#include <OpenGL/gl.h> // Use this when using mac
#include <GLUT/glut.h> // Use this when using mac
#else
#include <GL/gl.h>   //use this when in linux
#include <GL/glut.h> //use this when in linux
#endif
// -------------------------------------------------------------------------
PUJ_CGraf::World::
    World()
    : Superclass()
{
}

// -------------------------------------------------------------------------
void PUJ_CGraf::World::
    init(int shade)
{
  // Bounding box
  if (this->m_Camera != nullptr)
    this->m_Camera->set_bounding_box(this->bounding_box());

  // Lights
  if (this->m_Lights.size() > 0)
  {
    glShadeModel(shade);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    for (unsigned long long i = 0; i < this->m_Lights.size(); ++i)
    {
      glEnable(GL_LIGHT0 + i);
      this->m_Lights[i]->load(GL_LIGHT0 + i);
    } // end for
  }   // end if
}

// -------------------------------------------------------------------------
void PUJ_CGraf::World::
    set_camera(PUJ_CGraf::Camera *c)
{
  this->m_Camera = c;
}

// -------------------------------------------------------------------------
PUJ_CGraf::
    Camera *
    PUJ_CGraf::World::
        camera()
{
  return (this->m_Camera);
}

// -------------------------------------------------------------------------
const PUJ_CGraf::
    Camera *
    PUJ_CGraf::World::
        camera() const
{
  return (this->m_Camera);
}

// -------------------------------------------------------------------------
void PUJ_CGraf::World::
    set_viewport(int w, int h)
{
  if (this->m_Camera != nullptr)
    this->m_Camera->viewport(w, h);
}

// -------------------------------------------------------------------------
void PUJ_CGraf::World::
    add_light(PUJ_CGraf::Light *l)
{
  this->m_Lights.push_back(l);
}

// -------------------------------------------------------------------------
const PUJ_CGraf::World::
    TReal *
    PUJ_CGraf::World::
        bounding_box()
{
  this->m_BBox[0] = this->m_BBox[1] = this->m_BBox[2] = std::numeric_limits<TReal>::max();
  this->m_BBox[3] = this->m_BBox[4] = this->m_BBox[5] = std::numeric_limits<TReal>::lowest();

  decltype(this->m_Children)::const_iterator d;
  for (d = this->m_Children.begin(); d != this->m_Children.end(); ++d)
  {
    const TReal *b = (*d)->bounding_box();
    for (unsigned int i = 0; i < 3; ++i)
      this->m_BBox[i] =
          (b[i] < this->m_BBox[i]) ? b[i] : this->m_BBox[i];
    for (unsigned int i = 3; i < 6; ++i)
      this->m_BBox[i] =
          (this->m_BBox[i] < b[i]) ? b[i] : this->m_BBox[i];
  } // end for

  return (this->m_BBox);
}

// -------------------------------------------------------------------------
void PUJ_CGraf::World::
    set_texture(
        const unsigned char *buffer,
        const unsigned int &w, const unsigned int &h)
{
  // Do nothing
}

// -------------------------------------------------------------------------
void PUJ_CGraf::World::
    draw() const
{
  // Apply camera
  if (this->m_Camera != nullptr)
    this->m_Camera->lookat();

  // Draw objects
  decltype(this->m_Children)::const_iterator d;
  for (d = this->m_Children.begin(); d != this->m_Children.end(); ++d)
    (*d)->draw();
}

// eof - World.cxx
