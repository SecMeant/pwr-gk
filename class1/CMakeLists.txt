project(gl)

set(CLASS1_CXX_SOURCES src/main.cc)
add_executable(c1 ${CLASS1_CXX_SOURCES})
target_link_libraries(c1 glut GL fmt)
