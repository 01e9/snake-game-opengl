include(ExternalProject)

set(EXTERNAL_PROJECTS_PREFIX ${CMAKE_BINARY_DIR}/lib)
set(EXTERNAL_PROJECTS_INSTALL_PREFIX ${EXTERNAL_PROJECTS_PREFIX}/installed)

add_custom_target(external_all)
set_target_properties(external_all PROPERTIES EXCLUDE_FROM_ALL TRUE)

ExternalProject_Add(external_glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG ${EXTERNAL_PROJECT_VERSION_GLFW}
        PREFIX ${EXTERNAL_PROJECTS_PREFIX}
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_PROJECTS_INSTALL_PREFIX}
        )
set_target_properties(external_glfw PROPERTIES EXCLUDE_FROM_ALL TRUE)
add_dependencies(external_all external_glfw)

ExternalProject_Add(external_glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG ${EXTERNAL_PROJECT_VERSION_GLM}
        PREFIX ${EXTERNAL_PROJECTS_PREFIX}
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_PROJECTS_INSTALL_PREFIX}
        )
set_target_properties(external_glm PROPERTIES EXCLUDE_FROM_ALL TRUE)
add_dependencies(external_all external_glm)

ExternalProject_Add(external_gsl
        GIT_REPOSITORY https://github.com/Microsoft/GSL.git
        GIT_TAG ${EXTERNAL_PROJECT_VERSION_GSL}
        PREFIX ${EXTERNAL_PROJECTS_PREFIX}
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_PROJECTS_INSTALL_PREFIX} -DGSL_TEST=OFF # https://github.com/Microsoft/GSL/issues/525
        )
set_target_properties(external_gsl PROPERTIES EXCLUDE_FROM_ALL TRUE)
add_dependencies(external_all external_gsl)
