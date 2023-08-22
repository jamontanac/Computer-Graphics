
cmake_minimum_required(VERSION 3.10)

project(SolarSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#find_package(Boost REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)


if(APPLE)
  find_package(OpenGL COMPONENTS OpenGL REQUIRED)
  # OpenGL_GL_PREFERENCE can be set to "LEGACY" or "GLVND" i setted to "GLVND" since it is more modern
  find_package(GLUT REQUIRED)
  # Include Boost header directories
  include_directories(${Boost_INCLUDE_DIRS})

  # add_executable(solar_system my_solar_system_mac.cpp)
else()
  set(OpenGL_GL_PREFERENCE "GLVND")
  find_package(OpenGL REQUIRED)
  # OpenGL_GL_PREFERENCE can be set to "LEGACY" or "GLVND" i setted to "GLVND" since it is more modern
  find_package(GLUT REQUIRED)
  # Include Boost header directories
  include_directories(${Boost_INCLUDE_DIRS})

endif()
#add_executable(solar_system_template my_solar_system.cpp)
#add_executable(example_read_json example_read_json.cpp)

add_executable(solar_system my_solar_system.cpp)
# For debugging purposes: print the Boost libraries that CMake has found
message(STATUS "Boost libraries: ${Boost_LIBRARIES}")

if(APPLE)
  target_link_libraries(
    solar_system
    ${Boost_LIBRARIES}
    ${OPENGL_LIBRARIES}
    GLUT::GLUT
  )
else()
  target_link_libraries(
    solar_system
    ${Boost_LIBRARIES}
    OpenGL::OpenGL
    GLUT::GLUT
  )
endif()
