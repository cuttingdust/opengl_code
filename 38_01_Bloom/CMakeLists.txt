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
 shaders/shader.vs
 shaders/shader.fs
 shaders/shader.gs
 shaders/hdr.vs
 shaders/hdr.fs
 shaders/blur.vs
 shaders/blur.fs
 shaders/light.vs
 shaders/light.fs
 shaders/lighting.vs
 shaders/lighting.fs
 shaders/shapes.vs
 shaders/shapes.fs
 shaders/asteroids.vs
 shaders/asteroids.fs
 shaders/debugDepthQuad.vs
 shaders/debugDepthQuad.fs
 shaders/simpleDepthShader.vs
 shaders/simpleDepthShader.fs
 shaders/lightShader.fs
 shaders/shaderSingleColor.fs
 shaders/screenShader.vs
 shaders/screenShader.fs
 shaders/skyboxShader.vs
 shaders/skyboxShader.fs
 )

## 执行编译命令
set(SOURCES
 glad/src/glad.c
 src/main.cpp
 src/shader.cpp
 src/camera.cpp
 src/mesh.cpp
 src/model.cpp 

 include/shader.h
 include/camera.h
 include/stb_image.h
 include/mesh.h
 include/model.h
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
