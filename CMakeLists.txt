cmake_minimum_required(VERSION 3.4.1)
SET(PROJ_NAME chacha)
project(${PROJ_NAME})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DARM_COMPUTE_CL")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -DARM_COMPUTE_CL")

OPTION(GPU_MALI "use mali gpu" OFF)
OPTION(NPU_MYRIAD "use myriad npu" ON)
OPTION(NPU_RKNN "use rknn npu" ON)
OPTION(CPU_ARM "use arm cpu" OFF)



if (NPU_MYRIAD)
message("USE_NPU_MYRIAD")
add_compile_definitions(USE_NPU_MYRIAD=1)
endif()

if (NPU_RKNN)
message("USE_NPU_RKNN")
add_compile_definitions(USE_NPU_RKNN=1)
endif()

if (GPU_MALI)
message("USE_GPU_MALI")
add_compile_definitions(USE_GPU_MALI=1)
endif()

if (CPU_ARM)
message("USE_CPU_ARM")
add_compile_definitions(USE_CPU_ARM=1)
endif()


# https://docs.openvino.ai/latest/openvino_docs_OV_UG_Integrate_OV_with_your_application.html#doxid-openvino-docs-o-v-u-g-integrate-o-v-with-your-application

# message(${OpenVINO_SOURCE_DIR})
# EXECUTE_PROCESS (COMMAND uname -m OUTPUT_VARIABLE DEFINE_OS OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_STRIP_TRAILING_WHITESPACE) 

# if (${DEFINE_OS} STREQUAL "aarch64")
message("FOUND ARCHITECTURE : AARCH64")
find_package(OpenVINO REQUIRED)
find_package(InferenceEngine REQUIRED)
find_package(ngraph REQUIRED)

# else()
	# message("FOUND ARCHITECTURE : INTEL")
# endif()

link_directories(${CMAKE_SOURCE_DIR}/link)

# library headers
message(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/library/)
include_directories(${CMAKE_SOURCE_DIR}/lib/inc)
include_directories(${CMAKE_SOURCE_DIR}/library/ArmCL)
include_directories(${CMAKE_SOURCE_DIR}/module/args/)

set(CMAKE_INSTALL_RPATH "lib")


option(ARGS_BUILD_EXAMPLE "Build example" OFF)
option(ARGS_BUILD_UNITTESTS "Build unittests" OFF)
add_subdirectory(module/args/)


add_executable(${PROJ_NAME}
        main.cpp
        library/ArmCL/utils/GraphUtils.cpp 
        library/ArmCL/utils/Utils.cpp
		lib/src/stb_image.cpp

		lib/src/compose/manager.cpp

		lib/src/compose/mali.cpp
		lib/src/compose/myriad.cpp
		lib/src/compose/neon.cpp
		lib/src/compose/rknn.cpp

		lib/src/argument/parser.cpp
)

target_link_libraries(${PROJ_NAME} args)

# if (DEFINE_OS STREQUAL "aarch64")
	# OpenVINO
target_link_libraries(${PROJ_NAME} openvino::runtime)
# ArmCL
target_link_libraries(${PROJ_NAME} 
	arm_compute_core
	arm_compute_graph
	arm_compute
	pthread
)

# RKNN
	# if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
target_link_libraries(${PROJ_NAME}
	SimpleRKNN
	rknn_api
)
	# else()
# endif()

# install target and libraries
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/bin)

install(TARGETS ${PROJ_NAME} DESTINATION ./)
install(DIRECTORY link DESTINATION ./)
# install(PROGRAMS ${RKNN_API_LIB} DESTINATION lib)

