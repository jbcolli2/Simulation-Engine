cmake_minimum_required(VERSION 3.22)
project(Sim_Engine)

set(CMAKE_CXX_STANDARD 14)

# tell where to find the find_package files
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")


# find the required packages
set(GLM_ROOT_DIR ./)
find_package(GLM REQUIRED)
message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")
set(GLFW3_ROOT ./)
find_package(GLFW3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")
set(ASSIMP_INCLUDE_DIR ./)
set(ASSIMP_LIBRARY ./)
find_package(ASSIMP REQUIRED)
message(STATUS "Found ASSIMP in ${ASSIMP_INCLUDE_DIR}")

# add glfw to library list
set(LIBS ${LIBS} ${GLFW3_LIBRARY})

#set include directories
include_directories(${GLM_INCLUDE_DIR} ${GLFW3_INCLUDE_DIR} ${ASSIMP_INCLUDE_DIR} src include)

# add glad to library list
add_library(GLAD include/glad/src/glad.c)
set(LIBS ${LIBS} GLAD)


add_executable(Sim_Engine src/RunSENG.cpp src/SimEngine.cpp src/SimEngine.h)
target_link_libraries(Sim_Engine ${LIBS})