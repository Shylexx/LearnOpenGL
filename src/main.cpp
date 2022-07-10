#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.h"
#include "stb_image/stb_image.h"

#include <iostream>
#include <string>

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
								  "layout (location = 1) in vec3 aColor;\n"
								  "out vec3 ourColor;\n"
								  "void main()\n"
								  "{\n"
								  "   gl_Position = vec4(aPos, 1.0);\n"
								  "   ourColor = aColor;\n"
								  "}\0";

const char *fragmentShader2Source = "#version 330 core\n"
									"out vec4 FragColor;\n"
									"in vec3 ourColor;\n"
									"void main()\n"
									"{\n"
									"   FragColor = vec4(ourColor, 1.0f);\n"
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

	// Const Window Title for Showing FPS in window
	const char *titleStr = "LearnOpenGL - FPS: ";
	char windowTitle[80];

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
		// positions          // colors           // texture coords
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,	  // top right
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f	  // top left
	};

	// Indices for drawing a rect from two tris
	unsigned int indices[] = {
		// note that we start from 0!
		0, 1, 3, // first Triangle
		1, 2, 3	 // second Triangle
	};

	// Declare Shader using custom shader class
	Shader myShader("E:\\dev\\LearnOpenGL\\src\\shaders\\transformshader.vs", "E:\\dev\\LearnOpenGL\\src\\shaders\\basicshader.fs");

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
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// Texture mapping
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
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

	// Flip images vertically when loaded, to keep the textures the expected orientation
	stbi_set_flip_vertically_on_load(true);

	// Create a openGl texture
	unsigned int boxTexture;
	glGenTextures(1, &boxTexture);

	// Bind boxTexture to the GL_TEXTURE_2D
	glBindTexture(GL_TEXTURE_2D, boxTexture);

	// Configure some wrapping and filtering settings
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load Texture Image
	int width, height, nrChannels;
	unsigned char *image1 = stbi_load("E:\\dev\\LearnOpenGL\\assets\\container.jpg", &width, &height, &nrChannels, 0);
	if (image1)
	{
		// Make texture using loaded image asset
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image1);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture image" << std::endl;
		std::cout << stbi_failure_reason() << std::endl;
	}
	stbi_image_free(image1);

	unsigned int faceTexture;
	glGenTextures(1, &faceTexture);

	glBindTexture(GL_TEXTURE_2D, faceTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load a second texture
	unsigned char *image2 = stbi_load("E:\\dev\\LearnOpenGL\\assets\\awesomeface.png", &width, &height, &nrChannels, 0);
	if (image2)
	{
		// Specify RGBA, as PNG has an alpha channel
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image2);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture image" << std::endl;
		std::cout << stbi_failure_reason() << std::endl;
	}
	stbi_image_free(image2);

	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "Maximum Vertex Attributes: " << GL_MAX_VERTEX_ATTRIBS << std::endl;

	// Scaling and Rotating a rendered object
	// 1) Scale the container by 0.5 on each axis
	// 2) rotate the container 90 degrees around the Z
	// 3) The axis we rotate around should be a unit vector (normalise it if you're not rotating around xyz axis)
	// 4) Because We pass the matrix to the GLM functions, it automatically multiplies the matrices together.
	// 5) glm::rotate and scale is multiplying trans by the resulting matrix from the transformation
	glm::mat4 trans = glm::mat4(1.0f);

	myShader.use();

	// Set Longhand
	glUniform1i(glGetUniformLocation(myShader.ID, "texture1"), 0);
	// Or with shader class helper function
	myShader.setInt("texture2", 1);

	// Applying matrix transform to the vert shader.
	// 1) Query the location of the uniform
	// 2) Send the matrix data to the shaders using glUniform with Matrix4fv.
	// unsigned int transformLoc = glGetUniformLocation(myShader.ID, "transform");
	// glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
	// glUniformMatrix4v params:
	// First Param: The uniform location
	// Second Param: How many matrices we are sending (1)
	// Third Param: If we want to transpose the matrix (swap columns and rows) GLM does column-major ordering by default
	// Fourth param: the matrix data, value_ptr gets an OpenGL compatible version

	float deltaTime, fps;
	float lastFrameTime = 0.0f;

	// Disables VSYNC
	// glfwSwapInterval(0);

	// Render Loop
	while (!glfwWindowShouldClose(window))
	{
		deltaTime = glfwGetTime() - lastFrameTime;
		lastFrameTime = glfwGetTime();
		fps = (1 / deltaTime);

		// Show FPS in Window Title
		memset(windowTitle, 0, sizeof windowTitle);
		strcpy(windowTitle, titleStr);
		strcat(windowTitle, std::to_string(fps).data());
		glfwSetWindowTitle(window, windowTitle);

		//  Process Key events
		processInput(window);

		// Render Commands go Here:

		// Color to clear the screen with
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		// Clear the Color buffer
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw a triangle
		// draw our first triangle

		// Change the color over time!
		// float timeValue = glfwGetTime();
		// float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		// int vertexColorLocation = glGetUniformLocation(shaderProgram2, "unifColor");
		// glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		// Apply Matrix Transform
		unsigned int transformLoc = glGetUniformLocation(myShader.ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		float xoffset = 0.5f;
		myShader.setFloat("offsetX", xoffset);

		// Bind both textures to different texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, boxTexture);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, faceTexture);

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
