#pragma once
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Initial.h"
using namespace std;
class Shader
{
public:
    unsigned int program;

    /**
    * @brief 链接着色器
    * @param vertexPath 顶点着色器路径
    * @param fragmentPath 片段着色器路径
    */
    Shader(string vertexPath, string fragmentPath);
    /**
    * @brief 使用着色器
    */
    void use();

    // 以下为uniform函数

    /**
    * @brief 设置Bool型变量
    * @param name 变量名
    * @param val 变量值
    */
    void setBool(const string &name, bool val) const;

    /**
    * @brief 设置Int型变量
    * @param name 变量名
    * @param val 变量值
    */
    void setInt(const string &name, int val) const;

    /**
    * @brief 设置Float型变量
    * @param name 变量名
    * @param val 变量值
    */
    void setFloat(const string &name, float val) const;

    /**
    * @brief 用于3维空间
    * @param name 变量名
    * @param mat 变量值
    */
    void setMat4(const std::string &name, glm::mat4 mat) const;

    void setVec3(const std::string &name, float value1, float value2, float value3) const;
    void setVec3(const std::string &name, glm::vec3 vec) const;
private:

    /**
    * @brief 错误检查
    * @param shader 着色器
    * @param type 着色器类型
    */
    void errDetect(unsigned int shader, GLenum type);

    /**
    * @brief 文件读取
    * @param filePath 文件的路径
    * @return 文件中的字符串
    */
    string loadFile(string filePath);

};