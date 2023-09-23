
#include <PUJ_CGraf/Cylinder.h>

#include <cmath>

// -------------------------------------------------------------------------
PUJ_CGraf::Cylinder::
    Cylinder()
    : Superclass()
{
  this->m_URange[0] = 0;
  this->m_URange[1] = TReal(8) * std::atan(TReal(1));

  this->m_VRange[0] = 0;
  this->m_VRange[1] = 1;

  this->m_UClosed = true;
  this->m_VClosed = false;
}

// -------------------------------------------------------------------------
void PUJ_CGraf::Cylinder::
    set_center(const TReal &x, const TReal &y, const TReal &z)
{
  this->m_Center[0] = x;
  this->m_Center[1] = y;
  this->m_Center[2] = z;
}

// -------------------------------------------------------------------------
void PUJ_CGraf::Cylinder::
    set_radius(const TReal &r)
{
  this->m_Radius = r;
}

// -------------------------------------------------------------------------
void PUJ_CGraf::Cylinder::
    set_height(const TReal &h)
{
  this->m_Height = h;
  this->m_VRange[0] = 0;
  this->m_VRange[1] = h;
}

// -------------------------------------------------------------------------
void PUJ_CGraf::Cylinder::
    point(TReal *p, const TReal &u, const TReal &v) const
{
  p[0] = (this->m_Radius * std::cos(u)) + this->m_Center[0];
  p[1] = (this->m_Radius * std::sin(u)) + this->m_Center[1];
  p[2] = v + this->m_Center[2];
}

// -------------------------------------------------------------------------
void PUJ_CGraf::Cylinder::
    normal(TReal *n, const TReal &u, const TReal &v) const
{
  n[0] = std::cos(u);
  n[1] = std::sin(u);
  n[2] = TReal(0);
}

// eof - Cylinder.cxx
