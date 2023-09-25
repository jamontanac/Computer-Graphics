#ifndef __PUJ_CGraf__Object__h__
#define __PUJ_CGraf__Object__h__

#include <map>
#include <string>
#include <vector>

namespace PUJ_CGraf
{
  /**
   */
  class Object
  {
  public:
    using TReal = float;
    using TNatural = unsigned short;
    using TId = unsigned int;

    using VReal = std::vector<TReal>;
    using VNatural = std::vector<TNatural>;

  public:
    Object();
    virtual ~Object();

    virtual const TReal *bounding_box();

    virtual void set_texture(
        const unsigned char *buffer,
        const unsigned int &w, const unsigned int &h);

    void set_position(const TReal &x, const TReal &y, const TReal &z);
    void add_child(
        Object *c,
        const TReal &x = 0, const TReal &y = 0, const TReal &z = 0);
    Object *child(const unsigned int &i);
    const Object *child(const unsigned int &i) const;
    unsigned int number_of_children() const;

    virtual void draw() const;
    virtual void rotate(float da, float x, float y, float z)
    {
      this->m_Rotation[0] += da;
      this->m_Rotation[1] = x;
      this->m_Rotation[2] = y;
      this->m_Rotation[3] = z;
    }
    virtual void keypressed(unsigned char k) {}
    // virtual void read_Json(const std::string &in_fname) {}

  protected:
    void _update_bounding_box();
    virtual void _synch_buffers();

  protected:
    // Geometry
    TReal m_Position[3]{0};
    TReal m_Rotation[4]{0};
    TReal m_BBox[6]{0};
    VReal m_Points;
    VReal m_PointNormals;

    // Topology: size vs indices
    std::map<TNatural, VNatural> m_Cells;

    // Texture
    VReal m_TexCoords;
    std::vector<unsigned char> m_TexBuffer;
    unsigned int m_TexWidth{0};
    unsigned int m_TexHeight{0};

    // Children
    std::vector<Object *> m_Children;
  };
} // end namespace

#endif // __PUJ_CGraf__Object__h__

// eof - Object.h
