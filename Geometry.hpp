//
//  Geometry.hpp
//  CSE167 HW3
//
//  Created by 孙楚华 on 2/21/20.
//  Copyright © 2020 孙楚华. All rights reserved.
//

#ifndef Geometry_hpp
#define Geometry_hpp
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
#include "Node.hpp"
#include <stdio.h>


class Geometry : public Node
{
private:
    glm::mat4 model;
    std::vector<glm::vec3> points;
    std::vector<glm::vec3> points_norm;
    std::vector<unsigned int> faces;
    std::vector<unsigned int> normals;
    std::vector<unsigned int> vertices;
    std::vector<glm::vec3> p;
    std::vector<glm::vec3> n;
    glm::vec3 center_vec;
    glm::vec3 scale_vec;
    GLuint vao, vbo, vbo_n;
    GLuint program;
    GLuint modelLoc;
    unsigned int ebo;
public:
    Geometry(GLuint program, std::string);
    ~Geometry();
    
    void setModel(glm::mat4 C);
    void loadOBJ(std::string file);
    void draw(glm::mat4 transform);
    void update();
    void set_values(glm::vec3 vec);
};
#endif /* Geometry_hpp */
