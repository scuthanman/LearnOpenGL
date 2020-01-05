#include <glad/glad.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include "shader.h"
#include <windows.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// GLFW�ṩ��һЩ��Ⱦ�������������޶ȵĽӿڣ� �����ģ����ڣ����������
// GLAD����������OpenGL�ĺ���ָ���

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

//
void CreateShaderProgram(unsigned int& shaderProgram);
void CreateVAO(unsigned int& VAO, unsigned int& VBO, unsigned int& EBO);
void CreateTexture(unsigned int& texture1, unsigned int& texture2);
void DrawTriangle(unsigned int shaderProgram, unsigned int VAO);
void DrawTriangle(Shader& shader, unsigned int VAO);
void DrawTriangle(Shader& shader, unsigned int VAO, 
    unsigned int texture1, unsigned int texture2);

// settings
const int WND_WIDTH = 800;
const int WND_HEIGHT = 600;

int main() {
    // ��ʼ��
    glfwInit();
    // ָ��opengl 3.3 �汾
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // ʹ��core��Ⱦģʽ - �����Ⱦ���� (������������Ⱦģʽ - �̶���Ⱦ����)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // ��������
    GLFWwindow* window = glfwCreateWindow(WND_WIDTH, WND_HEIGHT, "DEMO", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // �����ڹ�����opengl����Ⱦ������
    glfwMakeContextCurrent(window);
    // ���ô��ڴ�С����Ļص�����
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // ��ʼ��opengl��غ����ĵ�ַָ��
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //unsigned int shaderProgram = 0;
    //CreateShaderProgram(shaderProgram);
    Shader shader("E:\\Project\\GitProject\\LearnOpenGL\\LearnOpenGL\\ShaderSrc\\shader.vs",
        "E:\\Project\\GitProject\\LearnOpenGL\\LearnOpenGL\\ShaderSrc\\shader.fs");

    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;
    CreateVAO(VAO, VBO, EBO);

    // ��һ��������ʾ���Ǵ��㽫��Ӧ�õ����е������ε�����ͱ���
    // �ڶ�������������������������
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    unsigned int texture1, texture2;
    CreateTexture(texture1, texture2);

    // ������windows��Ϣѭ��������ж��Ƿ���Ҫ�˳�
    while (!glfwWindowShouldClose(window)) {

        ///////////////���������¼�///////////////
        processInput(window);

        ///////////////������Ⱦ///////////////
        // �����Ļ����ɫ����
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // ִ�������Ļ
        glClear(GL_COLOR_BUFFER_BIT);
        // ����������
        DrawTriangle(shader, VAO, texture1, texture2);

        ///////////////ͼ����ʾ���µ���Ļ///////////////
        // ���Ƶ�ͼ���������Ļ��ʾ��
        glfwSwapBuffers(window);

        ///////////////��Ϣ��������¼�����///////////////
        // ����û�д���ʲô�¼�������������롢����ƶ��ȣ������´���״̬�������ö�Ӧ�Ļص�����;
        glfwPollEvents();
        Sleep(100);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    if (EBO != 0) {
        glDeleteBuffers(1, &EBO);
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void CreateShaderProgram(unsigned int& shaderProgram) {
    const char* vertexShaderSource =
        "#version 330 core \n"
        "layout(location = 0) in vec3 aPos; \n"
        "void main() { \n"
        "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); \n"
        "}\0";

    const char* fragmentShaderSource =
        "#version 330 core \n"
        "out vec4 FragColor; \n"
        "void main() { \n"
        "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); \n"
        "}\0";

    /////////////////////////////create vertex shader/////////////////////////////////////////////
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int ret;
    char error_msg[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &ret);
    if (!ret) {
        glGetShaderInfoLog(vertexShader, 512, NULL, error_msg);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED: %s", error_msg);
    }

    /////////////////////////////create fragment shader/////////////////////////////////////////////
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &ret);
    if (!ret) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, error_msg);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED: %s", error_msg);
    }

    ///////////////////////////////create program///////////////////////////////////////////
    //unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &ret);
    if (!ret) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, error_msg);
        printf("ERROR::SHADER::PROGRAM::LINK_FAILED: %s", error_msg);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void CreateVAO(unsigned int& VAO, unsigned int& VBO, unsigned int& EBO) {
    // ʹ��VAO, VBO, EBO ,����һ������(���������κϳ�)������

    // ���������δ���ɫ����
//     float vertices[] = {
//         // λ��             // ��ɫ
//         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // ����  - indx0
//        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // ����  - indx1
//         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // ����  - indx2
//     };
    // �ж��鲻ͬ���͵�vt��ʱ������±껹�ǰ����λ���㣬��������λ�á�
//     unsigned int indices[] = {
//         0, 1, 2
//     };

    // ���δ���ɫ����
//     float vertices[] = {
//         // λ��             // ��ɫ
//         0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // ���Ͻ� - indx0
//         0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // ���½� - indx1
//        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  // ���½� - indx2
//        -0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f   // ���Ͻ� - indx3
//     };
//     unsigned int indices[] = {
//         0, 1, 3,  // ��һ��������
//         1, 2, 3   // �ڶ���������
//     };

    float vertices[] = {
        //     ---- λ�� ----       ---- ��ɫ ----     - �������� -
             0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // ����
             0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // ����
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // ����
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // ����
    };

    unsigned int indices[] = {
        0, 1, 3,  // ��һ��������
        1, 2, 3   // �ڶ���������
    };

    ///////////////////////////////create VAO, VBO; copy vertex///////////////////////////////////////////
    //unsigned int VAO;
    // VAO ��Žṹʱ������ EBO����ָ�룬�������Խṹ�����飬�ṹ����VBOָ�����ν������ݵ���Ϣ
    glGenVertexArrays(1, &VAO);
    // ������ʵ����Ҫʹ�õĵط��ٰ�VAO
    glBindVertexArray(VAO);

    //unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    ///////////////////////////////vertex attribute///////////////////////////////////////////
    
    // ������Ե���ԭʼ���ݵĵ㣬����index���������
    // ��һ������ָ��������0��ʼȡ���ݣ��붥����ɫ����layout(location=0)��Ӧ
    // �ڶ�������ָ���������Ե������
    // ����������ָ����������
    // ���ĸ����������Ƿ�ϣ�����ݱ���׼������һ������ ��׼��������(-1 , 1)֮��ġ�
    // ����������ǲ�����Stride����ָ���������Ķ�������֮��ļ��(������ͬ���Զ�����ʼλ��֮����)
    // ������������ʾ���ǵ�λ�������ڻ�������ʼλ�õ�ƫ����(�����������еĸ���������ʼƫ��λ��)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);  // ����λ�ö�������
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));  // ������ɫ��������
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));  // ��������������
    // �Զ�������λ��ֵ��Ϊ���������ö������ԣ���������Ĭ���ǽ��õ�
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // ���Խ�� VBO, ��Ϊ�� glVertexAttribPointer ���Ѿ�ָ����ע�� VBO��
    // ע���������Ϊ֪������ν���VBO��һƬ�ڴ�
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // EBO�����Ա����
    // �������Ϊû��ע�ᣬ��û�д洢������EBO�ڴ�����ݣ����Բ��ܱ����
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // ���Խ�� VAO������ VAO �������ط��޸�
    glBindVertexArray(0);
}

