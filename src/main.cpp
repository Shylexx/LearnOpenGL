#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.h"
#include "stb_image/stb_image.h"

#include <iostream>
#include <string>

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

	// Initialise GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to init GLAD" << std::endl;
		return -1;
	}

	// Set the size of the viewport
	glViewport(0, 0, 800, 600);

	// Register function for when Frame Buffer (window) changes size
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glEnable(GL_DEPTH_TEST);

	// Triangle Vertex Data

	float vertices[] = {
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f};

	// Indices for drawing a rect from two tris
	unsigned int indices[] = {
		// note that we start from 0!
		0, 1, 3, // first Triangle
		1, 2, 3	 // second Triangle
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f)};

	// Declare Shader using custom shader class
	Shader myShader("E:\\dev\\LearnOpenGL\\src\\shaders\\coordinateshader.vs", "E:\\dev\\LearnOpenGL\\src\\shaders\\coordinateshader.fs");

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	// Texture mapping
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
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

	myShader.use();

	myShader.setInt("texture1", 0);
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
		// Clear the Color buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Bind both textures to different texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, boxTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, faceTexture);

		myShader.use();

		// Create transformations
		// Model Matrix
		glm::mat4 model = glm::mat4(1.0f);
		// Stationary Model
		// model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		// Rotating over time
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
		// View Matrix
		glm::mat4 view = glm::mat4(1.0f);
		// note that we're translating the scene in the reverse direction of where we want to move
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		// Perspective Projection Matrix
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

		// Send Transform Matrices to shader
		myShader.setMat4("model", model);
		myShader.setMat4("view", view);
		myShader.setMat4("projection", projection);

		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			myShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

				// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Swap Color Buffers
		glfwSwapBuffers(window);
		// Get Any Events during this iteration
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
