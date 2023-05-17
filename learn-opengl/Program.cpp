#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shaders.h"
#include "stb_image.h"
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void KeyCallbacks(GLFWwindow* window, int key, int scancode, int action, int mods);

static bool drawLines = false;
static float xOffset = 0.0f;
static float yOffset = 0.0f;
static float blendScale = 0.2f;

int main() {

	// Initializes GLFW
	glfwInit();


	// Configures options. check GLFW window handling documentation here
	//https://www.glfw.org/docs/latest/window.html#window_hints

	// Sets major and minor version of OpenGL we want to use. 4.6 in this case
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);


	// Enables the core-profile which gives us access to smaler subset
	// subset of OpenGL features without backwards-compatable features
	// we no longer need
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// Enable this on MAC OS
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


	// Creating the window object
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// Tells GLFW to call out callback function every time window size changes
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	// Initializing GLAD to manage OpenGL functions
	// glfwGetProcAddress gives us the correct functions based on OS
	// !!!ADD glad.c TO PROJECT FILE!!!!
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	// Reads text from files and compiles the shader program from that
	Shader ourShader("VertexShader.txt", "FragmentShader.txt");


	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	
	// (VBO)Vertex buffer object. used to store large amount of vertices to send at the same time to the CPU.
	// Since it is a slow process to send to the CPU we want to send as much data at the same time
	// (VAO)Vertex array object is used to store data from the VBO subsequentaly. OpenGL will no draw anything without this
	// The VAO need to be binded before anything else
	// These object is where shit happens but i am not realy sure what is going on behind the sceens
	// (EBO)Element buffer object is just to store vertices for more than one triangle so we can draw an Element with reduced overlap
	// Multiples of these are in an array and not seperate
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	unsigned int VBO, EBO;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);


	// Initialization code (done once(unless the object frequently changes))

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// glVertexAttribPointer arguments
	// Location = 0 so we pass in 0
	// We use vec3 so we pass in 3 values
	// We specifies the data type as float
	// If we want nomalaized data to be used we use GL_TRUE
	// Fifth argument is the "stride" it's the distance from one point to another in bytes
	// The last parameter is the offset off where another stride (for color for example) begins
	
	// Position attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Color attributes
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(1);
	// Texture attributes
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Press 'L' to change from Line or Fill triangles
	// Sets the keycallback we created to a specific window
	// This is used if we a key press only do someting once per click
	glfwSetKeyCallback(window, KeyCallbacks);


	// Texture 1
	// Generates texture and stores it in a unasinged int 
	unsigned int texture1, texture2;
	glGenTextures(1, &texture1);

	// Binds the texture to the active texture unit
	glBindTexture(GL_TEXTURE_2D, texture1);


	// Set the texture wrapping/filtering options (on the currently bound texture object)
	// 
	// Wrapping options:
	// GL_REPEAT repeats image
	// GL_MIRRORED_REPEAT self explainatory
	// GL_CLAMP_TO_EDGE drags the edge out
	// GL_CMALP_TO_BORDER gives the image a border (needs an exta parameter input for border color)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Filtering options:
	// GL_LINEAR smooth 
	// GL_NEARES pixelated
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Loads the image from a path and stores it as a char
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("Textures/WoodenContainer.jpg", &width, &height, &nrChannels, 0);
	

	if (data)
	{
		// Arguments for glTexImage2D
		// Specify texture target
		// Specify the Mipmap level
		// The format we want to the texture as
		// Sets the width
		// Sets the height
		// Always 0 (legacy stuff)
		// Specify the format of the source img
		// Specify the datatype of the source img
		// The actual image data
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	// Frees the imagedata from memory
	// Use after done loading the image
	stbi_image_free(data);

	// Texture 2
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load("Textures/awesomeface.png", &width, &height, &nrChannels, 0);

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

	// tells each uniform sampler in the fragment shader which texture unit they belong to (only has to be done once hence why it is out of the render loop)  
	ourShader.use();
	// manualy like this 
	glUniform1i(glGetUniformLocation(ourShader.ID, "ourTexture1"), 0);
	// or via the texture class
	ourShader.setInt("ourTexture2", 1);


	// Checks if GLFW has been instructed to close (this is the render loop)
	while (!glfwWindowShouldClose(window))
	{
		// Inputs below here:
		
		// Function for closing the window with ESC
		processInput(window);


		// Rendering commands below here:

		// Change the color of the window
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Activates a texture unit. we can have up to 16 per shader GL_TEXTURE0 - 15
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		ourShader.use();

		//glUniform1i(glGetUniformLocation(ourShader.ID, "ourTexture1"), 0);
		//ourShader.setInt("ourTexture1", 1);

		// moves the object on the screen
		ourShader.setFloat("xOffset", xOffset);
		ourShader.setFloat("yOffset", yOffset);
		ourShader.setFloat("blendScale", blendScale);


		glBindVertexArray(VAO);
		// Draws the first triangle from the latest binded VAO
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		// Draws the elements from EBO
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		// Check and call events and swap the buffers below here
		
		// Swaps the color buffer 
		// (large 2D buffer that contains color values for each pixel in GLFWs window)
		// that is used to render and show as output on the screen
		glfwSwapBuffers(window);

		// Checks if any event is triggerd like keyboard input and mouse movement
		// We can use callback functions here to do stuff with input
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	// Cleans up all the resources used and properly exits the application
	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	// Tells OpenGL Potition of window and size
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	// Checks if the user has pressed ESCAPE and closes the window if true
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

// A callback function that registers a key press and does something after that
void KeyCallbacks(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
	if (action == GLFW_RELEASE)
	{
		return;
	}

	switch (key)
	{
	case GLFW_KEY_DOWN:
		yOffset -= 0.1;
		break;

	case GLFW_KEY_UP:
		yOffset += 0.1;
		break;

	case GLFW_KEY_LEFT:
		xOffset -= 0.1;
		break;

	case GLFW_KEY_RIGHT:
		xOffset += 0.1;
		break;

	case GLFW_KEY_M:
		blendScale += 0.1;
		break;

	case GLFW_KEY_N:
		blendScale -= 0.1;
		break;

		default:
			break;
	}


	// Click Toggle
	//if (key == GLFW_KEY_L && action == GLFW_PRESS)
	//{
	//	if (drawLines == true)
	//	{
	//		drawLines = false;
	//	}
	//	else if (drawLines == false)
	//	{
	//		drawLines = true;
	//	}

	//	if (drawLines == true)
	//	{
	//		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//	}
	//	if (drawLines == false)
	//	{
	//		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//	}
	//}

	// Hold Toggle
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (key == GLFW_KEY_L && action == GLFW_RELEASE)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

