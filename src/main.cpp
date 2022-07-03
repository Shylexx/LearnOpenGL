#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <iostream>

// Resize OpenGL viewport when window size changed
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main()
{

	// Init glfw
	glfwInit();

	// Set glfw OpenGL Version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Make a native glfw window.
	GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);

	// Err and close if window init fails
	if (window == NULL)
	{
		std::cout << "Failed to init window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Make window the current opengl context
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to init GLAD" << std::endl;
		return -1;
	}

	// Set the size of the viewport
	glViewport(0, 0, 800, 600);

	// Register function for when Frame Buffer (window) changes size
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	while (!glfwWindowShouldClose(window))
	{
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
