## -------------------------------------------------------------------------
## @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
## -------------------------------------------------------------------------

include_directories(
  ${PROJECT_BINARY_DIR}/lib
  ${PROJECT_SOURCE_DIR}/lib
  )

add_executable(Stickman Stickman.cxx)
target_link_libraries(Stickman PUJ_CGraf)

## eof - CMakeLists.txt
