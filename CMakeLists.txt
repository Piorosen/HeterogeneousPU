cmake_minimum_required(VERSION 3.4.1)
SET(PROJ_NAME chacha)
project(${PROJ_NAME})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

# https://docs.openvino.ai/latest/openvino_docs_OV_UG_Integrate_OV_with_your_application.html#doxid-openvino-docs-o-v-u-g-integrate-o-v-with-your-application

# message(${OpenVINO_SOURCE_DIR})
EXECUTE_PROCESS (COMMAND uname -m
	OUTPUT_VARIABLE DEFINE_OS)

if (DEFINE_OS STREQUAL "aarch64")
	find_package(OpenVINO REQUIRED)
	find_package(InferenceEngine REQUIRED)
	find_package(ngraph REQUIRED)
endif()


if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	message(STATUS "64bit")
	set(LIB_ARCH lib64)
else()
	message(STATUS "32bit")
	set(LIB_ARCH lib)
endif()

set(LINK_LIB ${CMAKE_SOURCE_DIR}/link)
link_directories(${LINK_LIB})

# library headers
include_directories(${CMAKE_SOURCE_DIR}/library/)
include_directories(${CMAKE_SOURCE_DIR}/lib/inc)
include_directories(${CMAKE_SOURCE_DIR}/library/ArmCL)
include_directories(${CMAKE_SOURCE_DIR}/runtime/include)

set(CMAKE_INSTALL_RPATH "lib")

add_executable(${PROJ_NAME}
        main.cc
)

# OpenVINO
if (DEFINE_OS STREQUAL "aarch64")
	target_link_libraries(${PROJ_NAME} openvino::runtime)
endif()
# ArmCL
target_link_libraries(${PROJ_NAME} 
        arm_compute_core
        arm_compute_graph
        arm_compute
        )

# RKNN
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
	target_link_libraries(${PROJ_NAME}
                rknn_api
                SimpleRKNN
		pthread
		dl
	)
else()
	target_link_libraries(${PROJ_NAME}
                rknn_api
                SimpleRKNN
		dl
	)
endif()

# install target and libraries
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/bin)

install(TARGETS ${PROJ_NAME} DESTINATION ./)
install(DIRECTORY link DESTINATION ./)
# install(PROGRAMS ${RKNN_API_LIB} DESTINATION lib)

