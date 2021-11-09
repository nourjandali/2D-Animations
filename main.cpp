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
glm::mat4 trans(1.0f);
glm::vec4 color;
int indices = 0;
float xPos = 0.0f;
float yPos = 0.0f;
bool isSquare = false;
bool isHexagon = false;
bool isOctagon = false;
bool isRotating = false;
bool isScaling = false;
bool isTranslating = false;
bool isFull = false;

// Terminate window function
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

float randomPositions()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(-0.95, 0.95);
	return dis(gen);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_Q && action == GLFW_PRESS)
	{
		if (indices != 6)
		{
			indices = 6;
		}
		isSquare = true;
		isHexagon = false;
		isOctagon = false;
		isFull = false; // redraw object after clearing screen
		color = glm::vec4(1, 0.388, 0.278, 1.0f);
		xPos = randomPositions();
		yPos = randomPositions();
	}

	else if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		if (indices != 18)
		{
			indices = 18;
		}
		isSquare = false;
		isHexagon = true;
		isOctagon = false;
		isFull = false; // redraw object after clearing screen
		color = glm::vec4(1, 0.965, 0.561, 1.0f);
		xPos = randomPositions();
		yPos = randomPositions();
	}

	else if (key == GLFW_KEY_E && action == GLFW_PRESS)
	{
		if (indices != 24)
		{
			indices = 24;
		}
		isSquare = false;
		isHexagon = false;
		isOctagon = true;
		isFull = false; // redraw object after clearing screen
		color = glm::vec4(0.753, 0.839, 0.894, 1.0f);
		xPos = randomPositions();
		yPos = randomPositions();
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

	else if (key == GLFW_KEY_O && action == GLFW_PRESS)
	{
		isFull = true;
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

	GLuint vao1, vao2, vao3, vbo1, vbo2, vbo3, ibo1, ibo2, ibo3;

	/* ------------------------------ Square ------------------------------ */
	GLfloat squareVertices[] =
	{
		0.05f,  0.05f, 0.0f,  // top right
		0.05f, -0.05f, 0.0f,  // bottom right
		-0.05f, -0.05f, 0.0f, // bottom left
		-0.05f,  0.05f, 0.0f  // top left 
	};

	GLuint squareIndices[] =
	{
		0, 3, 1,  // first triangle
		1, 3, 2,  // second triangle
	};

	glGenVertexArrays(1, &vao1);
	glGenBuffers(1, &vbo1);
	glGenBuffers(1, &ibo1);

	// Bind VAO
	glBindVertexArray(vao1);

	// Bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, vbo1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);

	// Bind IBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(squareIndices), squareIndices, GL_STATIC_DRAW);

	// Set attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	/* ------------------------------ Hexagon ------------------------------ */
	GLfloat hexagonVertices[] =
	{
		0.0, 0.0, 0.0, // 0
		0.025, 0.05, 0.0, // 1
		0.05, 0.0, 0.0, // 2
		0.025, -0.05, 0.0, // 3
		-0.025, -0.05, 0.0, // 4
		-0.05, 0.0, 0.0, // 5
		-0.025, 0.05, 0.0, // 6
	};

	GLuint hexagonIndices[] =
	{
		0, 1, 2,
		0, 2, 3,
		0, 3, 4,
		0, 4, 5,
		0, 5, 6,
		0, 6, 1,
	};

	glGenVertexArrays(1, &vao2);
	glGenBuffers(1, &vbo2);
	glGenBuffers(1, &ibo2);

	// Bind VAO
	glBindVertexArray(vao2);

	// Bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, vbo2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(hexagonVertices), hexagonVertices, GL_STATIC_DRAW);

	// Bind IBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(hexagonIndices), hexagonIndices, GL_STATIC_DRAW);

	// Set attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	/* ------------------------------ Octagon ------------------------------ */
	GLfloat octagonVertices[] =
	{
		0.0, 0.0, 0.0, // 0
		0.025, 0.05, 0.0, // 1
		0.05, 0.025, 0.0, // 2
		0.05, -0.025, 0.0, // 3
		0.025, -0.05, 0.0, // 4
		-0.025, -0.05, 0.0, // 5
		-0.05, -0.025, 0.0, // 6
		-0.05, 0.025, 0.0, // 7
		-0.025, 0.05, 0.0, // 8
	};

	GLuint octagonIndices[] =
	{
		0, 1, 2,
		0, 2, 3,
		0, 3, 4,
		0, 4, 5,
		0, 5, 6,
		0, 6, 7,
		0, 7, 8,
		0, 8, 1,
	};
	glGenVertexArrays(1, &vao3);
	glGenBuffers(1, &vbo3);
	glGenBuffers(1, &ibo3);

	// Bind VAO
	glBindVertexArray(vao3);

	// Bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, vbo3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(octagonVertices), octagonVertices, GL_STATIC_DRAW);

	// Bind IBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo3);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(octagonIndices), octagonIndices, GL_STATIC_DRAW);

	// Set attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Set call back
	glfwSetKeyCallback(window, key_callback);

	// Local variables declaration
	float translTime = 0.0f;
	float scaleSize = 0.0f;
	float rotationAngle = 0.0f;
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

		trans = glm::translate(glm::mat4(1.0f), glm::vec3(xPos, yPos, 0.0));

		if (isRotating)
		{
			trans = glm::translate(glm::mat4(1.0f), glm::vec3(xPos, yPos, 0.0)) * glm::rotate(glm::mat4(1.0f), rotationAngle, glm::vec3(0.0, 0.0, 1.0));
			rotationAngle += 0.1f;
		}

		if (isScaling)
		{
			trans = glm::scale(glm::mat4(1.0f), glm::vec3(scaleSize, scaleSize, 0.0));
		}

		if (isTranslating)
		{
			trans = glm::translate(glm::mat4(1.0f), glm::vec3(translTime, yPos, 0.0)) * glm::rotate(glm::mat4(1.0f), rotationAngle, glm::vec3(0.0, 0.0, 1.0));
			xPos = translTime;
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

		// Clear screen
		if (isFull)
		{
			indices = 0;
		}

		// Bind VAO
		if (isSquare)
		{
			glBindVertexArray(vao1);
		}

		if (isHexagon)
		{
			glBindVertexArray(vao2);
		}

		if (isOctagon)
		{
			glBindVertexArray(vao3);
		}

		// Draw our object
		glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_INT, 0);
	}

	// Cleanup
	glDeleteBuffers(1, &vbo1);
	glDeleteBuffers(1, &ibo1);
	glDeleteVertexArrays(1, &vao1);

	glDeleteBuffers(1, &vbo3);
	glDeleteBuffers(1, &ibo3);
	glDeleteVertexArrays(1, &vao3);

	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
};