//
//  Node.hpp
//  CSE167 HW3
//
//  Created by 孙楚华 on 2/21/20.
//  Copyright © 2020 孙楚华. All rights reserved.
//

#ifndef Node_hpp
#define Node_hpp

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
#include <iostream>
#include <vector>
#include <string>
#include <memory>

#include "Object.h"
#include "Cube.h"
#include "PointCloud.h"
#include "shader.h"

class Node{
public:
    virtual void draw(glm::mat4 C) = 0;
    virtual void update() = 0;
    virtual void set_values(glm::vec3 vec) = 0;
};
#endif /* Node_hpp */
