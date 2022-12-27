cmake_minimum_required(VERSION 3.4.1)
SET(PROJ_NAME chacha)
project(${PROJ_NAME})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DARM_COMPUTE_CL")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -DARM_COMPUTE_CL")

SET(GPU_MALI 	false)
SET(CPU_ARM 	true)
SET(NPU_MYRIAD 	false)
SET(NPU_RKNN 	false)
SET(NPU_CORAL 	true)

if (NPU_MYRIAD)
message("USE_NPU_MYRIAD")
add_compile_definitions(USE_NPU_MYRIAD=1)
# message("FOUND ARCHITECTURE : AARCH64")
find_package(OpenVINO REQUIRED)
find_package(InferenceEngine REQUIRED)
find_package(ngraph REQUIRED)
endif()

# https://docs.openvino.ai/latest/openvino_docs_OV_UG_Integrate_OV_with_your_application.html#doxid-openvino-docs-o-v-u-g-integrate-o-v-with-your-application

# message(${OpenVINO_SOURCE_DIR})
# EXECUTE_PROCESS (COMMAND uname -m OUTPUT_VARIABLE DEFINE_OS OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_STRIP_TRAILING_WHITESPACE) 

link_directories(${CMAKE_SOURCE_DIR}/link)
link_directories(${CMAKE_SOURCE_DIR}/module/armnn_prebuilt)

# library headers
message(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/library/armnn)
include_directories(${CMAKE_SOURCE_DIR}/library/armnn/armnn/utility)


include_directories(${CMAKE_SOURCE_DIR}/library/)
include_directories(${CMAKE_SOURCE_DIR}/lib/inc)
include_directories(${CMAKE_SOURCE_DIR}/library/ArmCL)
include_directories(${CMAKE_SOURCE_DIR}/module/args/)
include_directories(${CMAKE_SOURCE_DIR}/module/libedgetpu)
include_directories(${CMAKE_SOURCE_DIR}/module/tensorflow)
include_directories(${CMAKE_SOURCE_DIR}/module/abseil-cpp)
include_directories(${CMAKE_SOURCE_DIR}/module/flatbuffers/include)

set(CMAKE_INSTALL_RPATH "lib")


option(ARGS_BUILD_EXAMPLE "Build example" OFF)
option(ARGS_BUILD_UNITTESTS "Build unittests" OFF)
add_subdirectory(module/args/)

set(SOURCE_FILES main.cpp 
				 lib/src/compose/manager.cpp)


if (NPU_MYRIAD)
	SET(SOURCE_FILES ${SOURCE_FILES} lib/src/compose/myriad.cpp)
endif()
if (NPU_RKNN)
message("USE_NPU_RKNN")
add_compile_definitions(USE_NPU_RKNN=1)
SET(SOURCE_FILES ${SOURCE_FILES} lib/src/compose/rknn.cpp)
endif()

if (GPU_MALI OR CPU_ARM)
	SET(SOURCE_FILES ${SOURCE_FILES} library/ArmCL/utils/GraphUtils.cpp 
									 library/ArmCL/utils/Utils.cpp
									 lib/src/compose/extension/ObjectDetection.cpp)
endif()

if (GPU_MALI)
	message("USE_GPU_MALI")
	add_compile_definitions(USE_GPU_MALI=1)
	SET(SOURCE_FILES ${SOURCE_FILES} lib/src/compose/mali.cpp)
endif()
if (CPU_ARM)
	message("USE_CPU_ARM")
	add_compile_definitions(USE_CPU_ARM=1)
	SET(SOURCE_FILES ${SOURCE_FILES} lib/src/compose/neon.cpp)
endif()
if (NPU_CORAL)
	message("USE_NPU_CORAL")
	add_compile_definitions(USE_NPU_CORAL=1)
	SET(SOURCE_FILES ${SOURCE_FILES} lib/src/compose/edgetpu.cpp)
endif()

message("Sources : " ${SOURCE_FILES})

add_executable(${PROJ_NAME}
        ${SOURCE_FILES}

		lib/src/stb_image.cpp
		lib/src/argument/parser.cpp
)


target_link_libraries(${PROJ_NAME} args pthread)

if (NPU_MYRIAD)
target_link_libraries(${PROJ_NAME} openvino::runtime)
endif()
if (NPU_RKNN)
# RKNN
target_link_libraries(${PROJ_NAME}
	SimpleRKNN
	rknn_api
)
endif()
if (CPU_ARM OR GPU_MALI)
# ArmCL
target_link_libraries(${PROJ_NAME} 
	arm_compute_core
	arm_compute_graph
	arm_compute
	armnn
	armnnDelegate
	armnnTfLiteParser
	
)
endif()
if (NPU_CORAL)
target_link_libraries(${PROJ_NAME} edgetpu)
endif()

if (NPU_CORAL OR CPU_ARM OR GPU_MALI)
	target_link_libraries(${PROJ_NAME} flatbuffers tensorflowlite tensorflowlite_c)
endif()


# install target and libraries
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/bin)

install(TARGETS ${PROJ_NAME} DESTINATION ./)
install(DIRECTORY link DESTINATION ./)
# install(PROGRAMS ${RKNN_API_LIB} DESTINATION lib)

