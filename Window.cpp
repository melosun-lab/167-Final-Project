#include "Window.h"
#include <stdlib.h>
#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <string.h>
#include <math.h>   // include math functions, such as sin, cos, M_PI
#include <iostream> // allow c++ style console printouts
/* 
 * Declare your variables below. Unnamed namespace is used here to avoid 
 * declaring global or static variables.
 */
namespace
{
	int width, height;
	std::string windowTitle("GLFW Starter Project");

	// Boolean switches
    Terrain* ter;
    Cube* cubePoints;
    Cube* skyfloor;
    unsigned int cubemapTexture;

	//PointCloud* currentObj;  // The object currently displaying.

    Cube* currentObj;

	float fovy = 60;
	float near = 1;
	float far = 1000;

    bool press = false;
    bool shift = false;


    std::vector<Transform*> army;
    std::vector<Transform*> temps;

    glm::vec3 cameraPos = glm::vec3(0.0, 2.0, 10.0);
    glm::vec3 cameraFront =  glm::vec3(0.0, 0.0, -1.0);
    glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraFront);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
    glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, up);
	
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    bool firstMouse = true;
    float yaw = -90.0f;
    float pitch = 0.0f;
    float lastX = width / 2.0f;
    float lastY = height / 2.0f;

    //glm::mat4 view = glm::lookAt(eye, center, up); // View matrix, defined by eye, center and up.
	glm::mat4 projection; // Projection matrix.

    GLuint skybox;
    GLuint sky;
    GLuint post;
	GLuint projectionLoc; // Location of projection in shader.
	GLuint viewLoc; // Location of view in shader.
	GLuint modelLoc; // Location of model in shader.
	GLuint colorLoc; // Location of color in shader.
    GLuint modeLoc;

    std::vector<std::string> faces
    {
        "skyright.jpg",
        "skyleft.jpg",
        "skytop.jpg",
        "skybottom.jpg",
        "skyfront.jpg",
        "skyback.jpg"
    };
    
    std::vector<glm::vec3> points;
    std::vector<glm::vec3> direction;

};

bool Window::initializeProgram()
{
    skybox = LoadShaders("shaders/skybox.vert", "shaders/skybox.frag");
    //sky = LoadShaders("shaders/shader.vert", "shaders/shader.frag");
    //post = LoadShaders("shaders/RasterizerQuad.vert", "shaders/RasterizerQuad.frag");

    if (!skybox)
    {
        std::cerr << "Failed to initialize skybox program" << std::endl;
        return false;
    }
    
    loadTexture();


    glUseProgram(skybox);
	projectionLoc = glGetUniformLocation(skybox, "projection");
	viewLoc = glGetUniformLocation(skybox, "view");
	modelLoc = glGetUniformLocation(skybox, "model");
	colorLoc = glGetUniformLocation(skybox, "color");
    modeLoc = glGetUniformLocation(skybox, "mode");
    
    return true;
}

bool Window::initializeObjects()
{
    
    cubePoints = new Cube(1000.0f);
    currentObj = cubePoints;
    
    ter = new Terrain(8, 50, 1, 1, 1, 1, 2);

    cubePoints->setID(cubemapTexture);
    
    skyfloor = new Cube(100.0f);

    skyfloor->setScale(10.0, 0.2, 10.0);
    
	return true;
}

void Window::initialzieTransfroms(){
    
}

void Window::cleanUp()
{
    delete cubePoints;

	// Delete the shader programs.
    glDeleteProgram(skybox);
}

GLFWwindow* Window::createWindow(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

	// 4x antialiasing.
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ 
	// Apple implements its own version of OpenGL and requires special treatments
	// to make it uses modern OpenGL.

	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window.
	GLFWwindow* window = glfwCreateWindow(width, height, windowTitle.c_str(), NULL, NULL);

	// Check if the window could not be created.
	if (!window)
	{
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window.
	glfwMakeContextCurrent(window);

#ifndef __APPLE__
	// On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

	// Initialize GLEW.
	if (glewInit())
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}
#endif

	// Set swap interval to 1.
	glfwSwapInterval(0);
	
	// Initialize the quad that will be textured with your image
	// The quad must be made with the window

	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, width, height);

	return window;
}

void Window::resizeCallback(GLFWwindow* window, int w, int h)
{
#ifdef __APPLE__
	// In case your Mac has a retina display.
	glfwGetFramebufferSize(window, &width, &height);
#endif
	width = w;
	height = h;

	// Set the viewport size.
	glViewport(0, 0, width, height);

	// Set the projection matrix.
	projection = glm::perspective(glm::radians(fovy),
		(float)width / (float)height, near, far);
}

void Window::idleCallback()
{
	// Perform any updates as necessary. 
	cubePoints->update();
}

void Window::displayCallback(GLFWwindow* window)
{
    glUseProgram(skybox);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    
    processInput(window);
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, up);

    glm::mat4 model = cubePoints->getModel();
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniform1f(modeLoc, 0.0f);
    cubePoints->draw();
    
    model = skyfloor->getModel();
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniform1f(modeLoc, 2.0f);
    skyfloor->draw();
    
    glUniform1f(modeLoc, 1.0f);
    
    model = ter->getModel();
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    ter->draw();
    
    
    //glBindFramebuffer(GL_FRAMEBUFFER, s.entities[1]->fb);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //renderScene(s, width, height);
    
    // Gets events, including input such as keyboard and mouse or window resizing.
    glfwPollEvents();
    // Swap buffers.
    glfwSwapBuffers(window);
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	/*
	 * TODO: Section 4: Modify below to add your key callbacks.
	 */
    const float cameraSpeed = 0.005f;
	 // Check for a key press.
	if (action == GLFW_PRESS)
	{
		// Uppercase key presses (shift held down + key press)
		if (mods == GLFW_MOD_SHIFT) {
            switch (key) {
                default:
                    shift = true;
                    break;
            }
		}
        else{
		// Deals with lowercase key presses
            switch (key)
            {
                case GLFW_KEY_ESCAPE:
                    // Close the window. This causes the program to also terminate.
                    glfwSetWindowShouldClose(window, GL_TRUE);
                    break;
                case GLFW_KEY_N:
                // Close the window. This causes the program to also terminate.
                    ter = new Terrain(7, 50, 1, 1, 1, 1, 1);
                    break;
                default:
                    shift = false;
                    break;
            }
        }
	}
     
}

void Window::cursorCallback(GLFWwindow *window, double xpos, double ypos){
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    if(xpos >= width - 1 || xpos <= 1.0){
        glfwSetCursorPos(window, width/2.0, height/2.0);
        lastX = width/2.0;
        lastY = height/2.0;
        xoffset = 0.0;
    }

    float sensitivity = 0.5f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}


void Window::buttonCallback(GLFWwindow* window, int button, int action, int mods){
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        press = true;
    }
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
        press = false;
    }
}

void Window::scrollCallback(GLFWwindow* window, double xoffset, double yoffset){
}

// load image file into texture object
void Window::loadTexture()
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    cubemapTexture = textureID;
}

void Window:: processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 2.5 * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        if(shift){
            cameraPos += cameraSpeed * up;
        }
        else{
            cameraPos -= cameraSpeed * up;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}
