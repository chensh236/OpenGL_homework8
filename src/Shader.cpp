#include "Shader.h"
/**
* @brief 链接着色器
* @param vertexPath 顶点着色器路径
* @param fragmentPath 片段着色器路径
*/
Shader::Shader(string vertexPath, string fragmentPath)
{
    // 读取文件
    string vCode = loadFile(vertexPath);
    string fCode = loadFile(fragmentPath);
    const char* vShaderCode = vCode.c_str();
    const char* fShaderCode = fCode.c_str();

    unsigned int vertex, fragment;
   /* cout << "test:" << endl << vShaderCode << endl;*/

    // 顶点着色器
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    errDetect(vertex, GL_COMPILE_STATUS);

    // 片段着色器
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    errDetect(fragment, GL_COMPILE_STATUS);

    // 编译着色器
    program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    errDetect(program, GL_LINK_STATUS);
    // 不必要时删除着色器
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}
/**
* @brief 使用着色器
*/
void Shader::use()
{
    glUseProgram(program);
}

// 以下为uniform函数

/**
* @brief 设置Bool型变量
* @param name 变量名
* @param val 变量值
*/
void Shader::setBool(const string &name, bool val) const
{
    glUniform1i(glGetUniformLocation(program, name.c_str()), (int)val);
}

/**
* @brief 设置Int型变量
* @param name 变量名
* @param val 变量值
*/
void Shader::setInt(const string &name, int val) const
{
    glUniform1i(glGetUniformLocation(program, name.c_str()), val);
}

/**
* @brief 设置Float型变量
* @param name 变量名
* @param val 变量值
*/
void Shader::setFloat(const string &name, float val) const
{
    glUniform1f(glGetUniformLocation(program, name.c_str()), val);
}


/**
* @brief 错误检查
* @param shader 着色器
* @param type 着色器类型
*/
void Shader::errDetect(unsigned int shader, GLenum type) {
    int success;
    glGetShaderiv(shader, type, &success);
    if (!success) {
        char infoLog[1024];
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        cout << "Err: " << infoLog << endl;
    }

}

/**
* @brief 文件读取
* @param filePath 文件的路径
* @return 文件中的字符串
*/
string Shader::loadFile(string filePath) {

    string code;
    ifstream shaderFile;
    shaderFile.exceptions(ifstream::failbit | ifstream::badbit);
    try
    {
        // 打开文件
        shaderFile.open(filePath.c_str());
        stringstream shaderStream;
        // 将文本从buffer读取到流中
        shaderStream << shaderFile.rdbuf();
        // 关闭
        shaderFile.close();
        // 流转换为string
        code = shaderStream.str();
    }
    catch (ifstream::failure e)
    {
        cout << "Err: Fail to read file: " << filePath << endl;
    }
    return code;
}

void Shader::setMat4(const std::string &name, glm::mat4 mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setVec3(const std::string &name, float value1, float value2, float value3) const {
    glUniform3f(glGetUniformLocation(program, name.c_str()), value1, value2, value3);
}


void Shader::setVec3(const std::string &name, glm::vec3 vec) const {
    glUniform3f(glGetUniformLocation(program, name.c_str()), vec.x, vec.y, vec.z);
}