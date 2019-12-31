#include <glad/glad.h>
#include <GLFW\glfw3.h>
#include <iostream>

// GLFW提供了一些渲染物体所需的最低限度的接口， 上下文，窗口，输入输出等
// GLAD是用来管理OpenGL的函数指针的

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

//
void CreateShaderProgram(unsigned int& shaderProgram);
void CreateVAO(unsigned int& VAO, unsigned int& VBO, unsigned int& EBO);
void DrawTriangle(unsigned int shaderProgram, unsigned int VAO);

// settings
const int WND_WIDTH = 800;
const int WND_HEIGHT = 600;

int main() {
    // 初始化
    glfwInit();
    // 指定opengl 3.3 版本
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // 使用core渲染模式 - 编程渲染管线 (区别于立即渲染模式 - 固定渲染管线)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 创建窗口
    GLFWwindow* window = glfwCreateWindow(WND_WIDTH, WND_HEIGHT, "DEMO", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // 将窗口关联到opengl的渲染上下文
    glfwMakeContextCurrent(window);
    // 设置窗口大小变更的回调函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 初始化opengl相关函数的地址指针
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    unsigned int shaderProgram = 0;
    CreateShaderProgram(shaderProgram);

    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;
    CreateVAO(VAO, VBO, EBO);

    // 第一个参数表示我们打算将其应用到所有的三角形的正面和背面
    // 第二个参数告诉我们用线来绘制
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // 类似于windows消息循环，入口判断是否需要退出
    while (!glfwWindowShouldClose(window)) {

        ///////////////处理输入事件///////////////
        processInput(window);

        ///////////////处理渲染///////////////
        // 清空屏幕的颜色设置
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // 执行清空屏幕
        glClear(GL_COLOR_BUFFER_BIT);
        // 绘制三角形
        DrawTriangle(shaderProgram, VAO);

        ///////////////图像显示更新到屏幕///////////////
        // 绘制的图像输出到屏幕显示中
        glfwSwapBuffers(window);

        ///////////////消息队列里的事件处理///////////////
        // 查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数;
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

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
    // 使用VAO, VBO, EBO ,绘制一个矩形(两个三角形合成)的坐标
    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };

    ///////////////////////////////create VAO, VBO; copy vertex///////////////////////////////////////////
    //unsigned int VAO;
    // VAO 大概结构时包含了 EBO对象指针，顶点属性结构体数组，结构中有VBO指针和如何解析数据的信息
    glGenVertexArrays(1, &VAO);
    // 可以在实际需要使用的地方再绑定VAO
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
    // 第一个参数指定从索引0开始取数据，与顶点着色器中layout(location=0)对应
    // 第二个参数指定顶点属性的组件数
    // 第三个参数指定数据类型
    // 第四个参数定义是否希望数据被标准化（归一化）， 标准数据是在(-1 , 1)之间的。
    // 第五个参数是步长（Stride），指定在连续的顶点属性之间的间隔
    // 第六个参数表示我们的位置数据在缓冲区起始位置的偏移量
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // 以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的
    glEnableVertexAttribArray(0);

    // 可以解绑 VBO, 因为在 glVertexAttribPointer 中已经指明了注册 VBO。
    // 注册暂且理解为知道了如何解析VBO这一片内存
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // EBO不可以被解绑
    // 暂且理解为没被注册，即没有存储如何理解EBO内存的数据，所以不能被解绑
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // 可以解绑 VAO，避免 VAO 被其他地方修改
    glBindVertexArray(0);
}

void DrawTriangle(unsigned int shaderProgram, unsigned int VAO) {
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    // glDrawArrays(GL_TRIANGLES, 0, 3); 若使用了EBO，则不能直接DrawArrays

    // 第一个参数指定了我们绘制的模式 GL_TRIANGLES 画三角形
    // 第二个参数是我们打算绘制顶点的个数 sizeof(indices) = 6 个顶点
    // 第三个参数是索引的类型
    // 指定EBO中的偏移量
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}