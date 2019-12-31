#include "shader.h"
#include <fstream>
#include <sstream>

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {

    // 读取顶点着色器源码
    std::string vertexShaderSource;
    std::ifstream vertexFile;
    vertexFile.open(vertexPath, std::ios::in);
    if (vertexFile.is_open()) {
        std::stringstream file_stream;
        file_stream << vertexFile.rdbuf();
        vertexFile.close();
        vertexShaderSource = file_stream.str();
    }

    int ret = 0;
    char error_msg[512] = { 0 };

    // 编译顶点着色器源码
    const char* vertexShaderSrc = vertexShaderSource.c_str();
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &ret);
    if (!ret) {
        glGetShaderInfoLog(vertexShader, 512, NULL, error_msg);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED: %s", error_msg);
    }


    // 读取片段着色器源码
    std::string fragmentShaderSource;
    std::ifstream fragmentFile;
    fragmentFile.open(fragmentPath, std::ios::in);
    if (fragmentFile.is_open()) {
        std::stringstream file_stream;
        file_stream << fragmentFile.rdbuf();
        fragmentShaderSource = file_stream.str();
    }

    // 编译片段着色器源码
    const char* fragmentShaderSrc = fragmentShaderSource.c_str();
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSrc, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &ret);
    if (!ret) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, error_msg);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED: %s", error_msg);
    }

    // 创建着色器程序 
    //unsigned int shaderProgram;
    shader_program_ = glCreateProgram();
    glAttachShader(shader_program_, vertexShader);
    glAttachShader(shader_program_, fragmentShader);
    glLinkProgram(shader_program_);
    glGetProgramiv(shader_program_, GL_LINK_STATUS, &ret);
    if (!ret) {
        glGetProgramInfoLog(shader_program_, 512, NULL, error_msg);
        printf("ERROR::SHADER::PROGRAM::LINK_FAILED: %s", error_msg);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::Use() {
    glUseProgram(shader_program_);
}

void Shader::SetBool(const std::string& name, bool value) const {
    int location = glGetUniformLocation(shader_program_, name.c_str());
    glUniform1i(location, (int)value);
}

void Shader::SetInt(const std::string& name, int value) const {
    int location = glGetUniformLocation(shader_program_, name.c_str());
    glUniform1i(location, value);
}

void Shader::SetFloat(const std::string& name, float value) const {
    int location = glGetUniformLocation(shader_program_, name.c_str());
    glUniform1f(location, value);
}