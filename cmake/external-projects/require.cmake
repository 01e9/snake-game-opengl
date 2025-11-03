set(CMAKE_FIND_ROOT_PATH "${EXTERNAL_PROJECTS_INSTALL_DIR};${CMAKE_FIND_ROOT_PATH}")

find_package(Microsoft.GSL CONFIG REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.89 REQUIRED COMPONENTS thread)
