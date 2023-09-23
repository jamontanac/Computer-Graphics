#ifndef __PUJ_CGraf__Light__h__
#define __PUJ_CGraf__Light__h__

namespace PUJ_CGraf
{
  /**
   */
  class Light
  {
  public:
    using TReal = float;
    using TNatural = unsigned short;

  public:
    Light();
    virtual ~Light() = default;

    virtual void load(const unsigned long long &id) = 0;
  };
} // end namespace

#endif // __PUJ_CGraf__Light__h__

// eof - Light.h
