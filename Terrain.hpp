//
//  Terrain.hpp
//  CSE167 HW4
//
//  Created by 孙楚华 on 3/4/20.
//  Copyright © 2020 孙楚华. All rights reserved.
//

#ifndef Terrain_hpp
#define Terrain_hpp
#include <stdio.h>
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
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <stdio.h>
#include <math.h>
#include "Object.h"#include <stdlib.h>
#include <time.h>
#include <cstdlib>
#include <ctime>




class Terrain : public Object
{
private:
    GLuint vao, vbo, vbo_n;
    unsigned int ebo;
    std::vector<std::vector<float>> z;
    std::vector<std::vector<glm::vec3>> table;
    int size;
    int length;
    float reduce;
    float inter;
    std::vector<glm::vec3> points;
    std::vector<glm::vec3> points_norm;
    std::vector<unsigned int> faces;
public:
    Terrain(int n, int len, float corner1, float conrer2, float conrer3, float corner4, float reach);
    ~Terrain();
    
    void draw();
    void update();
    
    void diamondSquare(int startx, int endx, int starty, int endy, float reduce, int iter);
    inline static float random(float range, int time);
    
};
#endif /* Terrain_hpp */
