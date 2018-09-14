#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

const GLint WIDTH = 800;
const GLint HEIGHT = 600;

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

	while (!glfwWindowShouldClose(mainWindow))
	{
		// get and handler user input event
		glfwPollEvents();

		// clear window
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(mainWindow);
	}

	return 0;
}