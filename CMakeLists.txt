project(gl)

set(CLASS1_CXX_SOURCES class1/src/main.cc)
add_executable(c1 ${CLASS1_CXX_SOURCES})
target_link_libraries(c1 glut GL fmt)

set(CLASS1_CXX_SOURCES class2/src/main.cc)
add_executable(c2 ${CLASS1_CXX_SOURCES})
target_link_libraries(c2 glut GL fmt)
set_property(TARGET c2 PROPERTY CXX_STANDARD 17)
