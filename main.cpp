// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <random>
#include <iostream>

// Include GLEW
#include "Dependencies\glew\glew.h"

// Include GLFW
#include "Dependencies\glfw\glfw3.h"

// Include GLM
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include "Dependencies\glm\gtc\type_ptr.hpp"

#include "shader.hpp"

// Global variables
GLFWwindow* window;
const int width = 1024, height = 1024;
int verticesCount = 0;
glm::mat4 trans(1.0f);
glm::vec4 color;
bool isRotating = false;
bool isScaling = false;
bool isTranslating = false;

// Terminate window function
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_Q && action == GLFW_PRESS)
	{
		if (verticesCount != 6)
			verticesCount = 6;
		color = glm::vec4(1, 0.388, 0.278, 1.0f);
	}

	else if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		isRotating = true;
		isScaling = false;
		isTranslating = false;
	}

	else if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		isRotating = false;
		isScaling = true;
		isTranslating = false;
	}

	else if (key == GLFW_KEY_T && action == GLFW_PRESS)
	{
		isRotating = false;
		isScaling = false;
		isTranslating = true;
	}
}

int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(width, height, "2D Animations", NULL, NULL);
	if (window == NULL)
	{
		fprintf(stderr, "Failed to open GLFW window.");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		glfwTerminate();
		return -1;
	}

	// Specify the size of the rendering window
	glViewport(0, 0, width, height);

	// Background color
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

	GLfloat vertices[] =
	{
		0.05f,  0.05f, 0.0f,  // top right
		0.05f, -0.05f, 0.0f,  // bottom right
		-0.05f, -0.05f, 0.0f, // bottom left
		-0.05f,  0.05f, 0.0f  // top left 
	};

	GLuint indices[] =
	{
		0, 3, 1,  // first triangle
		1, 3, 2,  // second triangle
	};


	GLuint vao, vbo, ibo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	// Bind VAO
	glBindVertexArray(vao);

	// Bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Bind IBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Wireframe mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Set attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Set call back
	glfwSetKeyCallback(window, key_callback);

	float translTime = 0.0f;
	float scaleSize = 0.0f;
	bool translateDirection = false; // right -> true, left -> false
	bool scaleDirection = false; // bigger -> true, smaller -> false


	// Check if the window was closed
	while (!glfwWindowShouldClose(window))
	{
		// Terminate window
		processInput(window);

		// Swap buffers
		glfwSwapBuffers(window);

		// Check for events
		glfwPollEvents();

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		// Bind VAO
		glBindVertexArray(vao);

		if (isRotating)
		{
			trans = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime() * 100, glm::vec3(0.0, 0.0, 1.0));
		}

		if (isScaling)
		{
			trans = glm::scale(glm::mat4(1.0f), glm::vec3(scaleSize, scaleSize, 0.0));
		}

		if (isTranslating)
		{
			trans = glm::translate(glm::mat4(1.0f), glm::vec3(translTime, 0.0, 0.0));
			//std::cout << translTime << "\n";
		}

		// Translating over time
		if (translTime > 0.95f)
		{
			translTime = 0.95f;
			translateDirection = true;

		}
		else if (translTime < -0.95f)
		{
			translTime = -0.95f;
			translateDirection = false;
		}

		if (translateDirection)
		{
			translTime -= 0.001f;
		}
		else {
			translTime += 0.001f;
		}

		// Scaling over time
		if (scaleSize > 20.0f)
		{
			scaleSize = 20.0f;
			scaleDirection = true;
		}
		else if (scaleSize < 0.1f)
		{
			scaleSize = 0.1f;
			scaleDirection = false;
		}

		if (scaleDirection)
		{
			scaleSize -= 0.01f;
		}
		else {
			scaleSize += 0.01f;
		}

		// Send transform variable to vertex shader
		unsigned int transformLoc = glGetUniformLocation(programID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		// Send color variable to fragment shader
		unsigned int colorLoc = glGetUniformLocation(programID, "color");
		glUniform4fv(colorLoc, 1, glm::value_ptr(color));

		// Draw our object
		glDrawElements(GL_TRIANGLES, verticesCount, GL_UNSIGNED_INT, 0);
	}

	// Cleanup
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}