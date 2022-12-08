#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "shader.h"


float vertices[] = {
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f
};

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}



int main(int, char **)
{
    glfwInit();
    
    ///设置版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    
    /// 渲染 核心模式
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    GLFWwindow* windows = glfwCreateWindow(800, 600, "HelloGL", NULL, NULL);
    if (NULL == windows) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    ///GLFW将窗口的上下文设置为当前线程的上下文
    glfwMakeContextCurrent(windows);
    
    
    /// GLAD
    /// glad: 加载所有的OpenGL函数指针
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    Shader ourShader("../../shaders/shader.vs", "../../shaders/shader.fs");
    std::cout << __FILE__ << std::endl;
    
    ///创建VBO与VAO对象 并赋予ID
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    /// 绑定VBO与VAO对象
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    ///为当前绑定到target的缓冲对象创建一个新的数据存储。
    ///如果data不是NULL， 则是使用来自指针的数据初始化数据存储
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    ///告知shader如何解析缓冲里的属性值
    ///poistion属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    ///开启VAO管理的第一个属性值
    glEnableVertexAttribArray(0);
    
    ///color属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    ///开启VAO管理的第二个属性值
    glEnableVertexAttribArray(1);
    
    
    ///解绑VAO VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    
    ///渲染循环
    while (!glfwWindowShouldClose(windows)) {
        processInput(windows);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); ///状态设置
        glClear(GL_COLOR_BUFFER_BIT); ///状态使用
        
        ourShader.use();
        
        ///设置uniform值
        ourShader.setFloat("offsetX", 0.5);
        
        ///渲染三角形
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        ///glfw ：交换缓冲区和轮询IO事件（按键按下/释放、鼠标移动等）
        glfwSwapBuffers(windows);
        glfwPollEvents();
    }
    
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(ourShader.ID);
    
    
    ///glfw: 回收前面分配的GLFW先关资源
    glfwTerminate();
    
    
    return 0;
}
