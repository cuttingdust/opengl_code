cmake_minimum_required(VERSION 3.0.0)
project(HelloGL VERSION 0.1.0)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# 使用C++ 11标准
set(CMAKE_CXX_STANDARD 11)

## 添加头文件
set(GLFW_H /opt/homebrew/Cellar/glfw/3.3.8/include)
set(ASSIMP_H /opt/homebrew/Cellar/assimp/5.2.5/include)
set(GLAD_H ${PROJECT_SOURCE_DIR}/glad/include)
set(GLM_H ${PROJECT_SOURCE_DIR}/glm)
include_directories(${GLAD_H} ${GLFW_H} ${GLM_H} ${ASSIMP_H} include)


##添加目标链接
set(GLFW_LINK /opt/homebrew/Cellar/glfw/3.3.8/lib/libglfw.3.dylib)
set(ASSIMP_LINK /opt/homebrew/Cellar/assimp/5.2.5/lib/libassimp.5.dylib)
link_libraries(${GLFW_LINK} ${ASSIMP_LINK})


set(SHADERS
 shaders/fPointShader.glsl
 shaders/fragmentShader.glsl
 shaders/vertexShader.glsl
 shaders/vPointShader.glsl
 )

## 执行编译命令
set(SOURCES
 glad/src/glad.c
 src/main.cpp
 src/Shader.cpp
 src/Camera.cpp
 src/ffMesh.cpp
 src/ffModel.cpp
 src/ffAssimpHelper.cpp
 src/ffBone.cpp
 src/ffImage.cpp
 src/ffAnimation.cpp
 src/ffAnimator.cpp
 
 include/Shader.h
 include/Camera.h
 include/ffMesh.h
 include/ffModel.h
 include/ffAssimpHelper.h
 include/ffBone.h
 include/ffImage.h
 include/ffAnimation.h
 include/ffAnimator.h
 include/Base.h
 include/stb_image.h
)

source_group("Shader Files" FILES ${SHADERS})

add_executable(HelloGL ${SOURCES} ${SHADERS})

## 链接系统的OpenGL 框架
if (APPLE)
    target_link_libraries(HelloGL "-framework OpenGL")
endif()

include(CTest)
enable_testing()



set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
