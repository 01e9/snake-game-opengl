cmake_minimum_required(VERSION 4.0)

project(snake_game_opengl LANGUAGES CXX)

include(GNUInstallDirs)

include(cmake/external-projects/define.cmake)
if(NOT EXISTS ${EXTERNAL_PROJECTS_INSTALL_DIR})
    message(AUTHOR_WARNING "Build the `external_all` target then reload CMake.")
    return()
endif()
include(cmake/external-projects/require.cmake)

add_executable(${PROJECT_NAME}
    src/object/Board.cpp
    src/object/Snake.cpp
    src/object/Treat.cpp
    src/scene/Main.cpp
    src/util/ShaderProgram.cpp
    src/util/Cube.cpp
    src/main.cpp
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
target_include_directories(${PROJECT_NAME}
    PRIVATE $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
    PRIVATE $<BUILD_INTERFACE:${EXTERNAL_PROJECTS_INSTALL_DIR}/${CMAKE_INSTALL_INCLUDEDIR}>
)

set(COMMON_LIBS glad glfw3 Boost::thread)
if (WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${COMMON_LIBS} opengl32)
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE ${COMMON_LIBS} GL dl pthread X11 Xrandr Xinerama Xcursor Xxf86vm)
endif()
