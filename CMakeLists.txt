cmake_minimum_required(VERSION 3.4.1)
SET(PROJ_NAME chacha)
project(${PROJ_NAME})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# if(CMAKE_SIZEOF_VOID_P EQUAL 8)
# 	message(STATUS "64bit")
# 	set(LIB_ARCH lib64)
# else()
# 	message(STATUS "32bit")
# 	set(LIB_ARCH lib)
# endif()

# ArmCL
# SET(ARMCL_DIR ${CMAKE_SOURCE_DIR}/library/ComputeLibrary)
# link_directories(${ARMCL_DIR}/build)
# include_directories(${ARMCL_DIR} ${ARMCL_DIR}/include)

# rknn api
# set(RKNN_API_PATH ${CMAKE_SOURCE_DIR}/library/rknn)
# include_directories(${RKNN_API_PATH}/include)
# set(RKNN_API_LIB ${RKNN_API_PATH}/${CMAKE_SYSTEM_NAME}/${LIB_ARCH}/librknn_api.so)

#stb
include_directories(${CMAKE_SOURCE_DIR}/library/)

set(CMAKE_INSTALL_RPATH "lib")

add_executable(${PROJ_NAME}
        main.cc
)

# ArmCL
# target_link_libraries(${PROJ_NAME} arm_compute_core
#         arm_compute_graph
#         arm_compute
#         )


		

# # RKNN
# if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
# 	target_link_libraries(${PROJ_NAME}
# 		${RKNN_API_LIB}
# 		pthread
# 		dl
# 	)
# else()
# 	target_link_libraries(${PROJ_NAME}
# 		${RKNN_API_LIB}
# 		dl
# 	)
# endif()

# install target and libraries
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/bin)

install(TARGETS ${PROJ_NAME} DESTINATION ./)
install(DIRECTORY link DESTINATION ./)
# install(PROGRAMS ${RKNN_API_LIB} DESTINATION lib)

