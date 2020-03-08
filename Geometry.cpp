//
//  Geometry.cpp
//  CSE167 HW3
//
//  Created by 孙楚华 on 2/21/20.
//  Copyright © 2020 孙楚华. All rights reserved.
//

#include "Geometry.hpp"

Geometry::Geometry(GLuint program, std::string file){
    model = glm::mat4(1.0);
    loadOBJ(file);
    this->program = program;
    modelLoc = glGetUniformLocation(this->program, "model");
    glm::vec3 rotate_vec = glm::vec3(0.0f,0.0f,0.0f);
}

Geometry::~Geometry(){
}

void Geometry::setModel(glm::mat4 C){
    model = C;
    glUseProgram(program);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    
}

void Geometry::loadOBJ(std::string file){
    FILE* fp;
        float x, y, z;
        float xn, yn, zn;
        int fx, fy, fz, nx, ny, nz;
        int c1, c2;
    
        fp = fopen(strcpy(new char[file.length() + 1], file.c_str()), "rb");
    
        if (fp == NULL) { std::cerr << "error loading file" << std::endl; exit(-1); }
    
        c1 = fgetc(fp);
        c2 = c1; // stores the value of the previous character
    
        while (c1 != EOF) {
            if (c2 == 'v' && c1 == ' ') {
                fscanf(fp, "%f %f %f", &x, &y, &z);
                points.push_back(glm::vec3(x, y, z));
            }
            if (c2 == 'v' && c1 == 'n') {
                fscanf(fp, "%f %f %f", &xn, &yn, &zn);
                points_norm.push_back(glm::vec3(xn, yn, zn));
            }
            if (c2 == 'f') {
                fscanf(fp, "%i%*c%*i%*c%i %i%*c%*i%*c%i %i%*c%*i%*c%i", &fx, &nx, &fy, &ny, &fz, &nz);
                //std::cout << fx << std::endl;
                vertices.push_back(fx - 1);
                vertices.push_back(fy - 1);
                vertices.push_back(fz - 1);
                normals.push_back(nx - 1);
                normals.push_back(ny - 1);
                normals.push_back(nz - 1);
            }
            c2 = c1;
            c1 = fgetc(fp);
        }
    
    for(unsigned int i = 0; i < vertices.size(); i++){
        p.push_back(points[vertices[i]]);
        n.push_back(points_norm[normals[i]]);
        faces.push_back(i);
    }
    
    points = p;
    points_norm = n;

    fclose(fp);
    
    float x_max,x_min,y_max,y_min,z_max,z_min;
    
    x_max = std::numeric_limits<float>::min();
    y_max = std::numeric_limits<float>::min();
    z_max = std::numeric_limits<float>::min();
    x_min = std::numeric_limits<float>::max();
    y_min = std::numeric_limits<float>::max();
    z_min = std::numeric_limits<float>::max();
    
    glm::vec3 point;
    
    for(unsigned int i = 0; i < points.size(); i++){
        point = points[i];
        if(point.x < x_min){x_min = point.x;}
        if(point.y < y_min){y_min = point.y;}
        if(point.z < z_min){z_min = point.z;}
        if(point.x > x_max){x_max = point.x;}
        if(point.y > y_max){y_max = point.y;}
        if(point.z > z_max){z_max = point.z;}
    }
    
    float x_mid = (x_max + x_min)/2;
    float y_mid = (y_max + y_min)/2;
    float z_mid = (z_max + z_min)/2;
    float x_scale = (x_max - x_min)/2;
    float y_scale = (y_max - y_min)/2;
    float z_scale = (z_max - z_min)/2;
    float scale = std::max(std::max(x_scale, y_scale), z_scale);
    
    center_vec = glm::vec3(-x_mid,-y_mid,-z_mid);
    scale_vec = glm::vec3(1.0f/scale,1.0f/scale,1.0f/scale);
    
    model = glm::translate(glm::mat4(1.0f), center_vec) * model;
    model = glm::scale(glm::mat4(1.0f), scale_vec) * model;
    
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(),
        points.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    glGenBuffers(1, &vbo_n);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_n);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points_norm.size(),
        points_norm.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * faces.size(), faces.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Geometry::draw(glm::mat4 transform){
    setModel(transform);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, (int)faces.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Geometry::update(){
}

void Geometry::set_values(glm::vec3 vec){
}
