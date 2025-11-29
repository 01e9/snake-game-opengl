add_custom_target(external_all
	DEPENDS
		external_gsl
		external_glfw
		external_glm
		external_glad
		external_boost
)
set_target_properties(external_all PROPERTIES EXCLUDE_FROM_ALL TRUE)

ExternalProject_Add(external_gsl
	#URL https://github.com/Microsoft/GSL/archive/v4.2.0.tar.gz
	URL "file://${EXT_PROJ_SOURCES_DIR}/gsl.tar.gz"
	PREFIX "${EXT_PROJ_DIR}"
	EXCLUDE_FROM_ALL TRUE
	CMAKE_ARGS
		-DCMAKE_INSTALL_PREFIX=${EXT_PROJ_INSTALL_DIR}
		-DGSL_TEST=OFF
)

ExternalProject_Add(external_glfw
	#URL https://github.com/glfw/glfw/archive/3.3.10.tar.gz
	URL "file://${EXT_PROJ_SOURCES_DIR}/glfw.tar.gz"
	PREFIX "${EXT_PROJ_DIR}"
	EXCLUDE_FROM_ALL TRUE
	CMAKE_ARGS
		-DCMAKE_INSTALL_PREFIX=${EXT_PROJ_INSTALL_DIR}
		-DBUILD_SHARED_LIBS=OFF
		-DGLFW_BUILD_EXAMPLES=OFF
		-DGLFW_BUILD_TESTS=OFF
		-DGLFW_BUILD_DOCS=OFF
		-DGLFW_INSTALL=ON
)

ExternalProject_Add(external_glm
	#URL https://github.com/g-truc/glm/archive/1.0.2.tar.gz
	URL "file://${EXT_PROJ_SOURCES_DIR}/glm.tar.gz"
	PREFIX "${EXT_PROJ_DIR}"
	EXCLUDE_FROM_ALL TRUE
	CMAKE_ARGS
		-DCMAKE_INSTALL_PREFIX=${EXT_PROJ_INSTALL_DIR}
)

ExternalProject_Add(external_glad
	# Generated on https://glad.dav1d.de + custom CMakeLists.txt
	URL "file://${EXT_PROJ_SOURCES_DIR}/glad.tar.gz"
	PREFIX "${EXT_PROJ_DIR}"
	EXCLUDE_FROM_ALL TRUE
	CMAKE_ARGS
		-DCMAKE_INSTALL_PREFIX=${EXT_PROJ_INSTALL_DIR}
)

ExternalProject_Add(external_boost
	URL https://archives.boost.io/release/1.89.0/source/boost_1_89_0.tar.gz
	PREFIX "${EXT_PROJ_DIR}"
	EXCLUDE_FROM_ALL TRUE
	CONFIGURE_COMMAND ./bootstrap.sh --with-libraries=thread --prefix=${EXT_PROJ_INSTALL_DIR}
	BUILD_COMMAND ./b2 -j${EXT_PROJ_BUILD_CPU_COUNT} link=static install
	BUILD_IN_SOURCE 1
	INSTALL_COMMAND ""
)