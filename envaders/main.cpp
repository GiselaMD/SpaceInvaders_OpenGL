
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <string.h>
#include <time.h>
#define GL_LOG_FILE "gl.log"
#include <iostream>
#include "gl_utils.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int g_gl_width = 640;
int g_gl_height = 480;
int vertexPosLocation;
float dx = 0, dy = 0;
GLFWwindow *g_window = NULL;

// if a key is pressed / released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	static bool gWireframe = 0;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_LEFT || key == GLFW_KEY_A && action == GLFW_PRESS) {
		if (dx > -0.9){
			printf("LEFT");
			dx -= 0.1;
		}
	}

	if (key == GLFW_KEY_RIGHT || key == GLFW_KEY_D && action == GLFW_PRESS) {
		if (dx < 0.9) {
			printf("Right");
			dx += 0.1;
		}
	}

	glUniform3f(vertexPosLocation, dx, dy, 0.0f);
}

int main() {
	restart_gl_log();
	// all the GLFW and GLEW start-up code is moved to here in gl_utils.cpp
	start_gl("Space Invaders - Gisela e Karolina");
	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	//glEnable( GL_DEPTH_TEST ); // enable depth-testing
	//glDepthFunc( GL_LESS );		 // depth-testing interprets a smaller value as "closer"


	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float spaceshuttleVertices[] = {
		// positions          // colors           // texture coords
		 0.1f, -0.75f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 0.1f, -0.9f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.1f, -0.9f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.1f, -0.75f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
	};
	unsigned int spacehuttleIndices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	float enemyVertices[] = {
		// positions          // colors           // texture coords
		-0.8f,  0.9f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		-0.8f,  0.8f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.9f,  0.8f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.9f,  0.9f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
	};
	unsigned int enemyIndices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	unsigned int VBOs[2], VAOs[2], EBOs[2];
	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);
	glGenBuffers(2, EBOs);

	//Spaceshuttle
	// ---------
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(spaceshuttleVertices), spaceshuttleVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(spacehuttleIndices), spacehuttleIndices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//Enemy
	// ---------
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(enemyVertices), enemyVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(enemyIndices), enemyIndices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	char vertex_shader[1024 * 256];
	char vertex_shader_enemy[1024 * 256];
	char fragment_shader[1024 * 256];
	parse_file_into_str("test_vs.glsl", vertex_shader, 1024 * 256);
	parse_file_into_str("vs_enemy.glsl", vertex_shader_enemy, 1024 * 256);
	parse_file_into_str("test_fs.glsl", fragment_shader, 1024 * 256);

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	const GLchar *p = (const GLchar *)vertex_shader;
	glShaderSource(vs, 1, &p, NULL);
	glCompileShader(vs);

	// check for compile errors
	int params = -1;
	glGetShaderiv(vs, GL_COMPILE_STATUS, &params);
	if (GL_TRUE != params) {
		fprintf(stderr, "ERROR: GL shader index %i did not compile\n", vs);
		print_shader_info_log(vs);
		return 1; // or exit or something
	}

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	p = (const GLchar *)fragment_shader;
	glShaderSource(fs, 1, &p, NULL);
	glCompileShader(fs);

	// check for compile errors
	glGetShaderiv(fs, GL_COMPILE_STATUS, &params);
	if (GL_TRUE != params) {
		fprintf(stderr, "ERROR: GL shader index %i did not compile\n", fs);
		print_shader_info_log(fs);
		return 1; // or exit or something
	}

	//programme with shaders for Spaceshuttle
	GLuint shader_programme = glCreateProgram();
	glAttachShader(shader_programme, fs);
	glAttachShader(shader_programme, vs);
	glLinkProgram(shader_programme);

	glGetProgramiv(shader_programme, GL_LINK_STATUS, &params);
	if (GL_TRUE != params) {
		fprintf(stderr, "ERROR: could not link shader programme GL index %i\n",
			shader_programme);
		print_programme_info_log(shader_programme);
		return false;
	}

	//create enemy vertex shader
	GLuint vs2 = glCreateShader(GL_VERTEX_SHADER);
	p = (const GLchar *)vertex_shader_enemy;
	glShaderSource(vs2, 1, &p, NULL);
	glCompileShader(vs2);

	// check for compile errors
	glGetShaderiv(vs2, GL_COMPILE_STATUS, &params);
	if (GL_TRUE != params) {
		fprintf(stderr, "ERROR: GL shader index %i did not compile\n", vs2);
		print_shader_info_log(vs2);
		return 1; // or exit or something
	}

	//programme with shaders for enemy
	GLuint shader_programme_enemy = glCreateProgram();
	glAttachShader(shader_programme_enemy, fs);
	glAttachShader(shader_programme_enemy, vs2);
	glLinkProgram(shader_programme_enemy);

	glGetProgramiv(shader_programme_enemy, GL_LINK_STATUS, &params);
	if (GL_TRUE != params) {
		fprintf(stderr, "ERROR: could not link shader programme GL index %i\n",
			shader_programme_enemy);
		return 0;
	}

	// load and create a texture SHUTTERSPACE
	// -------------------------
	unsigned int texture_spaceshuttle;
	glGenTextures(1, &texture_spaceshuttle);
	glBindTexture(GL_TEXTURE_2D, texture_spaceshuttle);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;

	stbi_set_flip_vertically_on_load(true);
	// load image, create texture and generate mipmaps
	unsigned char *data = stbi_load("spaceshuttle.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// load and create a texture ENEMY
	// -------------------------
	unsigned int texture_enemy;
	glGenTextures(1, &texture_enemy);
	glBindTexture(GL_TEXTURE_2D, texture_enemy);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//stbi_set_flip_vertically_on_load(true);
	// load image, create texture and generate mipmaps
	data = stbi_load("enemy.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glEnable(GL_CULL_FACE); // cull face
	glCullFace(GL_BACK);		// cull back face
	glFrontFace(GL_CW);			// GL_CCW for counter clock-wise


								// Set the required callback functions
	glfwSetKeyCallback(g_window, key_callback);


	while (!glfwWindowShouldClose(g_window)) {

		// update other events like input handling
		glfwPollEvents();
		if (GLFW_PRESS == glfwGetKey(g_window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(g_window, 1);
		}
		// wipe the drawing surface clear
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glViewport(0, 0, g_gl_width, g_gl_height);

		//Enemy
		glActiveTexture(texture_enemy);
		glBindTexture(GL_TEXTURE_2D, texture_enemy);
		glUseProgram(shader_programme_enemy);
		glBindVertexArray(VAOs[1]);
		//initializing arrays of positions
		float x_min[16] = {};
		float x_max[16] = {};
		float y_min[16] = {};
		float y_max[16] = {};
		//clone enemies
		
		for (int i = 0; i <= 16; i++)
		{
			glm::mat4 trans;
			if (i <= 7) {//PRIMEIRA LINHA
				trans = glm::translate(trans, glm::vec3((0.1f + ((float)i) / 5), 0.0f, 0.0f));
				x_min[i] = (-0.9f + (0.1f + ((float)i) / 5));
				x_max[i] = (-0.8f + (0.1f + ((float)i) / 5));
				y_min[i] = 0.8f;
				y_max[i] = 0.9f;
			}
			if (i > 7) {//SEGUNDA LINHA
				trans = glm::translate(trans, glm::vec3((-1.6f + ((float)i) / 5), -0.2f, 0.0f));
				x_min[i] = (-0.9f + (-1.5f + ((float)i) / 5));
				x_max[i] = (-0.8f + (-1.5f + ((float)i) / 5));
				y_min[i] = 0.6f;
				y_max[i] = 0.7f;
			}
			unsigned int transformLoc = glGetUniformLocation(shader_programme_enemy, "transform");
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		//SpaceShuttle
		glActiveTexture(texture_spaceshuttle);
		glBindTexture(GL_TEXTURE_2D, texture_spaceshuttle);
		glUseProgram(shader_programme);
		//get sumPos position
		vertexPosLocation = glGetUniformLocation(shader_programme, "sumPos");
		glBindVertexArray(VAOs[0]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		
		// put the stuff we've been drawing onto the display
		glfwSwapBuffers(g_window);
	}

	// close GL context and any other GLFW resources
	glfwTerminate();
	return 0;
}