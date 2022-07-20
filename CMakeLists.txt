cmake_minimum_required(VERSION 3.21)
project(Sim_Engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# tell where to find the find_package files
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")


link_directories(${CMAKE_SOURCE_DIR}/lib)

# find the required packages
set(GLM_ROOT_DIR ./)
find_package(GLM REQUIRED)
message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")
set(GLFW3_ROOT ./)
find_package(GLFW3 3.3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")
set(ASSIMP_INCLUDE_DIR ./)
set(ASSIMP_LIBRARY ./)
find_package(ASSIMP REQUIRED)
message(STATUS "Found ASSIMP in ${ASSIMP_INCLUDE_DIR}")

#find_package(OpenGL REQUIRED)
#message("OpenGL found in ${OpenGL_INCLUDE_DIR}")
INCLUDE_DIRECTORIES(/System/Library/Frameworks)
FIND_LIBRARY(COCOA_LIBRARY Cocoa)
FIND_LIBRARY(OpenGL_LIBRARY OpenGL)
FIND_LIBRARY(IOKit_LIBRARY IOKit)
FIND_LIBRARY(CoreVideo_LIBRARY CoreVideo)
MARK_AS_ADVANCED(COCOA_LIBRARY OpenGL_LIBRARY)

# add glfw to library list
set(LIBS ${LIBS} ${GLFW3_LIBRARY} ${OpenGL_LIBRARY} ${COCOA_LIBRARY} ${IOKit_LIBRARY} ${CoreVideo_LIBRARY} )




# add glad to library list
add_library(GLAD include/glad/glad.c)
set(LIBS ${LIBS} GLAD)

# Source files
set(SRC_ENGINE src/Engine/SimEngine.cpp src/Engine/DisplayManager.cpp src/Scene/SceneManager.cpp src/Engine/Input.cpp)
set(SRC_OBJECTS src/ObjComp/Object.cpp src/ObjComp/Component.cpp src/ObjComp/ComponentManager.cpp src/Scene/Mesh.h src/Scene/Material.h src/Scene/Cube.cpp src/Scene/Cube.h src/Scene/SolidMaterial.cpp src/Scene/SolidMaterial.h src/Scene/Scene.cpp src/Scene/Camera.h src/Scene/Camera.cpp)
set(SRC_RENDER src/Rendering/Renderer.cpp)

add_compile_definitions(GL_SILENCE_DEPRECATION)

add_executable(Sim_Engine src/RunSENG.cpp ${SRC_ENGINE} ${SRC_OBJECTS} ${SRC_RENDER} src/Misc/Util.cpp)
target_link_libraries(Sim_Engine ${LIBS})


include_directories(src include)


add_executable(Playground play/play.cpp ${SRC_OBJECTS})
target_link_libraries(Playground ${LIBS})