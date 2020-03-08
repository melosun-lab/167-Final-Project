//
//  Transform.hpp
//  CSE167 HW3
//
//  Created by 孙楚华 on 2/21/20.
//  Copyright © 2020 孙楚华. All rights reserved.
//

#ifndef Transform_hpp
#define Transform_hpp

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
#include "Node.hpp"


class Transform : public Node
{
private:
    glm::mat4 M;
    glm::mat4 R;
    glm::mat4 S;
    std::vector<Node*> children;
    glm::vec3 rotate_vec;
    glm::vec3 move_vec;
    glm::vec3 origin;
    float deg;
    float limit;
    bool spin;
    bool forward;
public:
    Transform(glm::mat4 transform);
    ~Transform();
    
    void addChild(Node* child);
    void removeChild(Node* child);
    void draw(glm::mat4 C);
    void update();
    void set_spin(glm::vec3 vec);
    void set_vec(glm::vec3 vec);
    void set_limit(float l);
    void set_translate(glm::vec3 vec);
    void set_rotate(glm::vec3 vec, float deg);
    void set_values(glm::vec3 vec);
    void set_scale(double yoffset);
};

#endif /* Transform_hpp */
