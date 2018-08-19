include(ExternalProject)

set(EXTERNAL_PROJECTS_PREFIX ${CMAKE_BINARY_DIR}/lib)
set(EXTERNAL_PROJECTS_INSTALL_PREFIX ${EXTERNAL_PROJECTS_PREFIX}/installed)

add_custom_target(external_all)
set_target_properties(external_all PROPERTIES EXCLUDE_FROM_ALL TRUE)

ExternalProject_Add(external_glfw
        URL https://github.com/glfw/glfw/archive/${EXTERNAL_PROJECT_VERSION_GLFW}.tar.gz
        PREFIX ${EXTERNAL_PROJECTS_PREFIX}
        CMAKE_ARGS
            -DCMAKE_INSTALL_PREFIX=${EXTERNAL_PROJECTS_INSTALL_PREFIX}
            -DBUILD_SHARED_LIBS=OFF
            -DGLFW_BUILD_EXAMPLES=OFF
            -DGLFW_BUILD_TESTS=OFF
            -DGLFW_BUILD_DOCS=OFF
            -DGLFW_INSTALL=ON
        )
set_target_properties(external_glfw PROPERTIES EXCLUDE_FROM_ALL TRUE)
add_dependencies(external_all external_glfw)

ExternalProject_Add(external_glm
        URL https://github.com/g-truc/glm/archive/${EXTERNAL_PROJECT_VERSION_GLM}.tar.gz
        PREFIX ${EXTERNAL_PROJECTS_PREFIX}
        CMAKE_ARGS
            -DCMAKE_INSTALL_PREFIX=${EXTERNAL_PROJECTS_INSTALL_PREFIX}
            -DGLM_TEST_ENABLE=OFF
        )
set_target_properties(external_glm PROPERTIES EXCLUDE_FROM_ALL TRUE)
add_dependencies(external_all external_glm)

ExternalProject_Add(external_gsl
        URL https://github.com/Microsoft/GSL/archive/${EXTERNAL_PROJECT_VERSION_GSL}.tar.gz
        PREFIX ${EXTERNAL_PROJECTS_PREFIX}
        CMAKE_ARGS
            -DCMAKE_INSTALL_PREFIX=${EXTERNAL_PROJECTS_INSTALL_PREFIX}
            -DGSL_TEST=OFF
        )
set_target_properties(external_gsl PROPERTIES EXCLUDE_FROM_ALL TRUE)
add_dependencies(external_all external_gsl)
