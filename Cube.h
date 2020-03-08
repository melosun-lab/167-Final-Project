#ifndef _CUBE_H_
#define _CUBE_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>

#include "Object.h"

class Cube : public Object
{
private:
	GLuint vao;
	GLuint vbos[2];
    unsigned int ID;
public:
	Cube(float size);
	~Cube();

	void draw();
	void update();

	void spin(float deg);
    
    void setID(unsigned int ID);
    
    void setTranslate(float x, float y, float z);
    
    void setScale(float x, float y, float z);
};

#endif

