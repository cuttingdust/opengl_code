#include "camera.h"


Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : front_(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed_(gSpeed), mouseSensitivity_(gSensitivity), zoom_(gZoom)
{
    position_ = position;
    worldUp_ = up;
    yaw_ = yaw;
    pitch_ = pitch;
    updateCameraVectors();;
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : front_(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed_(gSpeed), mouseSensitivity_(gSensitivity), zoom_(gZoom) {
    position_ = glm::vec3(posX, posY, posZ);
    worldUp_ = glm::vec3(upX, upY, upZ);
    yaw_ = yaw;
    pitch_ = pitch;
    updateCameraVectors();;
}

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(position_, position_ + front_, up_);
//    return calculateLookAtMatrix(position_, position_ + front_, up_);
}

void Camera::ProcessKeyboard(CameraMovement direction, float deltaTime) {
    float velocity = movementSpeed_ * deltaTime;
    if (direction == CM_FORWARD)
        position_ += front_ * velocity;
    if (direction == CM_BACKWARD)
        position_ -= front_ * velocity;
    if (direction == CM_LEFT)
        position_ -= right_ * velocity;
    if (direction == CM_RIGHT)
        position_ += right_ * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) {
    xoffset *= mouseSensitivity_;
    yoffset *= mouseSensitivity_;

    yaw_   += xoffset;
    pitch_ += yoffset;

    if (constrainPitch)
    {
        if (pitch_ > 89.0f)
            pitch_ = 89.0f;
        if (pitch_ < -89.0f)
            pitch_ = -89.0f;
    }

    /// 使用更新的Euler角度更新前、右和上向量
    updateCameraVectors();;
}

void Camera::ProcessMouseScroll(float yoffset) {
    zoom_ -= (float)yoffset;
    if (zoom_ < 1.0f)
        zoom_ = 1.0f;
    if (zoom_ > 45.0f)
        zoom_ = 45.0f;;
}

void Camera::updateCameraVectors() {
    /// 计算新的Front向量
    glm::vec3 front;
    front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front.y = sin(glm::radians(pitch_));
    front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front_ = glm::normalize(front);
    
    /// 还重新计算右向量和上向量
    /// 归一化向量，因为你向上或向下看得越多，它们的长度就越接近0，这会导致移动速度变慢。
    right_ = glm::normalize(glm::cross(front_, worldUp_));
    up_ = glm::normalize(glm::cross(right_, front_));;
}

glm::mat4 Camera::calculateLookAtMatrix(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp) { 
    /// 1、位置 = 已知
    /// 2、计算方向
    glm::vec3 zAxis = glm::normalize(position - target);
    
    /// 3、获取右轴
    glm::vec3 xAxis = glm::normalize(glm::cross(glm::normalize(worldUp), zAxis));
    
    /// 4、计算向上的向量
    glm::vec3 yAxis = glm::cross(zAxis, xAxis);
    
    /// 创建位移和旋转矩阵
    /// 在GLM中， 采用[col][row]
    glm::mat4 translation = glm::mat4(1.0f);
    translation[3][0] = -position.x; /// 第四列第一行
    translation[3][1] = -position.y;
    translation[3][2] = -position.z;
    
    glm::mat4 rotation = glm::mat4(1.0f);
    rotation[0][0] = xAxis.x; /// 第一列， 第一行
    rotation[1][0] = xAxis.y;
    rotation[2][0] = xAxis.z;
    rotation[0][1] = yAxis.x; // 第一列， 第二行
    rotation[1][1] = yAxis.y;
    rotation[2][1] = yAxis.z;
    rotation[0][2] = zAxis.x; // 第一列， 第三行
    rotation[1][2] = zAxis.y;
    rotation[2][2] = zAxis.z;

    return rotation * translation; ///先位移， 再旋转
}







