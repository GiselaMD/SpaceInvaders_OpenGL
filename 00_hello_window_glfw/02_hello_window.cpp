/*
 "Hello World GLFW3". Just the basics.
 Dependencies:
    GLFW3
 - add ifdef/endif APPLE
 - add opengl callback (reshape)
 - add glew for opengl
 by Luiz Gonzaga Jr @ Unisinos University
*/
#define GLEW_STATIC
#include "GL/glew.h"
#include <iostream>
#include <GLFW/glfw3.h>

static int width = 800, height = 600;

static void reshape(GLFWwindow *window, int w, int h)
{
	width = w > 1 ? w : 1;
	height = h > 1 ? h : 1;
	glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	std::cout << key << std::endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}


int main(void)
{
	GLFWwindow* window;
        std::cout << "Passou A"<<std::endl;
	if (!glfwInit())
		return -1;
#ifdef APPLE
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 2 );
	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	window = glfwCreateWindow(width, 600, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}


	glfwMakeContextCurrent(window);
        glfwSetKeyCallback(window, key_callback);
        glfwSetFramebufferSizeCallback(window, reshape);

        // Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
          {
            std::cerr << "Failed to initialize GLEW" << std::endl;
            return false;
          }

        std::cout << "OpenGL: " <<  glGetString(GL_VERSION);
        std::cout <<  "  GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;


        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	while (!glfwWindowShouldClose(window))
	{

		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
