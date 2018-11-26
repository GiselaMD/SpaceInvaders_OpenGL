
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>
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
#include <ft2build.h>
#include FT_FREETYPE_H  
#include "Shader.h"

int g_gl_width = 640;
int g_gl_height = 480;
int vertexPosLocation;
int vertexPosLocation2;
float dx_bullet = 0, dy_bullet = 0;
float dx_nave = 0, dy_nave = 0;
bool enemy_exist[24];
bool toThrowBullet = false;
GLuint VBOs[4], VAOs[4], EBOs[3];
GLuint countTiro;
void loadEnemies(void);
//
///// Holds all state information relevant to a character as loaded using FreeType
//struct Character {
//	GLuint TextureID;   // ID handle of the glyph texture
//	glm::ivec2 Size;    // Size of glyph
//	glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
//	GLuint Advance;    // Horizontal offset to advance to next glyph
//};
//
//std::map<GLchar, Character> Characters;
//
//void RenderText(Shader &shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

//initializing arrays of positions
float x_min[24] = {};
float x_max[24] = {};
float y_min[24] = {};
float y_max[24] = {};

GLFWwindow *g_window = NULL;

// if a key is pressed / released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	static bool gWireframe = 0;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if ((key == GLFW_KEY_LEFT || key == GLFW_KEY_A) && action == GLFW_PRESS) {
		if (dx_nave > -0.9) {
			printf("LEFT");
			dx_nave -= 0.1;
			if (!toThrowBullet) {
				dx_bullet -= 0.1;
			}
		}
	}

	if ((key == GLFW_KEY_RIGHT || key == GLFW_KEY_D) && action == GLFW_PRESS) {
		if (dx_nave < 0.9) {
			printf("Right");
			dx_nave += 0.1;
			if (!toThrowBullet) {
				dx_bullet += 0.1;
			}
		}
	}

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		printf("Space");
		toThrowBullet = true;
	}

	if (key == GLFW_KEY_R && action == GLFW_PRESS) {
		loadEnemies();
	}
}
void loadEnemies() {
	for (int i = 0; i < 24; i++) {
		enemy_exist[i] = true;
	}
}
void verificaTiro(int countTiro) {
	countTiro = 1;
	//Tentando verificar se bullet acertou enemy
	for (int i = 0; i < 24; i++) {
		printf("%d", countTiro);
		//Verifica se está dentro do intervalo de inimigos
		if ((dx_bullet >= x_min[i] && dx_bullet <= x_max[i]) && (dy_bullet >= y_min[i] && dy_bullet <= y_max[i])) {
			if ((y_min[i + 12] > y_min[i])) {
				printf("está em cima!");
				enemy_exist[i] = false;
				countTiro = 0;
			}
			if ((y_min[i + 12] < y_min[i])) {
				printf("está embaixo!");
				enemy_exist[i+12] = false;
				y_min[i + 12] = 20; //mudo a altura pra depois conseguir apagar o enemy de cima
				countTiro = 0;		//OBS: passar o enemy de baixo pra cima, para então conseguir pegar o mais embaixo
			}
		}
	}
}

