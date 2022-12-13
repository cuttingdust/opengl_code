#include "shader.h"
#include "camera.h"
#include "mesh.h"
#include "model.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>



#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool firstMouse = true;

float lastX = 800.0f / 2.0f;
float lastY = 600.0f / 2.0f;


unsigned int diffuseMap;
unsigned int specularMap;
unsigned int emissionMap;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }


    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(CM_FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(CM_BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(CM_LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(CM_RIGHT, deltaTime);

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}



void mouse_callback(GLFWwindow* window, double xPos, double yPos){
    if (firstMouse) {
        lastX = xPos;
        lastY = yPos;

        firstMouse = false;
    }

    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos;
    lastX = xPos;
    lastY = yPos;

    float sensitivity = 1.0f;
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    camera.ProcessMouseMovement(xOffset, yOffset);
}


void scroll_callback(GLFWwindow* window, double xOffset, double yOffset){
    camera.ProcessMouseScroll(static_cast<float>(yOffset));
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
    glfwSetFramebufferSizeCallback(windows, framebuffer_size_callback);
    glfwSetCursorPosCallback(windows, mouse_callback);
    glfwSetScrollCallback(windows, scroll_callback);


    //glfwSetInputMode(windows, GLFW_CURSOR, GLFW_CURSOR_NORMAL);


    /// GLAD
    /// glad: 加载所有的OpenGL函数指针
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    glEnable(GL_DEPTH_TEST);

    Shader ourShader("../../shaders/shader.vs", "../../shaders/shader.fs");
    Shader lightShader("../../shaders/shader.vs", "../../shaders/lightShader.fs");


    Model ourModel("../../resources/nanosuit/nanosuit.obj");
    
    
    ///渲染循环
    while (!glfwWindowShouldClose(windows)) {

        ///摄像机控制
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(windows);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);///状态设置
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); ///状态使用


        ourShader.use();
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(camera.zoom_), 800.0f / 600.0f, 0.1f, 100.f);
        ourShader.setMat4("projection", projection);

        glm::mat4 view = glm::mat4(1.0f);
        view = camera.GetViewMatrix();
        ourShader.setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        ourShader.setMat4("model", model);


        // render the loaded model
           model = glm::mat4(1.0f);
           model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
           model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));    // it's a bit too big for our scene, so scale it down
           ourShader.setMat4("model", model);
           ourModel.Draw(ourShader);


        glfwSwapBuffers(windows);
        glfwPollEvents();
    }

    ///glfw: 回收前面分配的GLFW先关资源
    glfwTerminate();


    return 0;
}

