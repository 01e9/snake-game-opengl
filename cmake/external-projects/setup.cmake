include("${CMAKE_CURRENT_LIST_DIR}/define.vars.cmake")

set(CMAKE_FIND_ROOT_PATH "${EXT_PROJ_INSTALL_DIR};${CMAKE_FIND_ROOT_PATH}")
link_directories("${EXT_PROJ_LIB_DIR}")

include("${CMAKE_CURRENT_LIST_DIR}/define.libs.cmake")

set(EXT_PROJ_READY FALSE)
if(NOT IS_DIRECTORY "${EXT_PROJ_INSTALL_DIR}")
    message(AUTHOR_WARNING "Build the `external_all` target then reload CMake.")
    # `message(*_ERROR)` will case invalid CMake config and build will fail
else()
    set(EXT_PROJ_READY TRUE)
    include("${CMAKE_CURRENT_LIST_DIR}/require.libs.cmake")
endif()
