#define _USE_MATH_DEFINES // for C++
#include <cmath>
#include <GLFW/glew.h>
#include<GLFW/glfw3.h>
#include<Windows.h>
#include<gl/GLU.h>
#include<stdio.h>
#include <iostream>
#include <vector>
#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"glfw3.lib")
#pragma comment(lib,"opengl32.lib")

#include <GLFW/glm/glm.hpp>
#include "GLFW/glm/gtc/matrix_transform.hpp"

const GLuint WIDTH = 640, HEIGHT = 480;
GLfloat Xaxis = 0.f, Yaxis = 0.f;
GLfloat scale = 1.f;
GLfloat alpha = 0.f, beta = 0.f;
bool full = true;
GLuint VBO, VAO;
glm::mat4 projection = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f * cos(45), 0.5f * sin(45), 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

glm::mat4 figure;
const GLchar* vertexShaderSource = "#version 330\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec3 color;\n"
"out vec3 fragmentColor;\n"
"uniform mat4 figure;\n"
"void main()\n"
"{\n"
"gl_Position = figure * vec4(position, 1.0f);\n"
"fragmentColor = color;"
"}\0";

const GLchar* fragmentShaderSource = "#version 330\n"
"in vec3 fragmentColor;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(fragmentColor, 1.0);\n"
"}\0";

void rebuild() {
    //МАСШТАБ 
    figure = glm::scale(projection, glm::vec3(scale, scale, scale));
    //ПЕРЕНОС
    figure = glm::translate(figure, glm::vec3(Xaxis, Yaxis, 0));
    //ПОВОРОТ
    figure = glm::rotate(figure, alpha, glm::vec3(0, 1, 0));
    figure = glm::rotate(figure, beta, glm::vec3(1, 0, 0));
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    
    if (key == GLFW_KEY_LEFT ) {
        alpha += 0.5;
    }
    if (key == GLFW_KEY_RIGHT) {
        alpha -= 0.5;
    }
    if (key == GLFW_KEY_UP) {
        beta += 0.5;
    }
    if (key == GLFW_KEY_DOWN ) {
        beta -= 0.5;
    }
    if (key == 'A' ) {
        Xaxis -= 0.1;
    }
    if (key == 'D') {
        Xaxis += 0.1;
    }
    if (key == 'W') {
        Yaxis += 0.1;
    }
    if (key == 'S') {
        Yaxis -= 0.1;
    }
    if (key == '=') {
        scale += 0.1;
    }
    if (key == '-') {
        scale -= 0.1;
    }
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        if (full)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        full = !full;
    }
    rebuild();
}

void display() {
    GLdouble vertices[] = {
        ////////////////////////////////// FRONT
        0.3f, 0.3f, -0.3f, 1., 0., 0.,
        -0.3f, -0.3f, -0.3f, 1., 0., 0.,
        -0.3f, 0.3f, -0.3f, 1., 0., 0.,

        0.3f, 0.3f, -0.3f, 1., 0., 0.,
        0.3f, -0.3f, -0.3f, 1., 0., 0.,
        -0.3f, -0.3f, -0.3f, 1., 0., 0.,
        ////////////////////////////////////LEFT
        -0.3f, -0.3f, -0.3f, 0., 1., 0.,
        -0.3f, -0.3f, 0.3f, 0., 1., 0.,
        -0.3f, 0.3f, 0.3f, 0., 1., 0.,

        -0.3f, -0.3f, -0.3f, 0., 1., 0.,
        -0.3f, 0.3f, 0.3f, 0., 1., 0.,
        -0.3f, 0.3f, -0.3f, 0., 1., 0.,
        /////////////////////////////////BOTTOM
        0.3f, -0.3f, 0.3f, 0., 0., 1.,
        -0.3f, -0.3f, -0.3f, 0., 0., 1.,
        0.3f, -0.3f, -0.3f, 0., 0., 1.,

        0.3f, -0.3f, 0.3f, 0., 0., 1.,
        -0.3f, -0.3f, 0.3f, 0., 0., 1.,
        -0.3f, -0.3f, -0.3f, 0., 0., 1.,
        //////////////////////////////////RIGHT
        0.3f, 0.3f, 0.3f, 1., 1., 1.,
        0.3f, -0.3f, -0.3f, 1., 1., 1.,
        0.3f, 0.3f, -0.3f, 1., 1., 1.,

        0.3f, -0.3f, -0.3f, 1., 1., 1.,
        0.3f, 0.3f, 0.3f, 1., 1., 1.,
        0.3f, -0.3f, 0.3f, 1., 1., 1.,
        ////////////////////////////////// TOP
        0.3f, 0.3f, 0.3f, 0., 0., 1.,
        0.3f, 0.3f, -0.3f, 0., 0., 1.,
        -0.3f, 0.3f, -0.3f, 0., 0., 1.,

        0.3f, 0.3f, 0.3f, 0., 0., 1.,
        -0.3f, 0.3f, -0.3f, 0., 0., 1.,
        -0.3f, 0.3f, 0.3f, 0., 0., 1.,
        ///////////////////////////////// BACK
        -0.3f, 0.3f, 0.3f, 1., 0., 0.,
        -0.3f, -0.3f, 0.3f, 1., 0., 0.,
        0.3f, -0.3f, 0.3f, 1., 0., 0.,

        0.3f, 0.3f, 0.3f, 1., 0., 0.,
        -0.3f, 0.3f, 0.3f, 1., 0., 0.,
        0.3f, -0.3f, 0.3f, 1., 0., 0.,
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 6 * sizeof(GLdouble), (GLvoid*)0);
    glEnableVertexAttribArray(0);//включаю массив атритутов
    glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 6 * sizeof(GLdouble), (GLvoid*)(3 * sizeof(GLdouble)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


int main() {
    if (!glfwInit())exit(EXIT_FAILURE);

    GLFWwindow* window = glfwCreateWindow(640, 480, "Window", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    //   LoadTextures();
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    glewInit();

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    // ! Создаем вершинный шейдер
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // ! Передаем исходный код
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    // ! Компилируем шейдер
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // ! Создаем программу и прикрепляем шейдеры к ней
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    GLint success;
    GLchar infoLog[512];
    // ! Линкуем шейдерную программу
    glLinkProgram(shaderProgram);
    // ! Проверяем статус сборки
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER PROGRAM LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLuint matrix = glGetUniformLocation(shaderProgram, "figure");
    display();
    rebuild();

    glUseProgram(shaderProgram);
    while (!glfwWindowShouldClose(window)) {
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Передать наши трансформации в текущий шейдер
        glUniformMatrix4fv(matrix, 1, GL_FALSE, &figure[0][0]);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}