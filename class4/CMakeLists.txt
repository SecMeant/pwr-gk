project(gl)

set(CXX_SOURCES src/main.cc)
add_executable(c3 ${CXX_SOURCES})
target_link_libraries(c3 glut GL GLU fmt)
set_property(TARGET c3 PROPERTY CXX_STANDARD 17)
