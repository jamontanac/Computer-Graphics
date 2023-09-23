
#include <PUJ_CGraf/ParametricObject.h>

#include <cstring>

// -------------------------------------------------------------------------
PUJ_CGraf::ParametricObject::
    ParametricObject()
    : Superclass()
{
}

// -------------------------------------------------------------------------
void PUJ_CGraf::ParametricObject::
    sample(const TNatural &su, const TNatural &sv)
{
  this->m_Points.clear();
  this->m_PointNormals.clear();

  this->m_Cells.clear();
  std::map<TNatural, VNatural>::iterator triangles =
      this->m_Cells.insert(std::make_pair(3, VNatural())).first;

  for (TNatural iv = 0; iv < sv; ++iv)
  {
    TReal tv = TReal(iv) / TReal(sv - ((this->m_VClosed) ? 0 : 1));
    TReal v = tv * (this->m_VRange[1] - this->m_VRange[0]);
    v += this->m_VRange[0];
    for (TNatural iu = 0; iu < su; ++iu)
    {
      TReal tu = TReal(iu) / TReal(su - ((this->m_UClosed) ? 0 : 1));
      TReal u = tu * (this->m_URange[1] - this->m_URange[0]);
      u += this->m_URange[0];

      // Point and its normal
      unsigned long long i = this->m_Points.size();
      this->m_Points.push_back(0);
      this->m_Points.push_back(0);
      this->m_Points.push_back(0);
      this->m_PointNormals.push_back(0);
      this->m_PointNormals.push_back(0);
      this->m_PointNormals.push_back(0);
      this->point(this->m_Points.data() + i, u, v);
      this->normal(this->m_PointNormals.data() + i, u, v);

      // Textures
      this->m_TexCoords.push_back(tu);
      this->m_TexCoords.push_back(tv);

      // Simple triangles
      unsigned long long j = i / 3;
      if (iu < su - 1 && iv < sv - 1)
      {
        triangles->second.push_back(j);
        triangles->second.push_back(j + 1);
        triangles->second.push_back(j + su);

        triangles->second.push_back(j + 1);
        triangles->second.push_back(j + su + 1);
        triangles->second.push_back(j + su);
      } // end if

      // Border triangles
      if (iu == su - 1 && this->m_UClosed && iv < sv - 1)
      {
        triangles->second.push_back(j);
        triangles->second.push_back(((j + 1) % su) + (su * iv));
        triangles->second.push_back(j + su);

        triangles->second.push_back(((j + 1) % su) + (su * iv));
        triangles->second.push_back(((j + 1) % su) + (su * (iv + 1)));
        triangles->second.push_back(j + su);
      } // end if
    }   // end for
  }     // end for

  this->m_Points.shrink_to_fit();
  this->m_PointNormals.shrink_to_fit();
  triangles->second.shrink_to_fit();

  this->_update_bounding_box();
  this->_synch_buffers();
}

// eof - ParametricObject.cxx
