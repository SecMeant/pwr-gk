project(gl)

set(CXX_SOURCES main.cc)

add_executable(${PROJECT_NAME} ${CXX_SOURCES})

target_link_libraries(${PROJECT_NAME} glut GL fmt)

