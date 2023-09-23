#ifndef __PUJ_CGraf__AmbientLight__h__
#define __PUJ_CGraf__AmbientLight__h__

#include <PUJ_CGraf/Light.h>

namespace PUJ_CGraf
{
  /**
   */
  class AmbientLight
      : public Light
  {
  public:
    using Self = AmbientLight;
    using Superclass = Light;

    using TReal = Superclass::TReal;
    using TNatural = Superclass::TNatural;

  public:
    AmbientLight();
    virtual ~AmbientLight() = default;

    void set_color(
        const TReal &r, const TReal &g, const TReal &b, const TReal &a = 1);

    virtual void load(const unsigned long long &id) override;

  protected:
    TReal m_Color[4]{0};
  };
} // end namespace

#endif // __PUJ_CGraf__AmbientLight__h__

// eof - AmbientLight.h
