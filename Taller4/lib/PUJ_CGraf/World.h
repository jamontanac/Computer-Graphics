#ifndef __PUJ_CGraf__World__h__
#define __PUJ_CGraf__World__h__

#include <PUJ_CGraf/Object.h>

// -- FORWARD DECLARATIONS --
namespace PUJ_CGraf
{
  class Light;
}
namespace PUJ_CGraf
{
  class Camera;
}

namespace PUJ_CGraf
{
  /**
   */
  class World final
      : public Object
  {
  public:
    using Self = World;
    using Superclass = Object;
    using TReal = Superclass::TReal;
    using TNatural = Superclass::TNatural;
    using VReal = Superclass::VReal;
    using VNatural = Superclass::VNatural;

  public:
    World();
    virtual ~World() = default;

    void init(int shade);

    void set_camera(Camera *c);
    Camera *camera();
    const Camera *camera() const;
    void set_viewport(int w, int h);

    void add_light(Light *l);

    virtual const TReal *bounding_box() override;
    virtual void set_texture(
        const unsigned char *buffer,
        const unsigned int &w, const unsigned int &h) override;
    virtual void draw() const override;
    virtual void keypressed(unsigned char k) override
    {
      for (auto *c : this->m_Children)
      {
        c->keypressed(k);
      }
    }

  protected:
    Camera *m_Camera{nullptr};
    std::vector<Light *> m_Lights;
  };
} // end namespace

#endif // __PUJ_CGraf__World__h__

// eof - World.h
