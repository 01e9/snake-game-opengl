cmake_minimum_required(VERSION 4.0)

project(snake_game_opengl)

include(GNUInstallDirs)

include(cmake/external-projects/define.cmake)
if(NOT EXISTS ${EXTERNAL_PROJECTS_INSTALL_DIR})
    message(AUTHOR_WARNING "Build the `external_all` target then reload CMake.")
    return()
endif()
include(cmake/external-projects/require.cmake)

add_executable(${PROJECT_NAME}
    glad/src/glad.c
    src/object/Board.cpp
    src/object/Snake.cpp
    src/object/Treat.cpp
    src/scene/Main.cpp
    src/util/ShaderProgram.cpp
    src/util/Cube.cpp
    src/main.cpp
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
target_include_directories(${PROJECT_NAME}
    PRIVATE $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
    PRIVATE $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/glad/include>
    PRIVATE $<BUILD_INTERFACE:${EXTERNAL_PROJECTS_INSTALL_DIR}/${CMAKE_INSTALL_INCLUDEDIR}>
)
if (WIN32)
    target_link_libraries(${PROJECT_NAME} opengl32 glfw3)
else()
    target_link_libraries(${PROJECT_NAME} GL glfw3 dl pthread X11 Xrandr Xinerama Xcursor Xxf86vm)
endif()
