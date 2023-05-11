#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void KeyCallbackToggleLines(GLFWwindow* window, int key, int scancode, int action, int mods);

static bool drawLines = false;

int main() {
	// Vertex shader
	// We declare all the input vertex atributes in "vec3"
	// The location of the input is at 0
	// We store this as a string for the moment, we are gonna compile this in GLSL later
	const char* vertexShaderSource = "#version 460 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

	// Fragment shader same as above (draws the color)
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\n\0";

	const char* fragmentShaderSource2 = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
		"}\n\0";

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


	// Initializing GLAD to manage OpenGL functions
	// glfwGetProcAddress gives us the correct functions based on OS
	// !!!ADD glad.c TO PROJECT FILE!!!!
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	// Tells GLFW to call out callback function every time window size changes
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	//---------------------vvv shaders vvv-----------------------------------------
	// This object will be used to compile to GLSL code
	unsigned int vertexShader;
	// In the parameter you chose what type of shader it is
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// This attatches the source code to the shader object and the compiles it
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// Checks if the compilation of the GLSL code was successfull
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Same as above but for fragment shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;

	}

	// Creates shaderProgram that links the vertex and fragment shader
	unsigned int shaderProgram;
	// Returns the ID reference to the created program object
	shaderProgram = glCreateProgram();
	// Attaching the shaders to the program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Checks if the compilation of the program wa successfull
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// Deletes the shades that have been linked since we no longer need them
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//--------------------------^^^Shaders^^^-------------------------------------

	//---------------------vvv shaders2 vvv-----------------------------------------
	// This object will be used to compile to GLSL code
	unsigned int vertexShader2;
	// In the parameter you chose what type of shader it is
	vertexShader2 = glCreateShader(GL_VERTEX_SHADER);

	// This attatches the source code to the shader object and the compiles it
	glShaderSource(vertexShader2, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader2);

	// Checks if the compilation of the GLSL code was successfull
	glGetShaderiv(vertexShader2, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader2, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Same as above but for fragment shader
	unsigned int fragmentShader2;
	fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
	glCompileShader(fragmentShader2);

	glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader2, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;

	}

	// Creates shaderProgram that links the vertex and fragment shader
	unsigned int shaderProgram2;
	// Returns the ID reference to the created program object
	shaderProgram2 = glCreateProgram();
	// Attaching the shaders to the program
	glAttachShader(shaderProgram2, vertexShader2);
	glAttachShader(shaderProgram2, fragmentShader2);
	glLinkProgram(shaderProgram2);

	// Checks if the compilation of the program wa successfull
	glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram2, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// Deletes the shades that have been linked since we no longer need them
	glDeleteShader(vertexShader2);
	glDeleteShader(fragmentShader2);
	//--------------------------^^^Shaders2^^^-------------------------------------


	// Vertices points of the two triangles
	//float vertices[] = {
	//	 0.5f,  0.5f, 0.0f,  // top right
	//	 0.5f, -0.5f, 0.0f,  // bottom right
	//	-0.5f, -0.5f, 0.0f,  // bottom left
	//	-0.5f,  0.5f, 0.0f   // top left 
	//};

	// Two triangles next to eachother
	float vertices[] = {
		-0.9f, -0.5f, 0.0f, // bot left
		-0.1f, -0.5f, 0.0f, // bot right
		-0.5f,  0.5f, 0.0f, //top
	};
	float vertices2[] = {
		 0.1f, -0.5f, 0.0f, // bot left
		 0.9f, -0.5f, 0.0f, // bot right
		 0.5f,  0.5f, 0.0f //top
	};

	// Points where the triangles should be drawn in between (index drawing)
	// because of this we only need 4 vertices instead of 6 and reduces overlap
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	// (VBO)Vertex buffer object. used to store large amount of vertices to send at the same time to the CPU.
	// Since it is a slow process to send to the CPU we want to send as much data at the same time
	// (VAO)Vertex array object is used to store data from the VBO subsequentaly. OpenGL will no draw anything without this
	// The VAO need to be binded before anything else
	// These object is where shit happens but i am not realy sure what is going on behind the sceens
	// (EBO)Element buffer object is just to store vertices for more than one triangle so we can draw an Element with reduced overlap
	// Multiples of these are in an array and not seperate
	unsigned int VAOs[2];
	glGenVertexArrays(2, VAOs);
	unsigned int VBOs[2], EBO;
	glGenBuffers(2, VBOs);
	glGenBuffers(1, &EBO);


	// Initialization code (done once(unless the object frequently changes))

	// 1. bind the vertex array
	glBindVertexArray(VAOs[0]);

	// 2. Copy out vertices array in a buffer for OpenGL to use
	// Allows us to bind several buffers at once as long as they have different buffer types
	// From this point on the VBO buffer is bound to GL_ARRAY_BUFFER so any call we make to 
	// GL_ARRAY_BUFFER will configure VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);

	// This copies the user-defined data to the currently bound buffer
	// GL_STREAM_DRAW sets the data once and is used by the GPU a few times
	// GL_STATIC_DRAW sets the data once and is used many times
	// GL_DYNAMIC_DRAW the data changes a lot and is used many times
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// For second triangle
	// this need to be done for every new VAO and VBO
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);



	// Same as above but for EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);



	// 3. Then set our vertex atributes pointers
	// The position data is stored as float
	// Location = 0 so we pass in 0
	// We use vec3 so we pass in 3 values
	// We specifies the data type as float
	// If we want nomalaized data to be used we use GL_TRUE
	// Fifth argument is the "stride" it's the distance from one point to another in bytes
	// The last parameter is the offset off where position data begins
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);


	// Press 'L' to change from Line or Fill triangles
	// Sets the keycallback we created to a specific window
	// This is used if we a key press only do someting once per click
	glfwSetKeyCallback(window, KeyCallbackToggleLines);

	// Uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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

		
		// Draws the triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(VAOs[0]);

		// Draws the first triangle from the latest binded VAO
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// If we want diffetent shaders for different triangle 
		// we first need to create the different shader
		// Then we need to call the different shader per different triangle
		glUseProgram(shaderProgram2);
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Draws multple triangles from an array
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		// Check and call events and swap the buffers below here
		
		// Swaps the color buffer 
		// (large 2D buffer that contains color values for each pixel in GLFWs window)
		// that is used to render and show as output on the screen
		glfwSwapBuffers(window);

		// Checks if any event is triggerd like keyboard input and mouse movement
		// We can use callback functions here to do stuff with input
		glfwPollEvents();
	}


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

