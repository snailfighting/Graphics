#include <GLFW/glfw3.h> 
#include <stdlib.h> 
#include <stdio.h> 

int alpha, beta;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == 'A')
		alpha -= 5;
	if (key == 'D')
		alpha += 5;
	if (key == 'W')
		beta += 5;
	if (key == 'S')
		beta -= 5;
}


void display(GLFWwindow* window) {
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	
	glMatrixMode(GL_PROJECTION);//каким образом 3-х мерный мир проeцируется на двухмерный экран 
	glLoadIdentity();//загрузить единичную матрицу 
	glOrtho(-5, 5, -5.0 * height / width, 5.0 * height / width, -5, 5);//задание по xyz придельные координаты -10,10,-10,10,-1,1 соответственно матрица не реалистичная 
	//gluPerspective(120, float(width) / height, 0.001, 2500); 
	glMatrixMode(GL_MODELVIEW);//с какой точки мы наблюдаем + низ 
	glLoadIdentity();
	glPushMatrix();
	glTranslatef(0, 0, -3);
	glScalef(0.5, 0.5, 0.5);
	glRotatef(alpha, 0, 1, 0);
	glRotatef(beta, 0, 0, 1);
	//alpha += 0.1; 

	glBegin(GL_POLYGON);//front
	glColor3f(1.0f, 0.f, 0.f);
		glVertex3f(0.2f, 0.2f, 0.f);
		glVertex3f(0.5f, 0.2f, 0.f);
		glVertex3f(0.5f, 0.5f, 0.f);
		glVertex3f(0.2f, 0.5f, 0.f);
	glEnd();

	glBegin(GL_POLYGON);//back
	glColor3f(0.f, 0.f, 1.0f);
		glVertex3f(0.2f, 0.2f, 0.3f);
		glVertex3f(0.5f, 0.2f, 0.3f);
		glVertex3f(0.5f, 0.5f, 0.3f);
		glVertex3f(0.2f, 0.5f, 0.3f);
	glEnd();

	glBegin(GL_POLYGON);//right
	glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(0.5f, 0.2f, 0.f);
		glVertex3f(0.5f, 0.2f, 0.3f);
		glVertex3f(0.5f, 0.5f, 0.3f);
		glVertex3f(0.5f, 0.5f, 0.f);
	glEnd();

	glBegin(GL_POLYGON);//left
	glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(0.2f, 0.2f, 0.f);
		glVertex3f(0.2f, 0.2f, 0.3f);
		glVertex3f(0.2f, 0.5f, 0.3f);
		glVertex3f(0.2f, 0.5f, 0.f);
	glEnd();

	glBegin(GL_POLYGON);//botton
	glColor3f(1.0f, 0.f, 0.f);
		glVertex3f(0.2f, 0.2f, 0.f);
		glVertex3f(0.5f, 0.2f, 0.f);
		glVertex3f(0.5f, 0.2f, 0.3f);
		glVertex3f(0.2f, 0.2f, 0.3f);
	glEnd();

	glBegin(GL_POLYGON);//top
	glColor3f(1.0f, 0.f, 0.f);
		glVertex3f(0.2f, 0.5f, 0.f);
		glVertex3f(0.5f, 0.5f, 0.f);
		glVertex3f(0.5f, 0.5f, 0.3f);
		glVertex3f(0.2f, 0.5f, 0.3f);
	glEnd();



	glBegin(GL_POINTS);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.f, 0.f, 0.f);
	glEnd();
	glPopMatrix();

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