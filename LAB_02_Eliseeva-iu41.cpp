#include <GLFW/glfw3.h> 
#include <stdlib.h> 
#include <stdio.h> 

int alpha, beta;//поворот вдоль оси; 
float Xaxis = 0.f, Yaxis = 0.f;//перенос вдоль оси x и y 
double sc = 1, xpos, ypos;//масштаб//получение координат курсора
bool polygon = true;// наличие каркаса


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	sc += yoffset/5;
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glfwGetCursorPos(window, &xpos, &ypos);

	if (xpos > width / 2 && ypos > height / 2) {
		beta += 5;
	}
	if (xpos < width / 2 && ypos < height / 2) {
		alpha -= 5;
	}
	if (xpos > width / 2 && ypos < height / 2) {
		alpha -= 5;
		beta += 5;
	}
	if (xpos < width / 2 && ypos > height / 2) {
		alpha += 5;
		beta -= 5;
	}
	
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if(/*button == GLFW_MOUSE_BUTTON_RIGHT && */ action != GLFW_PRESS) {
		polygon = true;
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		polygon = false;
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
}

void display(GLFWwindow* window) {
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	//Устанавливаем перспективу
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float ratio = width / (float)height;
	glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);

	//gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	//Устанавливаем угол взгляда
	glMatrixMode(GL_MODELVIEW);
	//Перенос 
	glTranslatef(Xaxis, 0, 0);
	glTranslatef(0, Yaxis, 0);
	//Поворот
	glRotatef(alpha, 1, 0, 0);
	glRotatef(beta, 0, 1, 0);
	//Масштаб
	glScaled(sc, sc, sc);

	if (polygon){

		glBegin(GL_POLYGON);
		glColor3f(1.0f, 0.f, 0.f);
			glVertex3f(-0.2f, -0.2f, 0.2f);
			glVertex3f(0.2f, -0.2f, 0.2f); 
			glVertex3f(0.2f, 0.2f, 0.2f); 
			glVertex3f(-0.2f, 0.2f, 0.2f);
		glEnd();

		glBegin(GL_POLYGON);
		glColor3f(0.f, 0.f, 1.0f);
			glVertex3f(-0.2f, -0.2f, -0.2f);
			glVertex3f(0.2f, -0.2f, -0.2f); 
			glVertex3f(0.2f, 0.2f, -0.2f); 
			glVertex3f(-0.2f, 0.2f, -0.2f);
		glEnd();

		glBegin(GL_POLYGON);
		glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(-0.2f, -0.2f, 0.2f); 
			glVertex3f(-0.2f, -0.2f, -0.2f);
			glVertex3f(-0.2f, 0.2f, -0.2f);
			glVertex3f(-0.2f, 0.2f, 0.2f);
		glEnd();

		glBegin(GL_POLYGON);
		glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(0.2f, -0.2f, 0.2f);
			glVertex3f(0.2f, -0.2f, -0.2f);
			glVertex3f(0.2f, 0.2f, -0.2f);
			glVertex3f(0.2f, 0.2f, 0.2f);
		glEnd();

		glBegin(GL_POLYGON);
		glColor3f(0.f, 1.f, 0.f);
			glVertex3f(-0.2f, -0.2f, -0.2f); 
			glVertex3f(0.2f, -0.2f, -0.2f);
			glVertex3f(0.2f, -0.2f, 0.2f);
			glVertex3f(-0.2f, -0.2f, 0.2f);
		glEnd();

		glBegin(GL_POLYGON);
		glColor3f(0.f, 1.f, 0.f);
			glVertex3f(-0.2f, 0.2f, -0.2f);
			glVertex3f(-0.2f, 0.2f, 0.2f); 
			glVertex3f(0.2f, 0.2f, 0.2f);
			glVertex3f(0.2f, 0.2f, -0.2f);
		glEnd();
	}
	else if (!polygon){

		glBegin(GL_LINE_LOOP);
		glColor3f(1.0f, 0.f, 0.f);
			glVertex3f(-0.2f, -0.2f, 0.2f);
			glVertex3f(0.2f, -0.2f, 0.2f);
			glVertex3f(0.2f, 0.2f, 0.2f);
			glVertex3f(-0.2f, 0.2f, 0.2f);
		glEnd();

		glBegin(GL_LINE_LOOP);
		glColor3f(0.f, 0.f, 1.0f);
			glVertex3f(-0.2f, -0.2f, -0.2f); 
			glVertex3f(0.2f, -0.2f, -0.2f);
			glVertex3f(0.2f, 0.2f, -0.2f);
			glVertex3f(-0.2f, 0.2f, -0.2f);
		glEnd();

		glBegin(GL_LINE_LOOP);
		glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(-0.2f, -0.2f, 0.2f);
			glVertex3f(-0.2f, -0.2f, -0.2f);
			glVertex3f(-0.2f, 0.2f, -0.2f);
			glVertex3f(-0.2f, 0.2f, 0.2f);
		glEnd();

		glBegin(GL_LINE_LOOP);
		glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(0.2f, -0.2f, 0.2f);
			glVertex3f(0.2f, -0.2f, -0.2f); 
			glVertex3f(0.2f, 0.2f, -0.2f);
			glVertex3f(0.2f, 0.2f, 0.2f);
		glEnd();

		glBegin(GL_LINE_LOOP);
		glColor3f(0.f, 1.f, 0.f);
			glVertex3f(-0.2f, -0.2f, -0.2f); 
			glVertex3f(0.2f, -0.2f, -0.2f);
			glVertex3f(0.2f, -0.2f, 0.2f); 
			glVertex3f(-0.2f, -0.2f, 0.2f);
		glEnd();

		glBegin(GL_LINE_LOOP);
		glColor3f(1.f, 1.f, 1.f);
			glVertex3f(-0.2f, 0.2f, -0.2f);
			glVertex3f(-0.2f, 0.2f, 0.2f); 
			glVertex3f(0.2f, 0.2f, 0.2f);
			glVertex3f(0.2f, 0.2f, -0.2f);
		glEnd();
	}	
	glLoadIdentity();

	//////////////////////////////////Демонстрация
	glRotatef(45, 1, 1, 0);

	glBegin(GL_LINE_LOOP);
	glColor3f(1.0f, 0.f, 0.f);
		glVertex3f(-1.1f, -0.6f, 0.f);
		glVertex3f(-1.1f, -0.4f, 0.f);
		glVertex3f(-0.9f, -0.4f, 0.f);
		glVertex3f(-0.9f, -0.6f, 0.f);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glColor3f(0.f, 0.f, 1.0f);
	glVertex3f(-1.1f, -0.6f, 0.2f);
	glVertex3f(-1.1f, -0.4f, 0.2f);
	glVertex3f(-0.9f, -0.4f, 0.2f);
	glVertex3f(-0.9f, -0.6f, 0.2f);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-0.9f, -0.4f, 0.f);
		glVertex3f(-0.9f, -0.6f, 0.f);
		glVertex3f(-0.9f, -0.6f, 0.2f);
		glVertex3f(-0.9f, -0.4f, 0.2f);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-1.1f, -0.6f, 0.f);
		glVertex3f(-1.1f, -0.4f, 0.f);
		glVertex3f(-1.1f, -0.4f, 0.2f);
		glVertex3f(-1.1f, -0.6f, 0.2f);
	glEnd();
	glLoadIdentity();

}

int main() {
	if (!glfwInit()) exit(EXIT_FAILURE);

	GLFWwindow* window = glfwCreateWindow(640, 480, "Window", NULL, NULL);
	if (!window) {
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