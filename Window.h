#ifndef _WINDOW_H_
#define _WINDOW_H_

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <memory>

#include "Object.h"
#include "Cube.h"
#include "PointCloud.h"
#include "shader.h"
#include "Transform.hpp"
#include "Geometry.hpp"
#include "Curve.hpp"
#include "Terrain.hpp"

class Window
{
public:
    typedef struct { float x, y, z; } vector;
    typedef struct { float m[16]; } matrix;
    typedef struct __attribute__((packed)) { char magic[2]; unsigned int size, reserved, offset, hsize, width, height, colors, compression, image_size, h_res, v_res, palletes, colors2; } bmp_header;

    typedef struct { float x, y, z, r, r2; double px, py; } gamestate;
    typedef struct { GLuint vao, buffer, vertices, program, textures[256], fb; int depth_test, texcount, M, V, P, tex, time;} entity;
    typedef struct { std::vector<entity*> entities; unsigned int entity_count; gamestate state; } scene;
	// Initializes your shader program(s) and uniform variable locations
	static bool initializeProgram();
	// Initialize your OBJ objects here
	static bool initializeObjects();
	// Make sure to clean up any data you dynamically allocated here
	static void cleanUp();
	// Creates a GLFW window context
	static GLFWwindow* createWindow(int width, int height);
	// Is called whenever the window is resized
	static void resizeCallback(GLFWwindow* window, int width, int height);
	// Is called on idle.
	static void idleCallback();
	// This renders to the glfw window. Add draw calls here
	static void displayCallback(GLFWwindow*);
	// Add your key press event handling here
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void cursorCallback(GLFWwindow* window, double xpos, double ypos);
    
    static void buttonCallback(GLFWwindow* window, int button, int action, int mods);
    
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    
    static unsigned char* loadPPM(const char* filename, int& width, int& height);
    
    static void loadTexture();
    
    static void initialzieTransfroms();
    
    static void processInput(GLFWwindow *window);
};

#endif
