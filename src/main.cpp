#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <iostream>

const char *vertexShaderSource = "#version 330 core\n"
								 "layout (location = 0) in vec3 aPos;\n"
								 "void main()\n"
								 "{\n"
								 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
								 "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
								   "out vec4 FragColor;\n"
								   "void main()\n"
								   "{\n"
								   "	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
								   "}\n\0";

const char *vertexShader2Source = "#version 330 core\n"
								  "layout (location = 0) in vec3 aPos;\n"
								  "out vec4 vertexColor;\n"
								  "void main()\n"
								  "{\n"
								  "   gl_Position = vec4(aPos, 1.0);\n"
								  "   vertexColor = vec4(0.5, 0.0, 0.0, 1.0);\n"
								  "}\0";

const char *fragmentShader2Source = "#version 330 core\n"
									"out vec4 FragColor;\n"
									"in vec4 vertexColor;\n"
									"void main()\n"
									"{\n"
									"	FragColor = vertexColor;\n"
									"}\n\0";

// Resize OpenGL viewport when window size changed
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
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

	// Triangle Vertex Data
	float vertices[] = {
		0.5f, 0.5f, 0.0f,	// top right
		0.5f, -0.5f, 0.0f,	// bottom right
		-0.5f, -0.5f, 0.0f, // bottom left
		-0.5f, 0.5f, 0.0f	// top left
	};

	// Indices for drawing a rect from two tris
	unsigned int indices[] = {
		// note that we start from 0!
		0, 1, 3, // first Triangle
		1, 2, 3	 // second Triangle
	};

	// Vertex Shader declared with an ID
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// Specify GLSL Source for Shader
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// Compile Vertex Shader
	glCompileShader(vertexShader);

	// Check for Shader Compile Errors
	int vertexSuccess;
	char vertexInfoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexSuccess);
	if (!vertexSuccess)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, vertexInfoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
				  << vertexInfoLog << std::endl;
	}

	// Fragment Shader
	unsigned int fragShader;
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragShader);

	int fragSuccess;
	char fragInfoLog[512];
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &fragSuccess);
	if (!fragSuccess)
	{
		glGetShaderInfoLog(fragShader, 512, NULL, fragInfoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
				  << fragInfoLog << std::endl;
	}

	// Shader Program Object
	// Final Linked version of Multiple Shaders combined
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	// Attach Shaders to Program and Link together.
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragShader);
	glLinkProgram(shaderProgram);

	int programSuccess;
	char programInfoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &programSuccess);
	if (!programSuccess)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, programInfoLog);
		std::cout << "ERROR::PROGRAM::LINK__FAILED\n"
				  << programInfoLog << std::endl;
	}

	// Delete Shaders (Once Linked, single objects no longer needed)
	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);

	// Create second shader, using shader2
	unsigned int vertexShader2;
	vertexShader2 = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader2, 1, &vertexShader2Source, NULL);
	glCompileShader(vertexShader2);

	glGetShaderiv(vertexShader2, GL_COMPILE_STATUS, &vertexSuccess);
	if (!vertexSuccess)
	{
		glGetShaderInfoLog(vertexShader2, 512, NULL, vertexInfoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
				  << vertexInfoLog << std::endl;
	}

	// Fragment Shader
	unsigned int fragShader2;
	fragShader2 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader2, 1, &fragmentShader2Source, NULL);
	glCompileShader(fragShader2);

	glGetShaderiv(fragShader2, GL_COMPILE_STATUS, &fragSuccess);
	if (!fragSuccess)
	{
		glGetShaderInfoLog(fragShader2, 512, NULL, fragInfoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
				  << fragInfoLog << std::endl;
	}

	unsigned int shaderProgram2;
	shaderProgram2 = glCreateProgram();

	// Attach Shaders to Program and Link together.
	glAttachShader(shaderProgram2, vertexShader2);
	glAttachShader(shaderProgram2, fragShader2);
	glLinkProgram(shaderProgram2);

	glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &programSuccess);
	if (!programSuccess)
	{
		glGetProgramInfoLog(shaderProgram2, 512, NULL, programInfoLog);
		std::cout << "ERROR::PROGRAM::LINK__FAILED\n"
				  << programInfoLog << std::endl;
	}

	glDeleteShader(vertexShader2);
	glDeleteShader(fragShader2);

	// Create a Vertex Buffer Object
	// Special OpenGL object to hold vertex data
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	// Vertex Array Object
	// OpenGL Object that stores:
	// Calls to glEn/DisableVertexAttribArray
	// Vertex Attribute Configs via glVertexAttribPointer
	// Vertex Buffer Objects associated with vertex attributes by calls to glVertexAttribPointer
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	// Element Buffer Object
	unsigned int EBO;
	glGenBuffers(1, &EBO);

	// Bind Vertex Array First
	glBindVertexArray(VAO);

	// Bind Vertex Buffer Object to the GL Array Buffer
	// Multiple Buffers can be bound at a time, but not of the same type.
	// Any Buffer calls on this buffer (The array buffer) will now affect this target (VBO)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Copies vertices array to the buffer's memory
	// glBufferData copies user defined data to the buffers memory
	// Params: Buffer to copy to, size of the data to be copied, the data itself, and the usage.
	// Usage Is how the GPU will manage the data
	// GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
	// GL_STATIC_DRAW: the data is set only once and used many times.
	// GL_DYNAMIC_DRAW: the data is changed a lot and used many times.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Configure EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Tell OpenGL how to interpret Vertex Data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	// glVertexAttribPointer Params:
	// Param 1: Which Vertex Attribute to configure
	// Param 2: Size of vertex attribute, vec3 so its 3.
	// Param 3: Type of data, GLSL Vec uses floats.
	// Param 4: Should data be normalised. Not relevant for floats.
	// Param 5: Stride, Space between consecutive vertex attributes
	// Param 6: Offset of where data starts in the buffer

	// Unbind Buffers so other calls wont modify the VAO or VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "Maximum Vertex Attributes: " << GL_MAX_VERTEX_ATTRIBS << std::endl;

	// Render Loop
	while (!glfwWindowShouldClose(window))
	{
		// Process Key events
		processInput(window);

		// Render Commands go Here:

		// Color to clear the screen with
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		// Clear the Color buffer
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw a triangle
		// draw our first triangle
		// Use the created shader program
		glUseProgram(shaderProgram2);
		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		// glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Swap Color Buffers
		glfwSwapBuffers(window);
		// Get Any Events during this iteration
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
