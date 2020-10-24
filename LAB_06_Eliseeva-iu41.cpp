#include <iostream>
#include <iomanip>
#include <vector>
#include <GLFW/glfw3.h> 
#include <Windows.h> 
#include <GLFW/glut.h> 
#include <GLFW/SOIL.h>
#include <math.h> 

// был отключен SDL

#pragma comment(lib,"glut32.lib") 
#pragma comment(lib,"glfw3.lib") 
#pragma comment(lib,"opengl32.lib") 
#pragma comment(lib,"SOIL.lib") 


using namespace std;

float alpha, beta;// поворот вдоль оси; 
float Xaxis = 0.f, Yaxis = 0.f;// перенос вдоль оси x и y 
float sc = 16.0, xpos, ypos;// масштаб// получение координат курсора 
bool full = true, start = false, tex = false;// наличие каркаса / анимация /текстура
float lightX = 0.f, lightY = 0.f;

float amb = 0.f, dif = 1.f, sp = 1.f; // компоненты освещения

float circles = 2.f;// количество вертикальных разбиений

float break_vertex[4][2] = { {-0.45f, 0.5f}, {0.45f, 0.5f},{0.2f, 0.f},{-0.2f, 0.f} }; // точки соприкосновения


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

struct Bazet { /// верхнее основание
    float x_coor1;
    float y_coor1;
    float h1;
};

struct Bazem {  //// нижнее основание 
    float x_coor2;
    float y_coor2;
    float h2;
};

vector < Pointr > vr;
vector < Point > v;
vector < Bazet > v1;
vector < Bazem > v2;

const double PI = 3.141592653589793;



void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    sc += yoffset;
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

    //////////////////////////*** ДВИЖЕНИЕ ***//////////////////////////
    if (key == 'A')
        Xaxis -= 0.1;
    if (key == 'D')
        Xaxis += 0.1;
    if (key == 'W')
        Yaxis += 0.1;
    if (key == 'S')
        Yaxis -= 0.1;

    ///////////////////////////*** РАЗБИЕНИЕ ***///////////////////////////
    if (key == 'C')
        circles += 1.f;
    if (key == 'V')
        circles -= 1.f;

    ////////////////////////*** АНИМАЦИЯ ***/////////////////////////////
    if (key == GLFW_KEY_SPACE) {
        start = true;
    }
    if (key == 'Z')
        start = false;

    ///////////////////////*** ВРАЩЕНИЕ ***////////////////////////
    if (key == GLFW_KEY_UP)
        alpha += 2.0;
    if (key == GLFW_KEY_DOWN)
        alpha -= 2.0;
    if (key == GLFW_KEY_LEFT)
        beta -= 2.0;
    if (key == GLFW_KEY_RIGHT)
        beta += 2.0;

    ///////////////////////////*** ТЕКСТУРА ***///////////////////////////
    if (key == 'T')
        tex = true;
    if (key == 'Y')
        tex = false;

    ///////////////////////////*** СВЕТ ***///////////////////////////
    if (key == GLFW_KEY_1)
        amb = 0.f;
    if (key == GLFW_KEY_2)
        amb = 1.f;

    if (key == GLFW_KEY_3)
        dif = 0.f;
    if (key == GLFW_KEY_4)
        dif = 1.f;

    if (key == GLFW_KEY_5)
        sp = 0.f;
    if (key == GLFW_KEY_6)
        sp = 1.f;

}

/////////////////////////////////////////////// СОЗДАНИЕ ФИГУРЫ /////////////////////////////////////////
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

////////////////////////////////////////////////////////// РИСОВАНИЕ ФИГУРЫ /////////////////////////////////////////////

void draw() {

    if (!full) glBegin(GL_LINE_LOOP);
    glNormal3f(0.f, 0.f, 1.0f);
    //  glColor3f(0.5f, 0.5f, 0.5f);
    for (int i = 0; i < v.size(); i++) {
        glVertex3f(v[i].x_coor, v[i].h, v[i].y_coor);
    }
    glEnd();
    int count1 = 0, count2 = 0;

    if (!full) glBegin(GL_LINES);
    if (full) glBegin(GL_TRIANGLE_STRIP);
    glNormal3f(0.f, 0.f, 1.0f);
    for (int i = 0; i < vr.size(); i++) {
        if (tex) { ////////////////////***** НАЛОЖЕНИ ТЕКСТУРЫ *****///////////////////////////////
            if (vr[i].hr == 0.0f) {
                glTexCoord2f(count1 / (2 * sc), 0.0f);
                count1++;
            }
            if (vr[i].hr == 0.5f) {
                glTexCoord2f(count2 / (2 * sc), 1.0f);
                count2++;
            }
        }//////////////////////////////////////////////////////////////////////////////////////////
        glVertex3f(vr[i].x_coorr, vr[i].hr, vr[i].y_coorr);

    }
    glEnd();

    if (!full) glBegin(GL_LINE_LOOP);
    if (full) glBegin(GL_TRIANGLE_STRIP);
    glNormal3f(0.f, 0.f, 1.0f);
    // glColor3f(1.f, 1.f, 1.f);
    for (int i = 0; i < v1.size(); i++) {
        glVertex3f(v1[i].x_coor1, v1[i].h1, v1[i].y_coor1);
    }
    glEnd();

    if (!full) glBegin(GL_LINE_LOOP);
    if (full) glBegin(GL_TRIANGLE_STRIP);
    glNormal3f(0.f, 0.f, 1.0f);
    //  glColor3f(1.f, 1.f, 1.f);
    for (int i = 0; i < v2.size(); i++) {
        glVertex3f(v2[i].x_coor2, v2[i].h2, v2[i].y_coor2);
    }
    glEnd();
}


