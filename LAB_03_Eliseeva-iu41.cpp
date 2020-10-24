#include <iostream>
#include <iomanip>
#include <vector>
#include<GLFW/glfw3.h> 
#include<Windows.h> 
#include<GLFW/glut.h> 
#include <math.h> 

#pragma comment(lib,"glut32.lib") 
#pragma comment(lib,"glfw3.lib") 
#pragma comment(lib,"opengl32.lib") 

using namespace std;

float alpha, beta;// поворот вдоль оси; 
float Xaxis = 0.f, Yaxis = 0.f;// перенос вдоль оси x и y 
float sc = 16.0, xpos, ypos;// масштаб// получение координат курсора 
bool full = true;// наличие каркаса / изменения 

float circles = 2.f;// количество вертикальных разбиений

struct Point
{
    float x_coor;
    float y_coor;
    float h;
};

struct Pointr
{
    float x_coorr;
    float y_coorr;
    float hr;
};

struct Bazet {
    float x_coor1;
    float y_coor1;
    float h1;
};

struct Bazem {
    float x_coor2;
    float y_coor2;
    float h2;
};


vector < Pointr > vr;
vector < Point > v;
vector < Bazet > v1;
vector < Bazem > v2;

const double PI = 3.141592653589793;

void create() {

    float x, y, r = 0.2, h = 0.5;

    float r_min = 0.2, r_max = 0.45, j = 0.0;

    for (float j = 0.0; j < h && r <= r_max; ) {
    

            for (float i = 0.0; i < 2 * PI;) {
                x = r * sin(i);
                y = r * cos(i);
                v.push_back({ x ,y, j });

                i += 2 * PI / sc;
            }

            x = r * sin(0);
            y = r * cos(0);

            v.push_back({ x ,y, j });

            if (circles == 0.0) circles = 1.f;
            r += 0.05 / (circles);
            j += 0.1 / (circles);
       
     }

    float help = r;
   

    for (float i = 0, r = 0.2; i < 2 * PI; ) {

        x = r * sin(i);
        y = r * cos(i);

        vr.push_back({ x ,y, 0.0 });

        x = help * sin(i);
        y = help * cos(i);

        vr.push_back({ x ,y, h });
      
        i += 2 * PI / sc;

        x = r * sin(i);
        y = r * cos(i);

        if (full) {
            vr.push_back({ x ,y, 0.0 }); 
            x = help * sin(i);
            y = help * cos(i);

            vr.push_back({ x ,y, h });
        }
    }

    for (r; r > 0.0;) {
        for (float i = 0.0; i < 2 * PI; ) {
            x = r * sin(i);
            y = r * cos(i);

          
            v1.push_back({ x , y, h });
            v1.push_back({ 0.0f , 0.0f, h });

            i += 2 * PI / sc;
            v1.push_back({ x , y , h });

        }
        x = r * sin(0);
        y = r * cos(0);
        v1.push_back({ x , y, h });

        r -= 0.05;
    }

    for (r = 0.2; r > 0.0;) {
        for (float i = 0.0; i < 2 * PI; ) {
            x = r * sin(i);
            y = r * cos(i);

      
            v2.push_back({ x , y , 0.0f });
            v2.push_back({ 0.0f , 0.0f , 0.0f });

            i += 2 * PI / sc;

            v2.push_back({ x , y , 0.0f });
        }
        x = r * sin(0);
        y = r * cos(0);
        v2.push_back({ x , y , 0.0f });

        r -= 0.05;
    }

}

