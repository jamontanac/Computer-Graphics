#ifndef __PUJ_CGraf__PPMImage__h__
#define __PUJ_CGraf__PPMImage__h__

#include <string>
#include <vector>

namespace PUJ_CGraf
{
  /**
   */
  class PPMImage
  {
  public:
    PPMImage();
    virtual ~PPMImage() = default;

    void read(const std::string &fname);

    const unsigned int &width() const;
    const unsigned int &height() const;
    const unsigned char *buffer() const;

  protected:
    unsigned int m_Width{0};
    unsigned int m_Height{0};
    std::vector<unsigned char> m_Buffer;
  };
} // end namespace

#endif // __PUJ_CGraf__PPMImage__h__

// eof - PPMImage.h
