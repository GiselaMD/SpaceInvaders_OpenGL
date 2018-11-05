/*
 "Hello World GLFW3". Just the basics.
 Dependencies:
    GLFW3
 - add callback
 by Luiz Gonzaga Jr @ Unisinos University
*/

#include <GLFW/glfw3.h>
#include <iostream>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	std::cout << key << std::endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}


int main(void)
{
	GLFWwindow* window;

	if (!glfwInit())
		return -1;
	window = glfwCreateWindow(1024, 768, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        glfwSetKeyCallback(window, key_callback);

	while (!glfwWindowShouldClose(window))
	{

		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