void draw() {

    if (!full) glBegin(GL_LINE_LOOP);
    for (int i = 0; i < v.size(); i++) {
        glVertex3f(v[i].x_coor,v[i].h,v[i].y_coor);
    }
    glEnd();

    if (!full) glBegin(GL_LINES);
     if (full) glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i < vr.size(); i++) {
        glVertex3f(vr[i].x_coorr, vr[i].hr, vr[i].y_coorr);
    }
    glEnd();

    if (!full) glBegin(GL_LINE_LOOP);
    if (full) glBegin(GL_TRIANGLE_STRIP);
    glColor3f(0.f, 1.f, 0.f);
    for (int i = 0; i < v1.size(); i++) {
        glVertex3f(v1[i].x_coor1, v1[i].h1, v1[i].y_coor1);
    }
    glEnd();

    if (!full) glBegin(GL_LINE_LOOP);
    if (full) glBegin(GL_TRIANGLE_STRIP);
    glColor3f(0.f, 0.f, 1.f);
    for (int i = 0; i < v2.size(); i++) {
        glVertex3f(v2[i].x_coor2, v2[i].h2, v2[i].y_coor2);
    }
    glEnd();
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    sc += yoffset;
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glfwGetCursorPos(window, &xpos, &ypos);

    if (xpos > width / 2 && ypos > height / 2) {
        beta += 2.0;
    }
    if (xpos < width / 2 && ypos < height / 2) {
        alpha -= 2.0;
    }
    if (xpos > width / 2 && ypos < height / 2) {
        alpha -= 2.0;
        beta += 2.0;
    }
    if (xpos < width / 2 && ypos > height / 2) {
        alpha += 2.0;
        beta -= 2.0;
    }

}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (action != GLFW_PRESS) {
        full = true;
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        full = false;
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key == 'A')
        Xaxis -= 0.1;
    if (key == 'D')
        Xaxis += 0.1;
    if (key == 'W')
        Yaxis += 0.1;
    if (key == 'S')
        Yaxis -= 0.1;

    if (key == 'C')
        circles += 1.f;
    if (key == 'V')
        circles -= 1.f;
}

void display(GLFWwindow* window) {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    glViewport(0, 0, width, height);// resize 
    glDepthRange(0, 1);
    float ratio = width / (float)height;

    GLfloat MAT[16] = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, -1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
    };

    GLfloat MAT2[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f * cos(45), -0.5f * sin(45), 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    //*****//ФРОНТАЛЬНАЯ ДИМЕТРИЯ//******//

    glMatrixMode(GL_PROJECTION);// МАТРИЦА ПРОЕКЦИЙ
    glLoadIdentity();                                    //glRotated(30, 0, -1, 0);
    glLoadMatrixf(MAT);                                  //glRotated(10, 1, 0, 0);
    glOrtho(-ratio, ratio, -1.f, 1.f, 10.f, -10.f);      //glRotated(5, 0, 0, -1);
    glMultMatrixf(MAT2);


    glMatrixMode(GL_MODELVIEW); // ВИДОВАЯ МАТРИЦА
    glLoadIdentity();

    glBegin(GL_POLYGON);
    glColor3f(1.0f, 0.f, 0.f);
    glVertex3f(-1.1f, -0.6f, 0.f);
    glVertex3f(-1.1f, -0.4f, 0.f);
    glVertex3f(-0.9f, -0.4f, 0.f);
    glVertex3f(-0.9f, -0.6f, 0.f);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.f, 0.f, 1.0f);
    glVertex3f(-1.1f, -0.6f, 0.2f);
    glVertex3f(-1.1f, -0.4f, 0.2f);
    glVertex3f(-0.9f, -0.4f, 0.2f);
    glVertex3f(-0.9f, -0.6f, 0.2f);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-0.9f, -0.4f, 0.f);
    glVertex3f(-0.9f, -0.6f, 0.f);
    glVertex3f(-0.9f, -0.6f, 0.2f);
    glVertex3f(-0.9f, -0.4f, 0.2f);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.1f, -0.6f, 0.f);
    glVertex3f(-1.1f, -0.4f, 0.f);
    glVertex3f(-1.1f, -0.4f, 0.2f);
    glVertex3f(-1.1f, -0.6f, 0.2f);
    glEnd();
    glLoadIdentity();

    //ПЕРЕНОС
    glTranslatef(Xaxis, 0, 0);
    glTranslatef(0, Yaxis, 0);
    //ПОВОРОТ
    glRotatef(alpha, 1, 0, 0);
    glRotatef(beta, 0, 1, 0);
    //МАСШТАБ 
   // glScaled(sc, sc, sc);

    ///*********// КОНУСОИДНЫЙ ЦИЛИНДР //**********// 

    create();
    draw();
    v.clear();
    vr.clear();
    v1.clear();
    v2.clear();       

    glLoadIdentity();
}


void main() {

    if (!glfwInit())exit(EXIT_FAILURE);

    GLFWwindow* window = glfwCreateWindow(640, 480, "Window", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback);

    while (!glfwWindowShouldClose(window))
    {
        display(window);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);

}