int main() {
	restart_gl_log();
	// all the GLFW and GLEW start-up code is moved to here in gl_utils.cpp
	start_gl("Space Invaders - Gisela e Karolina");
	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	//glEnable( GL_DEPTH_TEST ); // enable depth-testing
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	////glDepthFunc( GL_LESS );		 // depth-testing interprets a smaller value as "closer"
	//// Compile and setup the shader
	//Shader shader("vs_freetype", "fs_freetype");
	//glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(g_gl_width), 0.0f, static_cast<GLfloat>(g_gl_height));
	//shader.use();
	//glUniformMatrix4fv(glGetUniformLocation(glCreateProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	//// FreeType
	//FT_Library ft;
	//// All functions return a value different than 0 whenever an error occurred
	//if (FT_Init_FreeType(&ft))
	//	std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	//// Load font as face
	//FT_Face face;
	//if (FT_New_Face(ft, "fonts/arial.ttf", 0, &face))
	//	std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	//// Set size to load glyphs as
	//FT_Set_Pixel_Sizes(face, 0, 48);

	//// Disable byte-alignment restriction
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//// Load first 128 characters of ASCII set
	//for (GLubyte c = 0; c < 128; c++)
	//{
	//	// Load character glyph 
	//	if (FT_Load_Char(face, c, FT_LOAD_RENDER))
	//	{
	//		std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
	//		continue;
	//	}
	//	// Generate texture
	//	GLuint texture;
	//	glGenTextures(1, &texture);
	//	glBindTexture(GL_TEXTURE_2D, texture);
	//	glTexImage2D(
	//		GL_TEXTURE_2D,
	//		0,
	//		GL_RED,
	//		face->glyph->bitmap.width,
	//		face->glyph->bitmap.rows,
	//		0,
	//		GL_RED,
	//		GL_UNSIGNED_BYTE,
	//		face->glyph->bitmap.buffer
	//	);
	//	// Set texture options
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//	// Now store character for later use
	//	Character character = {
	//		texture,
	//		glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
	//		glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
	//		face->glyph->advance.x
	//	};
	//	Characters.insert(std::pair<GLchar, Character>(c, character));
	//}
	//glBindTexture(GL_TEXTURE_2D, 0);
	//// Destroy FreeType once we're finished
	//FT_Done_Face(face);
	//FT_Done_FreeType(ft);

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
		-0.78f,  0.9f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		-0.78f,  0.8f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.88f,  0.8f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.88f,  0.9f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
	};

	unsigned int enemyIndices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	glm::vec3 enemyPositions[] = {
		glm::vec3(0.0f,  0.0f, 0.0f),
		glm::vec3(0.15f, 0.0f, 0.0f),
		glm::vec3(0.3f,  0.0f, 0.0f),
		glm::vec3(0.45f, 0.0f, 0.0f),
		glm::vec3(0.6f,  0.0f, 0.0f),
		glm::vec3(0.75f, 0.0f, 0.0f),
		glm::vec3(0.9f,  0.0f, 0.0f),
		glm::vec3(1.05f, 0.0f, 0.0f),
		glm::vec3(1.2f,  0.0f, 0.0f),
		glm::vec3(1.35f, 0.0f, 0.0f),
		glm::vec3(1.5f,  0.0f, 0.0f),
		glm::vec3(1.65f, 0.0f, 0.0f),

		glm::vec3(0.0f,  -0.2f, 0.0f),
		glm::vec3(0.15f, -0.2f, 0.0f),
		glm::vec3(0.3f,  -0.2f, 0.0f),
		glm::vec3(0.45f, -0.2f, 0.0f),
		glm::vec3(0.6f,  -0.2f, 0.0f),
		glm::vec3(0.75f, -0.2f, 0.0f),
		glm::vec3(0.9f,  -0.2f, 0.0f),
		glm::vec3(1.05f, -0.2f, 0.0f),
		glm::vec3(1.2f,  -0.2f, 0.0f),
		glm::vec3(1.35f, -0.2f, 0.0f),
		glm::vec3(1.5f,  -0.2f, 0.0f),
		glm::vec3(1.65f, -0.2f, 0.0f),
	};

	float bulletVertices[] = {
		0.0, -0.7f, 0.0f
	};

	unsigned int bulletIndices[] = {
		0
	};

	//unsigned int VBOs[4], VAOs[4], EBOs[3];
	glGenVertexArrays(4, VAOs);
	glGenBuffers(4, VBOs);
	glGenBuffers(3, EBOs);

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

	//---------Bullet test

	//Bullet
	// ---------
	glBindVertexArray(VAOs[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bulletVertices), bulletVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(bulletIndices), bulletIndices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	////Free Type
	//glBindVertexArray(VAOs[3]);
	//glBindBuffer(GL_ARRAY_BUFFER, VBOs[3]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);

	char vertexShader[1024];
	char fragmentShader[1024];
	parse_file_into_str("vs.glsl", vertexShader, 1024);
	parse_file_into_str("fs.glsl", fragmentShader, 1024);

	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar *pe = (const GLchar *)vertexShader;
	glShaderSource(vShader, 1, &pe, NULL);
	glCompileShader(vShader);


	GLint result;
	GLchar infoLog[512];
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vShader, sizeof(infoLog), NULL, infoLog);
		std::cout << "Error! Vertex shader failed to compile. " << infoLog << std::endl;
	}


	GLint fShader = glCreateShader(GL_FRAGMENT_SHADER);
	pe = (const GLchar *)fragmentShader;
	glShaderSource(fShader, 1, &pe, NULL);
	glCompileShader(fShader);


	glGetShaderiv(fShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fShader, sizeof(infoLog), NULL, infoLog);
		std::cout << "Error! Fragment shader failed to compile. " << infoLog << std::endl;
	}


	GLint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vShader);
	glAttachShader(shaderProgram, fShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
		std::cout << "Error! Shader program linker failure. " << infoLog << std::endl;
	}

	glDeleteShader(vShader);
	glDeleteShader(fShader);

	glPointSize(10.0);


	//-------end Bullet test


	char vertex_shader[1024];
	char vertex_shader_enemy[1024];
	char fragment_shader[1024];
	parse_file_into_str("test_vs.glsl", vertex_shader, 1024);
	parse_file_into_str("vs_enemy.glsl", vertex_shader_enemy, 1024);
	parse_file_into_str("test_fs.glsl", fragment_shader, 1024);

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
	loadEnemies();

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


		//RenderText(shader, "This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
		//RenderText(shader, "(C) LearnOpenGL.com", 540.0f, 570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));


		//Enemy
		glActiveTexture(texture_enemy);
		glBindTexture(GL_TEXTURE_2D, texture_enemy);
		glUseProgram(shader_programme_enemy);
		glBindVertexArray(VAOs[1]);
		
		
		//clone enemies
		for (int i = 0; i < 24; i++){
			if (enemy_exist[i]) {
				glm::mat4 trans;
				trans = glm::translate(trans, enemyPositions[i]);
				x_min[i] = (-0.88f) + (0.15f * i); //left position + deslocamento
				x_max[i] = (-0.78f) + (0.15f * i); //right position + deslocamento
				if (i <= 11) {
					y_min[i] = 0.8f; //down position 
					y_max[i] = 0.9f; //up position 
				}
				if (i > 12) {
					y_min[i] = 0.6f; //down position 
					y_max[i] = 0.7f; //up position 
				}
				unsigned int transformLoc = glGetUniformLocation(shader_programme_enemy, "transform");
				glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}
		}

		//SpaceShuttle
  		glActiveTexture(texture_spaceshuttle);
		glBindTexture(GL_TEXTURE_2D, texture_spaceshuttle);
		glUseProgram(shader_programme);
		//get sumPos position
		vertexPosLocation = glGetUniformLocation(shader_programme, "sumPos");

		glUniform3f(vertexPosLocation, dx_nave, dy_nave, 0.0f);
		glBindVertexArray(VAOs[0]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		//Bullet test 
		glUseProgram(shaderProgram);
		vertexPosLocation2 = glGetUniformLocation(shaderProgram, "sumPos2");
		glUniform3f(vertexPosLocation2, dx_bullet, dy_bullet, 0.0f);
		glBindVertexArray(VAOs[2]);
		glDrawElements(GL_POINTS, 1, GL_UNSIGNED_INT, 0);

		if (toThrowBullet) {
			
			float timeValue = glfwGetTime();
			if (dy_bullet > 2.0f) {
				//printf("dy_bullet + %f", dy_bullet);
				dy_bullet = 0.0f;
				dx_bullet = dx_nave;
				glfwSetTime(0.0);
				toThrowBullet = false;
			}
			else {
				dy_bullet = (timeValue);
				verificaTiro(countTiro);
			}
		}

		
		//glBindVertexArray(0);
		//--- end Bullet test


		// put the stuff we've been drawing onto the display
		glfwSwapBuffers(g_window);
	}
	//glfwSetWindowShouldClose(g_window, GL_TRUE);
	// close GL context and any other GLFW resources
	glfwTerminate();
	return 0;
}
//
//void RenderText(Shader &s, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
//{
//	// Activate corresponding render state	
//	s.use();
//	glUniform3f(glGetUniformLocation(glCreateProgram(), "textColor"), color.x, color.y, color.z);
//	glActiveTexture(GL_TEXTURE0);
//	glBindVertexArray(VAOs[3]);
//
//	// Iterate through all characters
//	std::string::const_iterator c;
//	for (c = text.begin(); c != text.end(); c++)
//	{
//		Character ch = Characters[*c];
//
//		GLfloat xpos = x + ch.Bearing.x * scale;
//		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
//
//		GLfloat w = ch.Size.x * scale;
//		GLfloat h = ch.Size.y * scale;
//		// Update VBO for each character
//		GLfloat vertices[6][4] = {
//			{ xpos,     ypos + h,   0.0, 0.0 },
//			{ xpos,     ypos,       0.0, 1.0 },
//			{ xpos + w, ypos,       1.0, 1.0 },
//
//			{ xpos,     ypos + h,   0.0, 0.0 },
//			{ xpos + w, ypos,       1.0, 1.0 },
//			{ xpos + w, ypos + h,   1.0, 0.0 }
//		};
//		// Render glyph texture over quad
//		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
//		// Update content of VBO memory
//		glBindBuffer(GL_ARRAY_BUFFER, VBOs[3]);
//		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
//		glBindBuffer(GL_ARRAY_BUFFER, 0);
//		// Render quad
//		glDrawArrays(GL_TRIANGLES, 0, 6);
//		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
//		x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
//	}
//	glBindVertexArray(0);
//	glBindTexture(GL_TEXTURE_2D, 0);
//}