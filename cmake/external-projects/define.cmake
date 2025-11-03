include(ExternalProject)

#include(ProcessorCount)
#ProcessorCount(CPU_COUNT)
#MATH(EXPR CPU_COUNT "${CPU_COUNT}-2")

set(EXTERNAL_PROJECTS_SOURCES_DIR "${CMAKE_CURRENT_LIST_DIR}/sources")

set(EXTERNAL_PROJECTS_DIR "${CMAKE_BINARY_DIR}/external-projects")
set(EXTERNAL_PROJECTS_INSTALL_DIR "${EXTERNAL_PROJECTS_DIR}/installed")

link_directories(${EXTERNAL_PROJECTS_INSTALL_DIR}/${CMAKE_INSTALL_LIBDIR})

add_custom_target(external_all)
set_target_properties(external_all PROPERTIES EXCLUDE_FROM_ALL TRUE)

ExternalProject_Add(external_glfw
	#URL https://github.com/glfw/glfw/archive/3.4.tar.gz
	URL "file://${EXTERNAL_PROJECTS_SOURCES_DIR}/glfw.tar.gz"
	PREFIX ${EXTERNAL_PROJECTS_DIR}
	CMAKE_ARGS
	-DCMAKE_INSTALL_PREFIX=${EXTERNAL_PROJECTS_INSTALL_DIR}
	-DBUILD_SHARED_LIBS=OFF
	-DGLFW_BUILD_EXAMPLES=OFF
	-DGLFW_BUILD_TESTS=OFF
	-DGLFW_BUILD_DOCS=OFF
	-DGLFW_INSTALL=ON
)
set_target_properties(external_glfw PROPERTIES EXCLUDE_FROM_ALL TRUE)
add_dependencies(external_all external_glfw)

ExternalProject_Add(external_glm
	#URL https://github.com/g-truc/glm/archive/1.0.2.tar.gz
	URL "file://${EXTERNAL_PROJECTS_SOURCES_DIR}/glm.tar.gz"
	PREFIX ${EXTERNAL_PROJECTS_DIR}
	CMAKE_ARGS
	-DCMAKE_INSTALL_PREFIX=${EXTERNAL_PROJECTS_INSTALL_DIR}
)
set_target_properties(external_glm PROPERTIES EXCLUDE_FROM_ALL TRUE)
add_dependencies(external_all external_glm)

ExternalProject_Add(external_gsl
	#URL https://github.com/Microsoft/GSL/archive/v4.2.0.tar.gz
	URL "file://${EXTERNAL_PROJECTS_SOURCES_DIR}/gsl.tar.gz"
	PREFIX ${EXTERNAL_PROJECTS_DIR}
	CMAKE_ARGS
	-DCMAKE_INSTALL_PREFIX=${EXTERNAL_PROJECTS_INSTALL_DIR}
	-DGSL_TEST=OFF
)
set_target_properties(external_gsl PROPERTIES EXCLUDE_FROM_ALL TRUE)
add_dependencies(external_all external_gsl)

ExternalProject_Add(external_glad
	# Generated on https://glad.dav1d.de + custom CMakeLists.txt
	URL "file://${EXTERNAL_PROJECTS_SOURCES_DIR}/glad.tar.gz"
	PREFIX ${EXTERNAL_PROJECTS_DIR}
	CMAKE_ARGS
	-DCMAKE_INSTALL_PREFIX=${EXTERNAL_PROJECTS_INSTALL_DIR}
)
set_target_properties(external_glad PROPERTIES EXCLUDE_FROM_ALL TRUE)
add_dependencies(external_all external_glad)