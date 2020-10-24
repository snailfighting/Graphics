#include <iostream>
#include <iomanip>
#include <vector>
#include<GLFW/glfw3.h> 
#include<Windows.h> 
#include<iterator>
#include <math.h>
#include <tuple>


using namespace std;

int sgn(int val) {
    return (val > 0) ? (1) : ((val < 0) ? (-1) : (0));
}

namespace geometry {
    struct Color
    {
        float red, green, blue;
    };

    const Color RED{ 1.0f, 0.0f, 0.0f }, GREEN{ 0.0f, 1.0f, 0.0f }, BLUE{ 0.0f, 0.0f, 1.0f }, WHITE{ 1.0f, 1.0f, 1.0f };

    struct Point2F
    {
        double x;
        double y;
    public:
        double dot(Point2F p) {
            return x * p.x + y * p.y;
        }
        double cross(Point2F p) {
            return x * p.y - p.x * y;
        }
        double len() {
            return sqrt(y * y + x * x);
        }
        friend ostream& operator<< (ostream& os, Point2F& p)
        {
            os << "(" << p.x << ", " << p.y << ")";
            return os;
        };

        Point2F operator+ (Point2F p) {
            return { x + p.x, y + p.y };
        };

        Point2F operator- (Point2F p) {
            return { x - p.x, y - p.y };
        };

        Point2F operator* (double t) {
            return { x * t, y * t };
        };
    };

    struct Segment
    {

        Point2F a, b;

        Point2F dir() {
            return { b.x - a.x, b.y - a.y };
        }

        Point2F normal() {
            Point2F n_dir{ b.y - a.y , a.x - b.x };
            return n_dir * (1 / dir().len());
        };
        bool is_left(Point2F p) {
            return dir().cross(p - a) >= 0;
        }

        double intersection_parameter(Segment edge) {
            Point2F f = edge.a - a, seg_dir = dir(), edge_dir = edge.dir();
            double t = edge_dir.cross(f) / edge_dir.cross(seg_dir);
            if (isnan(t)) {
                t = 0;
            };
            return t;
        }

        void morph(double ta, double tb) {
            Point2F d = dir(), start = a;
            a = start + d * ta;
            b = start + d * tb;
        };

        /*SegmentPair cut(double ta, double tb) {
            Point2F d = dir();
            return { { a, a + (d * ta) }, { a + (d * tb), b } };
        };*/
    };
    /*
    struct SegmentPair {
        Segment s1;
        Segment s2;
    };
    struct Polygon : vector < Point2F > {};
    struct DividedPolygon {
        Polygon convex_extension;
        vector < Polygon > concave_exclusions;
    };*/

    struct Polygon : vector < Point2F > {
        bool is_convex() {
            int len = size();
            if (len >= 3) {
                for (int a = len - 2, b = len - 1, c = 0; c < len; a = b, b = c, ++c) {
                    Segment edge{ (*this)[a], (*this)[b] };
                    if (!edge.is_left((*this)[c]))
                        return false;
                }
            }
            return true;
        };
        
        vector < Segment > edges() {
            vector < Segment > v;
            int len = size();
            for (int i = 0; i < len - 1; i++) {
                v.push_back({ (*this)[i], (*this)[i + 1] });
            };

            if (len > 2) {
                v.push_back({ (*this)[len - 1], (*this)[0] });
            }
            return v;
        };

        vector < Polygon > divide() { 
            // To Do: здесь выпуклое дополнение до выпуклоого многоугольника всегда хранитьс€ в первой €чейке вектора, затем следуют
            // вырезанные куски по пор€дку. Ёто пиздец, надо сделать отдельну красивую структуру
            Polygon convex;
            vector < Polygon > polygons;
            int len = size();
            if (len > 3) {
                convex.push_back((*this)[0]);
                for (int a = len - 2, b = len - 1, c = 0; c < len; a = b, b = c, ++c) {
                    Segment edge{ (*this)[a], (*this)[b] };
                    if (!edge.is_left((*this)[c])) {
                        Polygon concave;
                        while (!edge.is_left((*this)[c])) {
                            concave.push_back((*this)[b]);
                            ++b, ++c;
                            edge.b = (*this)[b];
                        }
                        concave.push_back((*this)[b]);
                        concave.push_back((*this)[a]);
                        reverse(concave.begin(), concave.end());
                        polygons.push_back(concave);
                    }
                    convex.push_back((*this)[b]);
                }
            }
            polygons.insert(polygons.begin(), convex);
            return polygons;
        }
        vector < Segment > segments() {
            vector < Segment > v;
            int len = size();
            for (int i = 0; i < len / 2; i++ ) {
                v.push_back({ (*this)[2 * i], (*this)[2 * i + 1] });
            };
            return v;
        }

        bool cyrus_beck_clip(Segment &subject) {
            Point2F subj_dir = subject.dir();
            //auto division = divide();
            // —начвла осуществим внутреннее отсечение по дополненному до выпуклого многоугольнику
            vector < Segment > v = edges();
            double ta = 0.0, tb = 1.0;
            for (auto i = v.begin(); i != v.end(); ++i) {
                Segment edge = *i;
                double t = subject.intersection_parameter(edge);
                switch (sgn(edge.normal().dot(subj_dir)))
                {
                    case 1:
                        if (t > ta) ta = t;
                        break;
                    case -1:
                        if (t < tb) tb = t;
                        break;
                    case 0:
                        if (!edge.is_left(subject.a)) return false;
                        break;
                }
            }
            if (ta > tb) return false;
            subject.morph(ta, tb);
            return true;
        }

