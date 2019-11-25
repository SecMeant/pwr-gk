project(gl)

set(CLASS2_CXX_SOURCES src/main.cc)
add_executable(c2 ${CLASS2_CXX_SOURCES})
target_link_libraries(c2 glut GL fmt)
set_property(TARGET c2 PROPERTY CXX_STANDARD 17)
