include(ExternalProject)

include(ProcessorCount)
ProcessorCount(CPU_COUNT)
MATH(EXPR CPU_COUNT "${CPU_COUNT}-2")

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

ExternalProject_Add(external_boost
	URL https://archives.boost.io/release/1.89.0/source/boost_1_89_0.tar.gz
	PREFIX ${EXTERNAL_PROJECTS_DIR}
	CONFIGURE_COMMAND ./bootstrap.sh
		--with-libraries=system,thread,filesystem,chrono,atomic,regex,context,program_options
		--prefix=${EXTERNAL_PROJECTS_INSTALL_DIR}
	BUILD_IN_SOURCE 1
	BUILD_COMMAND ./b2 -j${CPU_COUNT} install
	INSTALL_COMMAND ""
)
set_target_properties(external_boost PROPERTIES EXCLUDE_FROM_ALL TRUE)
add_dependencies(external_all external_boost)

ExternalProject_Add(external_double_conversion
	#URL https://github.com/google/double-conversion/archive/adc7c2450f99e0ca673238276c93e7896afa7b29.tar.gz
	URL "file://${EXTERNAL_PROJECTS_SOURCES_DIR}/double-conversion.tar.gz"
	PREFIX ${EXTERNAL_PROJECTS_DIR}
	CMAKE_ARGS
	-DCMAKE_INSTALL_PREFIX=${EXTERNAL_PROJECTS_INSTALL_DIR}
	-DCMAKE_POSITION_INDEPENDENT_CODE=ON
)
set_target_properties(external_double_conversion PROPERTIES EXCLUDE_FROM_ALL TRUE)
add_dependencies(external_all external_double_conversion)

ExternalProject_Add(external_fast_float
	#URL https://github.com/fastfloat/fast_float/archive/refs/tags/v8.1.0.tar.gz
	URL "file://${EXTERNAL_PROJECTS_SOURCES_DIR}/fast-float.tar.gz"
	PREFIX ${EXTERNAL_PROJECTS_DIR}
	CMAKE_ARGS
	-DCMAKE_INSTALL_PREFIX=${EXTERNAL_PROJECTS_INSTALL_DIR}
	-DCMAKE_POSITION_INDEPENDENT_CODE=ON
)
set_target_properties(external_fast_float PROPERTIES EXCLUDE_FROM_ALL TRUE)
add_dependencies(external_all external_fast_float)

ExternalProject_Add(external_fmt
	#URL https://github.com/fmtlib/fmt/archive/refs/tags/12.1.0.tar.gz
	URL "file://${EXTERNAL_PROJECTS_SOURCES_DIR}/fmt.tar.gz"
	PREFIX ${EXTERNAL_PROJECTS_DIR}
	CMAKE_ARGS
	-DCMAKE_INSTALL_PREFIX=${EXTERNAL_PROJECTS_INSTALL_DIR}
)
set_target_properties(external_fmt PROPERTIES EXCLUDE_FROM_ALL TRUE)
add_dependencies(external_all external_fmt)

ExternalProject_Add(external_folly
	#URL https://github.com/facebook/folly/releases/download/v2025.10.27.00/folly-v2025.10.27.00.tar.gz
	URL "file://${EXTERNAL_PROJECTS_SOURCES_DIR}/folly.tar.gz"
	PREFIX ${EXTERNAL_PROJECTS_DIR}
	PATCH_COMMAND patch -p1 -i ${EXTERNAL_PROJECTS_SOURCES_DIR}/folly.patch
	CMAKE_ARGS
	-DCMAKE_INSTALL_PREFIX=${EXTERNAL_PROJECTS_INSTALL_DIR}
)
set_target_properties(external_folly PROPERTIES EXCLUDE_FROM_ALL TRUE)
add_dependencies(external_folly external_double_conversion external_fast_float external_fmt external_boost)
add_dependencies(external_all external_folly)