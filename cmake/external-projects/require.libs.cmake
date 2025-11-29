find_package(Microsoft.GSL CONFIG REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.89 REQUIRED COMPONENTS thread)