float transX = 0.0001;
float transY = 0.0001;

float speed = 0.0001;
bool pos_direction = true, neg_direction = true;
void animate() {

    for (int i = 0; i < 4; i++) {
        if (break_vertex[i][0] >= 0.6f) {
            transX -= speed;
            glTranslatef(transX, 0, 0.f);
            for (int i = 0; i < 4; i++) {
                break_vertex[i][0] -= speed;
            }
            pos_direction = true;
        }
        if (break_vertex[i][0] <= -0.6f) {
            transX += speed;
            glTranslatef(transX, 0, 0.f);
            for (int i = 0; i < 4; i++) {
                break_vertex[i][0] += speed;
            }
            pos_direction = false;
        }
        else {
            if (pos_direction) {
                transX -= speed;
                glTranslatef(transX, 0, 0.f);
                for (int i = 0; i < 4; i++) {
                    break_vertex[i][0] -= speed;
                }
            }
            else {
                transX += speed;
                glTranslatef(transX, 0, 0.f);
                for (int i = 0; i < 4; i++) {
                    break_vertex[i][0] += speed;
                }
            }
        }
        /////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////

        if (break_vertex[i][1] >= 0.65f) {
            transY -= speed;
            glTranslatef(0, transY, 0.f);
            for (int i = 0; i < 4; i++) {
                break_vertex[i][1] -= speed;
            }
            neg_direction = true;
        }
        if (break_vertex[i][1] <= -0.25f) {
            transY += speed;
            glTranslatef(0, transY, 0.f);
            for (int i = 0; i < 4; i++) {
                break_vertex[i][1] += speed;
            }
            neg_direction = false;
        }
        else {
            if (neg_direction) {
                transY -= speed;
                glTranslatef(0, transY, 0.f);
                for (int i = 0; i < 4; i++) {
                    break_vertex[i][1] -= speed;
                }
            }
            else {
                transY += speed;
                glTranslatef(0, transY, 0.f);
                for (int i = 0; i < 4; i++) {
                    break_vertex[i][1] += speed;
                }
            }
        }

    }
}



void light() {
    float lpos[] = { -1.0f, -0.5f, 1.f, 1.f };

    //float ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    float ambient[] = { amb, amb, amb, 1.0f };
    float black[] = { 0.f, 0.f, 0.f, 1.f };

    //float diffuse[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float diffuse[] = { dif, dif, dif, 1.0f };

    //float spec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float spec[] = { sp, sp, sp, 1.0f };
    float specref[] = { 1.0f, 1.0f, 1.0f, 1.0f };


    glColor3f(1.0, 1.0, 1.0);
    glEnable(GL_NORMALIZE); //Automatically normalize normals
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, lpos); // координаты источника света
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient); // фоновая подсветка

    glLightfv(GL_LIGHT0, GL_AMBIENT, black);
    // glLightfv(GL_LIGHT0, GL_AMBIENT, black); // фон при дифуз
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse); // диффузное 
    glLightfv(GL_LIGHT0, GL_SPECULAR, spec);


    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specref);
    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 128);

    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 2);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.00001);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.00001);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glColorMaterial(GL_FRONT_AND_BACK, GL_SHININESS);
}



void display(GLFWwindow* window) {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    glViewport(0, 0, width, height);// resize 
    glDepthRange(0, 1);
    float ratio = width / (float)height;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);// МАТРИЦА ПРОЕКЦИЙ

    glLoadIdentity();
    glOrtho(-ratio, ratio, -1.f, 1.f, 10.f, -10.f);

    glMatrixMode(GL_MODELVIEW); // ВИДОВАЯ МАТРИЦА

    //ПЕРЕНОС
    glTranslatef(Xaxis, Yaxis, 0);
    //ПОВОРОТ
    glRotatef(alpha, 1, 0, 0);
    glRotatef(beta, 0, 1, 0);


    if (start) animate(); //////**** АНИМАЦИЯ 
    light(); //////////////////***** СВЕТ

    glEnable(GL_TEXTURE_2D);

    static GLuint tex_ID = -1;
    if (tex_ID == -1)
        tex_ID = SOIL_load_OGL_texture(
            "pers.JPG",
            0,
            SOIL_CREATE_NEW_ID,
            SOIL_FLAG_POWER_OF_TWO
            | SOIL_FLAG_MIPMAPS
        );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, tex_ID);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

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
    //   LoadTextures();
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    // glfwSetCursorPosCallback(window, cursor_position_callback);
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
