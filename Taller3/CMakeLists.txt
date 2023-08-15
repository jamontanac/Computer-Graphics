
cmake_minimum_required(VERSION 3.10)

project(SolarSystem)

find_package(Boost REQUIRED)

if(APPLE)
  find_package(OpenGL COMPONENTS OpenGL REQUIRED)
else()
  set(OpenGL_GL_PREFERENCE "GLVND")
  find_package(OpenGL REQUIRED)
endif()
#  OpenGL_GL_PREFERENCE can be set to "LEGACY" or "GLVND" i setted to "GLVND" since it is more modern
#find_package(GLUT REQUIRED)
add_executable(solar_system_template my_solar_system.cpp)
#add_executable(example_read_json example_read_json.cpp)

if(APPLE)
  target_link_libraries(
    solar_system_template
    ${Boost_LIBRARIES}
    ${OPENGL_LIBRARIES}
    GLUT::GLUT
  )
else()
  target_link_libraries(
    solar_system_template
    ${Boost_LIBRARIES}
    OpenGL::OpenGL
    GLUT::GLUT
  )
endif()
