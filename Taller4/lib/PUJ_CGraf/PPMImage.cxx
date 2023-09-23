
#include <PUJ_CGraf/PPMImage.h>
#include <fstream>
#include <sstream>

// -------------------------------------------------------------------------
PUJ_CGraf::PPMImage::
    PPMImage()
{
}

// -------------------------------------------------------------------------
void PUJ_CGraf::PPMImage::
    read(const std::string &fname)
{
  // Read file buffer into memory
  std::ifstream in_file(fname.c_str());
  std::istringstream in_str(
      std::string(std::istreambuf_iterator<char>{in_file}, {}));
  in_file.close();

  // Read header
  std::string type;
  in_str >> type;
  in_str >> this->m_Width >> this->m_Height;
  this->m_Buffer.clear();
  this->m_Buffer.resize(this->m_Width * this->m_Height * 3, 0);
  this->m_Buffer.shrink_to_fit();

  // Read data
  for (unsigned char &v : this->m_Buffer)
  {
    unsigned int x;
    in_str >> x;
    v = (unsigned char)(x);
  } // end for
}

// -------------------------------------------------------------------------
const unsigned int &PUJ_CGraf::PPMImage::
    width() const
{
  return (this->m_Width);
}

// -------------------------------------------------------------------------
const unsigned int &PUJ_CGraf::PPMImage::
    height() const
{
  return (this->m_Height);
}

// -------------------------------------------------------------------------
const unsigned char *PUJ_CGraf::PPMImage::
    buffer() const
{
  return (this->m_Buffer.data());
}

// eof - PPMImage.cxx
