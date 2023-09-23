#ifndef __PUJ_CGraf__SpotLight__h__
#define __PUJ_CGraf__SpotLight__h__

#include <PUJ_CGraf/Light.h>

namespace PUJ_CGraf
{
  /**
   */
  class SpotLight
      : public Light
  {
  public:
    using Self = SpotLight;
    using Superclass = Light;

    using TReal = Superclass::TReal;
    using TNatural = Superclass::TNatural;

  public:
    SpotLight();
    virtual ~SpotLight() = default;

    void set_position(const TReal &x, const TReal &y, const TReal &z);
    void set_color(
        const TReal &r, const TReal &g, const TReal &b, const TReal &a = 1);

    virtual void load(const unsigned long long &id) override;

  protected:
    TReal m_Position[4]{0};
    TReal m_Diffuse[4]{0};
    TReal m_Specular[4]{0};
    TReal m_CutOff{180};
  };
} // end namespace

#endif // __PUJ_CGraf__SpotLight__h__

// eof - SpotLight.h
