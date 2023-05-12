#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shaders.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void KeyCallbackToggleLines(GLFWwindow* window, int key, int scancode, int action, int mods);

static bool drawLines = false;

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
		// positions         // colors
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
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
	unsigned int VBO;
	glGenBuffers(1, &VBO);


	// Initialization code (done once(unless the object frequently changes))

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Position attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Color attributes
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(1);


	// Press 'L' to change from Line or Fill triangles
	// Sets the keycallback we created to a specific window
	// This is used if we a key press only do someting once per click
	glfwSetKeyCallback(window, KeyCallbackToggleLines);

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

		ourShader.use();
		glBindVertexArray(VAO);
		// Draws the first triangle from the latest binded VAO
		glDrawArrays(GL_TRIANGLES, 0, 3);


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
void KeyCallbackToggleLines(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
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

