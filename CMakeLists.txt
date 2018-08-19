cmake_minimum_required(VERSION 3.5)

project(snake_game_opengl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(EXTERNAL_PROJECT_VERSION_GLFW 3.2.1)
set(EXTERNAL_PROJECT_VERSION_GLM 0.9.9.0)
set(EXTERNAL_PROJECT_VERSION_GSL v1.0.0)
include(ExternalProjects.cmake)

include(GNUInstallDirs)
link_directories(${EXTERNAL_PROJECTS_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})

add_executable(${PROJECT_NAME}
        glad/src/glad.c
        src/object/Board.cpp
        src/object/Snake.cpp
        src/object/Treat.cpp
        src/scene/Main.cpp
        src/util/ShaderProgram.cpp
        src/util/Cube.cpp
        src/main.cpp)
target_include_directories(${PROJECT_NAME}
        PRIVATE $<BUILD_INTERFACE:${EXTERNAL_PROJECTS_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}>
        PRIVATE $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/glad/include>
        PRIVATE $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>)
target_link_libraries(${PROJECT_NAME} GL glfw3 dl pthread X11 Xrandr Xinerama Xcursor Xxf86vm)

