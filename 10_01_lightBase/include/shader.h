#ifndef __SHADER_H__
#define __SHADER_H__

#include <glad/glad.h>
#include <string>
#include <iostream>

#include <glm/glm.hpp>

class Shader
{
public:
    ///着色器程序ID
    unsigned int id_;
    
    
    ///构造函数读取并构建shader
    Shader(const char* vertexPath, const char* fragmentPath);
    
    ///使用/激活shader
    void use();
    

    ///设置uniform的实用函数
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;
    void setVec3(const std::string& name, const float x, const float y, const float z) const;

private:
    /// 检查shader编译错误
    void checkCompileErrors(unsigned int shader, std::string type);
};
#endif
