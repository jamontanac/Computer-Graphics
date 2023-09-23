
#include <PUJ_CGraf/Sphere.h>

#include <cmath>

// -------------------------------------------------------------------------
PUJ_CGraf::Sphere::
    Sphere()
    : Superclass()
{
  this->m_URange[0] = 0;
  this->m_URange[1] = TReal(8) * std::atan(TReal(1));

  this->m_VRange[0] = 0;
  this->m_VRange[1] = TReal(4) * std::atan(TReal(1));

  this->m_UClosed = true;
  this->m_VClosed = false;
}

// -------------------------------------------------------------------------
void PUJ_CGraf::Sphere::
    set_center(const TReal &x, const TReal &y, const TReal &z)
{
  this->m_Center[0] = x;
  this->m_Center[1] = y;
  this->m_Center[2] = z;
}

// -------------------------------------------------------------------------
void PUJ_CGraf::Sphere::
    set_radius(const TReal &r)
{
  this->m_Radius = r;
}

// -------------------------------------------------------------------------
void PUJ_CGraf::Sphere::
    point(TReal *p, const TReal &u, const TReal &v) const
{
  TReal cu = std::cos(u);
  TReal su = std::sin(u);
  TReal cv = std::cos(v);
  TReal sv = std::sin(v);

  p[0] = (this->m_Radius * cu * sv) + this->m_Center[0];
  p[1] = (this->m_Radius * su * sv) + this->m_Center[1];
  p[2] = (this->m_Radius * cv) + this->m_Center[2];
}

// -------------------------------------------------------------------------
void PUJ_CGraf::Sphere::
    normal(TReal *n, const TReal &u, const TReal &v) const
{
  TReal cu = std::cos(u);
  TReal su = std::sin(u);
  TReal cv = std::cos(v);
  TReal sv = std::sin(v);

  n[0] = cu * sv;
  n[1] = su * sv;
  n[2] = cv;
}

// eof - Sphere.cxx
