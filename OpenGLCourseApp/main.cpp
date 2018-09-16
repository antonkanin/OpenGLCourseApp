#include <iostream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x2.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const GLint WIDTH = 800;
const GLint HEIGHT = 600;

GLuint VAO, VBO, shader, uniformModel;

bool direction = true;
float triOffset = 0.0f;
float triMaxoffset = 0.7f;
float triIncrement = 0.005f;

// Vertex Shader
static const char* vShader = "										\n\
#version 330														\n\
																	\n\
layout (location = 0) in vec3 pos;									\n\
																	\n\
uniform float xMove;												\n\
																	\n\
void main()															\n\
{																	\n\
	gl_Position = vec4(0.4 * pos.x + xMove, 0.4 * pos.y, pos.z, 1.0);		\n\
}";

// Fragment Shader
static const char* fShader = "										\n\
#version 330														\n\
																	\n\
out vec4 color;														\n\
																	\n\
void main()															\n\
{																	\n\
	color = vec4(1.0, 0.0, 0.0, 1.0);							\n\
}";

void CreateTriangle()
{
	GLfloat vertices[] = 
	{
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};
	
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO); // binding

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
			glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0); // unbinding
}

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, sizeof(eLog), nullptr, eLog);
		std::cout << "Error compiling the " + std::to_string(shaderType) + " shader: " + std::string(eLog) << std::endl;
		return;
	}

	glAttachShader(theProgram, theShader);
}

void CompileShaders()
{
	shader = glCreateProgram();

	if (!shader)
	{
		throw std::runtime_error("Error creating shader program");
	}

	AddShader(shader, vShader, GL_VERTEX_SHADER);
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), nullptr, eLog);
		std::cout << "Error linking the program: " + std::string(eLog) << std::endl;
		return;
	}

	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), nullptr, eLog);
		std::cout << "Error validating the program: " + std::string(eLog) << std::endl;
		return;
	}

	uniformModel = glGetUniformLocation(shader, "xMove");
}

int main()
{
	if (!glfwInit())
	{
		std::cout << "GLFW initialization failed" << std::endl;
		glfwTerminate();
		return 1;
	}

	// setup glfw window properties
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Core profile = No Backwards Compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Allow forward compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", nullptr, nullptr);
	if (!mainWindow)
	{
		std::cout << "GLFW window creation failed" << std::endl;
		glfwTerminate();
		return 1;
	}

	// Get bugger size information
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	// Allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Glew initialization failed" << std::endl;
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	};

	// setup viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	CreateTriangle();
	CompileShaders();

	while (!glfwWindowShouldClose(mainWindow))
	{
		// get and handler user input event
		glfwPollEvents();

		if (direction)
		{
			triOffset += triIncrement;
		}
		else
		{
			triOffset -= triIncrement;
		}

		if (abs(triOffset) >= triMaxoffset)
		{
			direction = !direction;
		}

		// clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);

		glUniform1f(uniformModel, triOffset);

		glBindVertexArray(VAO);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(0);

		glUseProgram(0);

		glfwSwapBuffers(mainWindow);
	}

	return 0;
}