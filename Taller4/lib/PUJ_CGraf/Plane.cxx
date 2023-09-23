
#include <PUJ_CGraf/Plane.h>

// -------------------------------------------------------------------------
PUJ_CGraf::Plane::
    Plane()
    : Superclass()
{
  this->m_URange[0] = 0;
  this->m_URange[1] = 1;

  this->m_VRange[0] = 0;
  this->m_VRange[1] = 1;

  this->m_UClosed = false;
  this->m_VClosed = false;
}

// -------------------------------------------------------------------------
void PUJ_CGraf::Plane::
    set_center(const TReal &x, const TReal &y, const TReal &z)
{
  this->m_Center[0] = x;
  this->m_Center[1] = y;
  this->m_Center[2] = z;
}

// -------------------------------------------------------------------------
void PUJ_CGraf::Plane::
    set_width(const TReal &w)
{
  this->m_Width = w;
  this->m_URange[0] = 0;
  this->m_URange[1] = w;
}

// -------------------------------------------------------------------------
void PUJ_CGraf::Plane::
    set_height(const TReal &h)
{
  this->m_Height = h;
  this->m_VRange[0] = 0;
  this->m_VRange[1] = h;
}

// -------------------------------------------------------------------------
void PUJ_CGraf::Plane::
    point(TReal *p, const TReal &u, const TReal &v) const
{
  p[0] = u + this->m_Center[0];
  p[1] = v + this->m_Center[1];
  p[2] = 0;
}

// -------------------------------------------------------------------------
void PUJ_CGraf::Plane::
    normal(TReal *n, const TReal &u, const TReal &v) const
{
  n[0] = 0;
  n[1] = 0;
  n[2] = 1;
}

// eof - Plane.cxx