void CreateTexture(unsigned int& texture1, unsigned int& texture2) {
    // ���� Texture1
    //unsigned int texture;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // ���ز���������
    int width, height, nrChannels;
    unsigned char* data = stbi_load("E:\\Project\\GitProject\\LearnOpenGL\\LearnOpenGL\\res\\container.jpg",
        &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        printf("Failed to load texture");
    }
    stbi_image_free(data);

    // ���� Texture2
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // ���ز���������
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("E:\\Project\\GitProject\\LearnOpenGL\\LearnOpenGL\\res\\awesomeface.png",
        &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        printf("Failed to load texture");
    }
    stbi_image_free(data);
}

void DrawTriangle(unsigned int shaderProgram, unsigned int VAO) {
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    // glDrawArrays(GL_TRIANGLES, 0, 3); ��ʹ����EBO������ֱ��DrawArrays

    // ��һ������ָ�������ǻ��Ƶ�ģʽ GL_TRIANGLES ��������
    // �ڶ������������Ǵ�����ƶ���ĸ��� sizeof(indices) = 6 ������
    // ����������������������
    // ���ĸ�������ָ��EBO�е�ƫ����
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void DrawTriangle(Shader& shader, unsigned int VAO) {
    shader.Use();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void DrawTriangle(Shader& shader, unsigned int VAO, unsigned int texture1, unsigned int texture2) {
    shader.Use();
    shader.SetInt("texture1", 0);
    shader.SetInt("texture2", 1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}