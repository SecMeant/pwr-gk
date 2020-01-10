project(gl)

set(CXX_SOURCES src/main.cc)
add_executable(c5 ${CXX_SOURCES})
target_link_libraries(c5 glut GL GLU fmt)
set_property(TARGET c5 PROPERTY CXX_STANDARD 17)
