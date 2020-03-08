//
//  Curve.hpp
//  CSE167 HW3
//
//  Created by 孙楚华 on 2/24/20.
//  Copyright © 2020 孙楚华. All rights reserved.
//

#ifndef Curve_hpp
#define Curve_hpp

#include <stdio.h>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include "Object.h"


class Curve : public Object
{
private:
    GLuint vao;
    GLuint vbos[1];
public:
    std::vector<glm::vec3> points;
    Curve();
    ~Curve();

    void draw();
    void update();
    std::vector<glm::vec3> get_points();
};
#endif /* Curve_hpp */