        vector < Segment > cyrus_beck_clip(vector < Segment > subjects) {
            /*if (!is_convex()) {
                throw new runtime_error("Should be convex");
            }*/
            vector < Segment > clippedSubjects;
            for (auto i = subjects.begin(); i != subjects.end(); ++i) {
                Segment subject = *i;
                if (cyrus_beck_clip(subject)) clippedSubjects.push_back(subject);
            }
            return clippedSubjects;
        }
    };
}

using namespace geometry;

geometry::Polygon v, s;

const int width = 1366;
const int height = 768;


int xpos, ypos;
bool segment_mode = false, clip = false, step = false, clean = false, mode_switched = false;

float pixel[3 * width * height]; // матрица пикселей
bool mark[width][height];

int Brez = 0, Seg = 0;

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    cout << string(100, '\n') << "x: " << xpos << "   y: " << ypos;   
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) 
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        if (segment_mode) s.push_back({ xpos , height - ypos });
        else              v.push_back({ xpos , height - ypos });
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) 
    {
        if (key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window, GL_TRUE);
        if (key == 'W') {
            clip ^= true; // ќбрезание
            mode_switched = true;
        }
        if (key == 'R')
            segment_mode ^= true;// –исование фигуры/отрезка
        if (key == 'A')
            clean = true;
        cout << segment_mode;
    }
}


void Set(int x, int y, Color rgb) {
    int index = 3 * (y * width + x);
    memcpy(pixel + index, &rgb, 3 * sizeof(float));
}


void draw() {
    glDrawPixels(width, height, GL_RGB, GL_FLOAT, &pixel);
}

//////////////////////**** Ѕ–≈«≈Ќ’≈ћ ****//////////////////////////


void Bresenham(int x0, int y0, int x1, int y1, Color rgb) {
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

    Set(x, y, rgb);

    for (int i = 1; i < dx; i++) {
        if (e < 0) {
            if (ch) {
                y = y + s2;
                // здесь мы уверены, что в данной горизонтальной линии содержитс§ лишь один писксель ребра, закршаиваем его
                if (y != y0 && y != y1) mark[x][y] ^= true; // значени€ в врешинах обновл€ютс€ отдельно
            }
            else {
                x = x + s1;
            }
            e = e + A;
        }
        else {
            y = y + s2;
            x = x + s1;
            // здесь заканчиваетс€ горизонтальна€ лини€, полностью содержаща€с€ в ребре, помечаем лишь единственный (начальный) пиксель в следующей линии
            if (y != y0 && y != y1) mark[x][y] ^= true;
            e = e + B;
        }
        Set(x, y, rgb);
    }

    // обновл€ютс€ лишь верхние точки ребра, причем с учетом предыдущего значени€
    if (y0 > y1) mark[x0][y0] ^= true;
    else mark[x1][y1] ^= true;
}

void Bresenham(int x0, int y0, int x1, int y1) {
    Bresenham(x0, y0, x1, y1, WHITE);
};
void Bresenham(Point2F p1, Point2F p2, Color rgb) {
    Bresenham(floor(p1.x), floor(p1.y), floor(p2.x), floor(p2.y), rgb);
};
void Bresenham(Point2F p1, Point2F p2) {
    Bresenham(p1, p2, WHITE);
}
void Bresenham(Segment s, Color rgb) {
    Bresenham(s.a, s.b, rgb);
}
void Bresenham(Segment s) {
    Bresenham(s, WHITE);
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
    
    if (Brez != v.size() || Seg / 2 != s.size() / 2 || mode_switched) {
        memset(pixel, 0.0f, sizeof(float) * (width * height * 3));
     
        vector < Segment > shape = v.edges();

        for (auto iter = shape.begin(); iter != shape.end(); ++iter) {
            Segment edge = *iter;
            Bresenham(edge);
            Bresenham(edge.a, edge.a + (edge.normal() * 10), RED);  
        }
        
        vector < Segment > segments = s.segments();
        if (!clip) {
            for (auto iter = segments.begin(); iter != segments.end(); ++iter)
                Bresenham(*iter);
        }
        else {
            vector < Segment > clipped_segments = v.cyrus_beck_clip(segments);
            for (auto iter = clipped_segments.begin(); iter != clipped_segments.end(); ++iter)
                Bresenham(*iter);
        }
        mode_switched = false;
        Seg = s.size();
        Brez = v.size();
    }
    
    draw();


    if (clean) {
        memset(pixel, 0.0f, sizeof(float) * (width * height * 3));
        v.clear();
        s.clear();

        glClear(GL_COLOR_BUFFER_BIT);

        clean = false;

    }

    glLoadIdentity();
}


int main() {
    Point2F p1{ 10, 12 }, p2{ -8, -10 };
    auto p3 = p1 + p2;
    cout << p3;

    if (!glfwInit()) exit(EXIT_FAILURE);

    GLFWwindow* window = glfwCreateWindow(width, height, "Window", NULL, NULL);
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
    return 0;
};
