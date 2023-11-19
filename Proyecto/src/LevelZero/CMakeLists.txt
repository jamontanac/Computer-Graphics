
subdirs(resources)

include_directories(
  ${PROJECT_BINARY_DIR}/lib
  ${PROJECT_SOURCE_DIR}/lib
  )

set(_t)
if(WIN32)
  set(_t WIN32)
elseif(APPLE)
  set(_t MACOSX_BUNDLE)
endif()

add_executable(LevelZero ${_t} LevelZero.cxx)
target_link_libraries(LevelZero Laberith)