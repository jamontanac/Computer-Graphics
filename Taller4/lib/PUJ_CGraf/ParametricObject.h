#ifndef __PUJ_CGraf__ParametricObject__h__
#define __PUJ_CGraf__ParametricObject__h__

#include <PUJ_CGraf/Object.h>

namespace PUJ_CGraf
{
  /**
   */
  class ParametricObject
      : public Object
  {
  public:
    using Self = ParametricObject;
    using Superclass = Object;
    using TReal = Superclass::TReal;
    using TNatural = Superclass::TNatural;
    using VReal = Superclass::VReal;
    using VNatural = Superclass::VNatural;

  public:
    ParametricObject();
    virtual ~ParametricObject() = default;

    virtual void sample(const TNatural &su, const TNatural &sv);

    virtual void point(TReal *p, const TReal &u, const TReal &v) const = 0;
    virtual void normal(TReal *n, const TReal &u, const TReal &v) const = 0;

  protected:
    TReal m_URange[2]{0};
    TReal m_VRange[2]{0};
    bool m_UClosed{false};
    bool m_VClosed{false};
  };
} // end namespace

#endif // __PUJ_CGraf__ParametricObject__h__

// eof - ParametricObject.h
