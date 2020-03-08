//
//  Curve.cpp
//  CSE167 HW3
//
//  Created by 孙楚华 on 2/24/20.
//  Copyright © 2020 孙楚华. All rights reserved.
//

#include "Curve.hpp"

#include <iostream>

Curve::Curve()
{
    /*
     * Cube indices used below.
     *    4----7
     *   /|   /|
     *  0-+--3 |
     *  | 5--+-6
     *  |/   |/
     *  1----2
     *
     */
    model = glm::mat4(1.0f);
    
    points.push_back(glm::vec3(0.0,0.0,0.0));

     // The 8 vertices of a cube.
    glm::vec3 a,b,c,d, p;
    
    std::vector<glm::vec3> control_points_1
    {
        glm::vec3(0.0, 0.0, 0.0),
        5.0f*glm::vec3(1.0,1.0, -8.0),
        5.0f*glm::vec3(2.0, -1.0, -9.0),
        5.0f*glm::vec3(-1.0, -2.0, -11.0),
    };
    
    a = -control_points_1[0] + 3.0f*control_points_1[1] - 3.0f*control_points_1[2] + control_points_1[3];
    b = 3.0f*control_points_1[0] - 6.0f*control_points_1[1] + 3.0f*control_points_1[2];
    c = -3.0f*control_points_1[0] + 3.0f*control_points_1[1];
    d = control_points_1[0];
    
    for(float t = 0.01; t < 1; t += 0.01){
        p = a * t * t * t + b * t * t + c *  t + d;
        points.push_back(p);
    }
    
    std::vector<glm::vec3> control_points_2
    {
        5.0f*glm::vec3(-1.0, -2.0, -11.0),
        10.0f*glm::vec3(-1.0, -2.0, -11.0) - 5.0f*glm::vec3(2.0, -1.0, -9.0),
        5.0f*glm::vec3(4.0, -2.0, -7.0),
        5.0f*glm::vec3(-5.0, 4.0, -12.0),
    };
    
    a = -control_points_2[0] + 3.0f*control_points_2[1] - 3.0f*control_points_2[2] + control_points_2[3];
    b = 3.0f*control_points_2[0] - 6.0f*control_points_2[1] + 3.0f*control_points_2[2];
    c = -3.0f*control_points_2[0] + 3.0f*control_points_2[1];
    d = control_points_2[0];
    
    for(float t = 0.01; t < 1; t += 0.01){
        p = a * t * t * t + b * t * t + c *  t + d;
        points.push_back(p);
    }
    
    std::vector<glm::vec3> control_points_3
    {
        5.0f*glm::vec3(-5.0, 4.0, -12.0),
        10.0f*glm::vec3(-5.0, 4.0, -12.0) - 5.0f*glm::vec3(4.0, -2.0, -7.0),
        5.0f*glm::vec3(0.0, 7.0, -15.0),
        5.0f*glm::vec3(3.0, -8.0, -12.0),
    };
    
    a = -control_points_3[0] + 3.0f*control_points_3[1] - 3.0f*control_points_3[2] + control_points_3[3];
    b = 3.0f*control_points_3[0] - 6.0f*control_points_3[1] + 3.0f*control_points_3[2];
    c = -3.0f*control_points_3[0] + 3.0f*control_points_3[1];
    d = control_points_3[0];
    
    for(float t = 0.01; t < 1; t += 0.01){
        p = a * t * t * t + b * t * t + c *  t + d;
        points.push_back(p);
    }
    
    std::vector<glm::vec3> control_points_4
    {
        5.0f*glm::vec3(3.0, -8.0, -12.0),
        10.0f*glm::vec3(3.0, -8.0, -12.0) - 5.0f*glm::vec3(0.0, 7.0, -15.0),
        5.0f*glm::vec3(7.0, -5.0, -6.0),
        5.0f*glm::vec3(5.0, 6.0, -11.0),
    };
    
    a = -control_points_4[0] + 3.0f*control_points_4[1] - 3.0f*control_points_4[2] + control_points_4[3];
    b = 3.0f*control_points_4[0] - 6.0f*control_points_4[1] + 3.0f*control_points_4[2];
    c = -3.0f*control_points_4[0] + 3.0f*control_points_4[1];
    d = control_points_4[0];
    
    for(float t = 0.01; t < 1; t += 0.01){
        p = a * t * t * t + b * t * t + c *  t + d;
        points.push_back(p);
    }
    
    std::vector<glm::vec3> control_points_5
    {
        5.0f*glm::vec3(5.0, 6.0, -11.0),
        10.0f*glm::vec3(5.0, 6.0, -11.0) - 5.0f*glm::vec3(7.0, -5.0, -6.0),
        -5.0f*glm::vec3(1.0,1.0, -8.0),
        glm::vec3(0.0, 0.0, 0.0),
    };
    
    a = -control_points_5[0] + 3.0f*control_points_5[1] - 3.0f*control_points_5[2] + control_points_5[3];
    b = 3.0f*control_points_5[0] - 6.0f*control_points_5[1] + 3.0f*control_points_5[2];
    c = -3.0f*control_points_5[0] + 3.0f*control_points_5[1];
    d = control_points_5[0];
    
    for(float t = 0.01; t < 1; t += 0.005){
        p = a * t * t * t + b * t * t + c *  t + d;
        points.push_back(p);
    }
    
    points.push_back(glm::vec3(0.0,0.0,0.0));


    // Generate a vertex array (VAO) and two vertex buffer objects (VBO).
    glGenVertexArrays(1, &vao);
    glGenBuffers(2, vbos);

    // Bind to the VAO.
    glBindVertexArray(vao);

    // Bind to the first VBO. We will use it to store the vertices.
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    // Pass in the data.
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(),
        points.data(), GL_STATIC_DRAW);
    // Enable vertex attribute 0.
    // We will be able to access vertices through it.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    // Bind to the second VBO. We will use it to store the indices.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[1]);
    // Pass in the data.
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * indices.size(),
//        indices.data(), GL_STATIC_DRAW);

    // Unbind from the VBOs.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind from the VAO.
    glBindVertexArray(0);
}

Curve::~Curve()
{
    // Delete the VBOs and the VAO.
    glDeleteBuffers(2, vbos);
    glDeleteVertexArrays(1, &vao);
}

void Curve::draw()
{
    glBindVertexArray(vao);
    glDrawArrays(GL_LINE_STRIP, 0, points.size());
    glBindVertexArray(0);
}

void Curve::update()
{
}

std::vector<glm::vec3> Curve:: get_points(){
    return points;
}
