#include <iostream>
#include <iomanip>
#include <vector>
#include<GLFW/glfw3.h> 
#include<Windows.h> 
#include<GLFW/glut.h> 
#include <math.h> 


using namespace std;



struct Point
{
    int x_coor;
    int y_coor;
};

vector < Point > v;

int width = 1366;
int height = 768;


int xpos, ypos;
bool mouse_click = false, filling = false, step = false, clean = false;

float pixel[3 * 1366 * 768]; // матрица пикселей
bool mark[1366][768];

int Brez = 0;




static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    double x = xpos, y = ypos;
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glfwGetCursorPos(window, &xpos, &ypos);
    if (mouse_click) {
        v.push_back({ (int)xpos , height - (int)ypos });
    }
    cout << string(100, '\n') << "x: " << xpos << "(" << x << ") y: " << ypos << "(" << y << ")";
    mouse_click = false;

}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        mouse_click = true;
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key == 'W')
        filling = true;// ракраска
    if (key == 'R')
        step = true;// устранение ступенчаости
    if (key == 'A')
        clean = true;
}


void Set(int x, int y, float e) {

    int index = 3 * (y * 1366 + x);

    for (int i = 0; i < 3; i++) {
        pixel[index + i] = e;
    }
}


void draw() {

    glDrawPixels(1366, 768, GL_RGB, GL_FLOAT, &pixel);
}

//////////////////////**** БРЕЗЕНХЕМ ****//////////////////////////

int sgn(int val) {
    return (val > 0) ? (1) : ((val < 0) ? (-1) : (0));
}

void Bresenham(int x0, int y0, int x1, int y1) {
    int x = x0;
    int y = y0;
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int s1 = sgn(x1 - x0);
    int s2 = sgn(y1 - y0);

    bool ch = false;

    if (dy > dx) {
        int buff = dx;
        dx = dy;
        dy = buff;
        ch = true;
    }
    else ch = false;

    int e = 2 * dy - dx;
    int A = 2 * dy;
    int B = 2 * dy - 2 * dx;

    Set(x, y, 1.0f);

    for (int i = 1; i < dx; i++) {
        if (e < 0) {
            if (ch) {
                y = y + s2;
                // здесь мы уверены, что в данной горизонтальной линии содержитс¤ лишь один писксель ребра, закршаиваем его
                if (y != y0 && y != y1) mark[x][y] ^= true; // значения в врешинах обновляются отдельно
            }
            else {
                x = x + s1;
            }
            e = e + A;
        }
        else {
            y = y + s2;
            x = x + s1;
            // здесь заканчивается горизонтальная линия, полностью содержащаяся в ребре, помечаем лишь единственный (начальный) пиксель в следующей линии
            if (y != y0 && y != y1) mark[x][y] ^= true;
            e = e + B;
        }
        Set(x, y, 1.0f);
    }

    // обновляются лишь верхние точки ребра, причем с учетом предыдущего значения
    if (y0 > y1) mark[x0][y0] ^= true;
    else mark[x1][y1] ^= true;
}

//////////////////////////////**** Устранение ступенчатости БРЕЗЕНХЕМОМ ****///////////////////////////////

void plot_line_low_aa(int x0, int y0, int x1, int y1, bool f) {


    int  dx = x1 - x0;
    int  dy = y1 - y0;
    int yi = 1;
    if (dy < 0) {
        yi = -1;
        dy = -dy;
    }
    float e = 0.5f;
    float l = 1.0f;

    if (dx == 0) {
        return;
    }


    float m = (float)dy / (float)dx;
    float w = 1.0f - (float)m;

    int y = y0;

    for (int x = x0; x < x1 + 1; x++) {

        if (f) {
            Set(x, y, l - e);
        }
        else {
            Set(x, y, e);
        }

        if (e >= w) {
            e -= w;
            y += yi;
        }
        else e += m;
    }
}

void plot_line_high_aa(int x0, int y0, int x1, int y1, bool f) {

    int  dx = x1 - x0;
    int  dy = y1 - y0;
    int xi = 1;
    if (dx < 0) {
        xi = -1;
        dx = -dx;
    }
    float e = 0.5f;
    float l = 1.0f;

    if (dy == 0) {
        return;
    }

    float m = (float)dx / (float)dy;
    float w = 1.0f - (float)m;

    int x = x0;


    for (int y = y0; y < y1 + 1; y++) {
        if (f) {
            Set(x, y, l - e);
        }
        else {
            Set(x, y, e);
        }

        if (e >= w) {
            e -= w;
            x += xi;
        }
        else e += m;

    }

}

void plot_line_aa(int x0, int y0, int x1, int y1) {


    if (abs(y1 - y0) < abs(x1 - x0)) {
        if (x0 > x1) {
            if (y0 > y1) {
                plot_line_low_aa(x1, y1, x0, y0, false);
            }
            else plot_line_low_aa(x1, y1, x0, y0, true);
        }
        else {
            if (y0 > y1) plot_line_low_aa(x0, y0, x1, y1, false);
            else plot_line_low_aa(x0, y0, x1, y1, true);
        }
    }
    else {
        if (y0 > y1) {
            if (x0 > x1) plot_line_high_aa(x1, y1, x0, y0, true);
            else plot_line_high_aa(x1, y1, x0, y0, false);
        }
        else {
            if (x0 < x1) plot_line_high_aa(x0, y0, x1, y1, false);
            else plot_line_high_aa(x0, y0, x1, y1, true);
        }
    }
}

//////////////////////////////////// * * * ЗАПОЛНЕНИЕ * * * /////////////////////////////////////

void fill() {
    bool flag = false;

    for (int j = 0; j < 768; j++) {
        flag = false;
        for (int i = 0; i < 1366 - 1; i++) {
            if (mark[i][j]) {
                if (flag) flag = false;
                else flag = true;
            }
            if (flag) Set(i, j, 1.f);

        }
    }

    filling = false;

}

///////////////////////////////////////////////////////////////////////

void display(GLFWwindow* window) {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    glViewport(0, 0, width, height);// resize 
    glDepthRange(0, 1);


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();



    if (Brez != v.size()) {
        memset(pixel, 0.0f, sizeof(float) * (1366 * 768 * 3));

        for (int i = 0; i < 1366; i++) {
            for (int j = 0; j < 768; j++) {
                mark[i][j] = false;
            }
        }

        if (!step) {
            if (v.size() >= 2) {
                for (int i = 0; i < v.size() - 1; i++) {
                    Bresenham(v[i].x_coor, v[i].y_coor, v[i + 1].x_coor, v[i + 1].y_coor);
                }
                Bresenham(v[v.size() - 1].x_coor, v[v.size() - 1].y_coor, v[0].x_coor, v[0].y_coor);
            }
        }

        Brez = v.size();
    }

    if (step) {
        if (v.size() >= 2) {
            for (int i = 0; i < v.size() - 1; i++) {
                plot_line_aa(v[i].x_coor, v[i].y_coor, v[i + 1].x_coor, v[i + 1].y_coor);
            }
            plot_line_aa(v[v.size() - 1].x_coor, v[v.size() - 1].y_coor, v[0].x_coor, v[0].y_coor);
        }

    }

    if (filling) fill();
    draw();


    if (clean) {

        step = false;
        filling = false;
        v.clear();
        glClear(GL_COLOR_BUFFER_BIT);


        for (int i = 0; i < 1366; i++) {
            for (int j = 0; j < 768; j++) {
                mark[i][j] = false;
            }
        }
        clean = false;

    }

    glLoadIdentity();
}


void main() {

    if (!glfwInit()) exit(EXIT_FAILURE);

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
