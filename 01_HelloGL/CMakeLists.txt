cmake_minimum_required(VERSION 3.0.0)
project(HelloGL VERSION 0.1.0)

# 使用C++ 11标准
set(CMAKE_CXX_STANDARD 11)

## 添加头文件
set(GLFW_H /opt/homebrew/Cellar/glfw/3.3.8/include)
set(GLAD_H ${PROJECT_SOURCE_DIR}/glad/include)
include_directories(${GLAD_H} ${GLFW_H})

## 添加目标链接
set(GLFW_LINK /opt/homebrew/Cellar/glfw/3.3.8/lib/libglfw.3.dylib)
link_libraries(${GLFW_LINK})

## 执行编译命令
set(SOURCES glad/src/glad.c src/main.cpp)

# 设置可执行文件输出路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_executable(HelloGL ${SOURCES})

## 链接系统的OpenGL 框架
if (APPLE)
    target_link_libraries(HelloGL "-framework OpenGL")
endif()

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
