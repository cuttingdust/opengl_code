#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>


enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

/// 摄像机 默认值
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;


/// 一个抽象的相机类，用于处理输入并计算相应的欧拉角、矢量和矩阵，以便在OpenGL中使用
class Camera
{
public:
    /// 带向量的构造函数
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
    
    /// 具有标量值的构造函数
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    /// 返回使用Euler角度和LookAt矩阵计算的视图矩阵
    glm::mat4 GetViewMatrix();

    ///  处理从任何类似键盘的输入系统接收的输入。接受摄像机定义的ENUM形式的输入参数（从窗口系统中提取）
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);

    /// 处理从鼠标输入系统接收的输入。需要x和y方向的偏移值。
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
    

    /// 处理从鼠标滚轮事件接收的输入。仅需要在y轴上输入
    void ProcessMouseScroll(float yoffset);

private:
    
    /// 根据摄影机的（更新的）Euler角度计算前向量
    void updateCameraVectors();
    
public:
    /// 摄像机的属性
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    
    /// 欧拉角值
    float Yaw;
    float Pitch;
    
    /// 摄像机选项
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
};
#endif
