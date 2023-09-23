include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  )

set(
  _src
  PUJ_CGraf/AmbientLight.cxx
  PUJ_CGraf/Camera.cxx
  PUJ_CGraf/Controller.cxx
  PUJ_CGraf/Cylinder.cxx
  PUJ_CGraf/Light.cxx
  PUJ_CGraf/Object.cxx
  PUJ_CGraf/Plane.cxx
  PUJ_CGraf/ParametricObject.cxx
  PUJ_CGraf/PPMImage.cxx
  PUJ_CGraf/Sphere.cxx
  PUJ_CGraf/SphericalCamera.cxx
  PUJ_CGraf/SpotLight.cxx
  PUJ_CGraf/World.cxx
  )
set(
  _hdr
  PUJ_CGraf/AmbientLight.h
  PUJ_CGraf/Camera.h
  PUJ_CGraf/Controller.h
  PUJ_CGraf/Cylinder.h
  PUJ_CGraf/Light.h
  PUJ_CGraf/Object.h
  PUJ_CGraf/Plane.h
  PUJ_CGraf/ParametricObject.h
  PUJ_CGraf/PPMImage.h
  PUJ_CGraf/Sphere.h
  PUJ_CGraf/SphericalCamera.h
  PUJ_CGraf/SpotLight.h
  PUJ_CGraf/World.h
  )

add_library(PUJ_CGraf STATIC ${_hdr} ${_src})

if(APPLE)
  find_library(OpenGL_LIBRARY OpenGL)
  find_library(GLUT_LIBRARY GLUT)
  target_link_libraries(
    PUJ_CGraf
    ${Boost_LIBRARIES}
    ${OpenGL_LIBRARY}
    ${GLUT_LIBRARY}
    )
else()
  target_link_libraries(
    PUJ_CGraf
    ${Boost_LIBRARIES}
    OpenGL::OpenGL
    OpenGL::GLU
    GLUT::GLUT
    )
endif()