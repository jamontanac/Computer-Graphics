
include_directories(
  ${BULLET_INCLUDE_DIRS}
  ${PROJECT_BINARY_DIR}/lib
  ${PROJECT_SOURCE_DIR}/lib
  )
add_library(
  Laberith SHARED
  BaseApplication.cxx BaseApplication_VTK.cxx BaseApplication.h
  CameraMan.cxx CameraMan.h
  )
generate_export_header(
  Laberith
  EXPORT_MACRO_NAME Laberith_EXPORT
  EXPORT_FILE_NAME Export.h
  )
target_link_libraries(
  Laberith
  ${VTK_LIBRARIES}
  ${BULLET_LIBRARIES}
  OgreMain OgreBites
  )

