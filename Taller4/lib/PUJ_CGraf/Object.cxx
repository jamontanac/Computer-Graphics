#define GL_SILENCE_DEPRECATION
#include <PUJ_CGraf/Object.h>

#include <cstring>
#include <limits>

#ifdef __APPLE__
#include <OpenGL/gl.h> // Use this when using mac
#include <GLUT/glut.h> // Use this when using mac
#else
#include <GL/gl.h>   //use this when in linux
#include <GL/glut.h> //use this when in linux
#endif
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
PUJ_CGraf::Object::
    Object()
{
}

// -------------------------------------------------------------------------
PUJ_CGraf::Object::
    ~Object()
{
}

// -------------------------------------------------------------------------
const PUJ_CGraf::Object::
    TReal *
    PUJ_CGraf::Object::
        bounding_box()
{
  return (this->m_BBox);
}

// -------------------------------------------------------------------------
void PUJ_CGraf::Object::
    set_texture(
        const unsigned char *buffer,
        const unsigned int &w, const unsigned int &h)
{
  this->m_TexBuffer.clear();
  this->m_TexBuffer.resize(w * h * 3);
  this->m_TexBuffer.shrink_to_fit();
  std::memcpy(
      this->m_TexBuffer.data(), buffer, w * h * 3 * sizeof(unsigned char));
  this->m_TexWidth = w;
  this->m_TexHeight = h;
}

// -------------------------------------------------------------------------
void PUJ_CGraf::Object::
    set_position(const TReal &x, const TReal &y, const TReal &z)
{
  this->m_Position[0] = x;
  this->m_Position[1] = y;
  this->m_Position[2] = z;
}

// -------------------------------------------------------------------------
void PUJ_CGraf::Object::
    add_child(Object *c, const TReal &x, const TReal &y, const TReal &z)
{
  c->set_position(x, y, z);
  this->m_Children.push_back(c);
}

// -------------------------------------------------------------------------
PUJ_CGraf::Object *PUJ_CGraf::Object::
    child(const unsigned int &i)
{
  if (i < this->m_Children.size())
    return (this->m_Children[i]);
  else
    return (nullptr);
}

// -------------------------------------------------------------------------
const PUJ_CGraf::Object *PUJ_CGraf::Object::
    child(const unsigned int &i) const
{
  if (i < this->m_Children.size())
    return (this->m_Children[i]);
  else
    return (nullptr);
}

// -------------------------------------------------------------------------
unsigned int PUJ_CGraf::Object::
    number_of_children() const
{
  return (this->m_Children.size());
}

// -------------------------------------------------------------------------
void PUJ_CGraf::Object::
    draw() const
{
  std::map<TNatural, VNatural>::const_iterator c;
  VNatural::const_iterator v;
  const TReal *p = this->m_Points.data();
  const TReal *pn = this->m_PointNormals.data();

  // Save parent's transform
  glPushMatrix();

  // Position object
  glTranslatef(
      this->m_Position[0],
      this->m_Position[1],
      this->m_Position[2]);
  glRotatef(
      this->m_Rotation[0],
      this->m_Rotation[1],
      this->m_Rotation[2],
      this->m_Rotation[3]);

  for (c = this->m_Cells.begin(); c != this->m_Cells.end(); ++c)
  {
    if (c->first == 1)
      glBegin(GL_POINTS);
    else if (c->first == 2)
      glBegin(GL_LINES);
    else if (c->first == 3)
      glBegin(GL_TRIANGLES);
    else if (c->first == 4)
      glBegin(GL_QUADS);
    for (v = c->second.begin(); v != c->second.end(); ++v)
    {
      if (pn != nullptr)
        glNormal3fv(pn + (*v * 3));
      glVertex3fv(p + (*v * 3));
    } // end for
    glEnd();
  } // end for

  decltype(this->m_Children)::const_iterator d;
  for (auto d = this->m_Children.begin(); d != this->m_Children.end(); ++d)
    (*d)->draw();

  // Get back parent's transform
  glPopMatrix();
}

// -------------------------------------------------------------------------
void PUJ_CGraf::Object::
    _update_bounding_box()
{
  // Update bounding box
  this->m_BBox[0] = this->m_BBox[1] = this->m_BBox[2] = std::numeric_limits<TReal>::max();
  this->m_BBox[3] = this->m_BBox[4] = this->m_BBox[5] = std::numeric_limits<TReal>::lowest();
  for (unsigned int i = 0; i < this->m_Points.size(); i += 3)
  {
    for (unsigned int j = 0; j < 3; ++j)
    {
      if (this->m_Points[i + j] < this->m_BBox[j])
        this->m_BBox[j] = this->m_Points[i + j];
      if (this->m_BBox[j + 3] < this->m_Points[i + j])
        this->m_BBox[j + 3] = this->m_Points[i + j];
    } // end for
  }   // end for
}

// -------------------------------------------------------------------------
void PUJ_CGraf::Object::
    _synch_buffers()
{
}

// eof - Object.cxx
