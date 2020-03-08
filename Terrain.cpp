//
//  Terrain.cpp
//  CSE167 HW4
//
//  Created by 孙楚华 on 3/4/20.
//  Copyright © 2020 孙楚华. All rights reserved.
//

#include "Terrain.hpp"

Terrain::Terrain(int n, int len, float corner1, float corner2, float corner3, float corner4, float reach){
    length = len;
    size = pow(2, n) + 1;
    reduce = reach;
    
    for(unsigned int i = 0; i < size; i++){
        std::vector<float> temp;
        for(unsigned int j = 0; j < size; j++){
            temp.push_back(0.0);
        }
        z.push_back(temp);
    }
    
    for(unsigned int i = 0; i < size; i++){
        std::vector<glm::vec3> temp;
        for(unsigned int j = 0; j < size; j++){
            temp.push_back(glm::vec3(0.0));
        }
        table.push_back(temp);
    }
    
    z[0][0] = corner1;
    z[0][size-1] = corner2;
    z[size-1][0] = corner3;
    z[size-1][size-1] = corner4;
    
    diamondSquare(0, size - 1, 0, size - 1, reduce, 0);
    
    inter = float(length) / float(size);
//    std::cout << inter <<std::endl;
//    std::cout << length <<std::endl;
//    std::cout << size <<std::endl;
    
    for(unsigned int i = 0; i < size; i++){
        for(unsigned int j = 0; j < size; j++){
            points.push_back(glm::vec3(0.0 + j * inter, z[i][j], 0.0 - i * inter));
            table[i][j] = glm::vec3(0.0 + j * inter, z[i][j], 0.0 - i * inter);
        }
    }
    
    for(unsigned int i = 0; i < size; i++){
        for(unsigned int j = 0; j < size; j++){
            if(i == 0){
                if(j == 0){ //left-up cornor
                    glm::vec3 hori = table[i][j+1] - table[i][j];
                    glm::vec3 vert = table[i+1][j] - table[i][j];
                    glm::vec3 norm = glm::cross(hori, vert);
                    points_norm.push_back(norm);
                }
                else if(j == size - 1){ //right-up cornor
                    glm::vec3 hori = table[i][j] - table[i][j-1];
                    glm::vec3 vert = table[i+1][j] - table[i][j];
                    glm::vec3 norm = glm::cross(hori, vert);
                    points_norm.push_back(norm);
                }
                else{ //up-edge
                    glm::vec3 hori = table[i][j+1] - table[i][j-1];
                    glm::vec3 vert = table[i+1][j] - table[i][j];
                    glm::vec3 norm = glm::cross(hori, vert);
                    points_norm.push_back(norm);
                }
            }
            else if(i == size - 1){
                if(j == 0){ //left-down cornor
                    glm::vec3 hori = table[i][j+1] - table[i][j];
                    glm::vec3 vert = table[i][j] - table[i-1][j];
                    glm::vec3 norm = glm::cross(hori, vert);
                    points_norm.push_back(norm);
                }
                else if(j == size - 1){ //right-down cornor
                    glm::vec3 hori = table[i][j] - table[i][j-1];
                    glm::vec3 vert = table[i][j] - table[i-1][j];
                    glm::vec3 norm = glm::cross(hori, vert);
                    points_norm.push_back(norm);
                }
                else{ //down-edge
                    glm::vec3 hori = table[i][j+1] - table[i][j-1];
                    glm::vec3 vert = table[i][j] - table[i-1][j];
                    glm::vec3 norm = glm::cross(hori, vert);
                    points_norm.push_back(norm);
                }
            }
            else{
                if(j == 0){ //left-edge
                    glm::vec3 hori = table[i][j+1] - table[i][j];
                    glm::vec3 vert = table[i+1][j] - table[i-1][j];
                    glm::vec3 norm = glm::cross(hori, vert);
                    points_norm.push_back(norm);
                }
                else if(j == size - 1){ //right-edge
                    glm::vec3 hori = table[i][j] - table[i][j-1];
                    glm::vec3 vert = table[i+1][j] - table[i-1][j];
                    glm::vec3 norm = glm::cross(hori, vert);
                    points_norm.push_back(norm);
                }
                else{ //internal
                    glm::vec3 hori = table[i][j+1] - table[i][j-1];
                    glm::vec3 vert = table[i+1][j] - table[i-1][j];
                    glm::vec3 norm = glm::cross(hori, vert);
                    points_norm.push_back(norm);
                }
            }
        }
    }
    
    for(unsigned int i = 0; i < size - 1; i++){
        for(unsigned int j = 0; j < size - 1; j++){
            faces.push_back(j + i * size);
            faces.push_back(j + size + i * size);
            faces.push_back(j + 1 + i * size);
            faces.push_back(j + 1 + i * size);
            faces.push_back(j + size + i * size);
            faces.push_back(j + size + 1 + i * size);
        }
    }
    
//    for(unsigned int i = 0; i < points.size(); i++){
//        std::cout << glm::to_string(points[i]) << std::endl;
//    }
//
//    std::cout << "===========================================================================" << std::endl;
//
//    for(unsigned int i = 0; i < points_norm.size(); i++){
//        std::cout << glm::to_string(points_norm[i]) << std::endl;
//    }
//
//    std::cout << "===========================================================================" << std::endl;
//
//    for(unsigned int i = 0; i < faces.size(); i++){
//        std::cout << faces[i] << std::endl;
//    }
//
    
    srand((unsigned)time(NULL));
    
    model = glm::mat4(1);
    color = glm::vec3(1, 0, 0);
    
    
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
    // Pass in the data.
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points_norm.size(),
        points_norm.data(), GL_STATIC_DRAW);
    // Enable vertex attribute 1.
    // We will be able to access points through it.
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * faces.size(), faces.data(), GL_STATIC_DRAW);
    
    // Unbind from the VBO.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind from the VAO.
    glBindVertexArray(0);
    
}

Terrain::~Terrain(){
}

void Terrain::diamondSquare(int startx, int endx, int starty, int endy, float reduce, int iter){
    int leng = endx - startx;
    
    if(leng == 1){
        return;
    }
    int midx = (startx + endx) / 2;
    int midy = (starty + endy) / 2;
    
    //mid point
    float avg = 0.0;
    avg += z[startx][starty] + z[startx][endy] + z[endx][starty] + z[endx][endy];
    avg /= 4.0;
    z[midx][midy] = avg + random(reduce, iter);
    
    //up point
    avg = 0.0;
    avg += z[startx][starty] + z[midx][midy] + z[startx][endy];
    if(midx - leng >= 0){
        avg += z[midx - leng][midy];
        avg /= 4.0;
        z[startx][midy] = avg + random(reduce, iter);
    }
    else{
        avg /= 3.0;
        z[startx][midy] = avg + random(reduce, iter);
    }
    
    //down point
    avg = 0.0;
    avg += z[endx][starty] + z[endx][endy] + z[midx][midy];
    if(midx + leng < size){
        avg += z[midx + leng][midy];
        avg /= 4.0;
        z[endx][midy] = avg + random(reduce, iter);
    }
    else{
        avg /= 3.0;
        z[endx][midy] = avg + random(reduce, iter);
    }
    
    //left point
    avg = 0.0;
    avg += z[startx][starty] + z[endx][starty] + z[midx][midy];
    if(midy - leng >= 0){
        avg += z[midx][midy - leng];
        avg /= 4.0;
        z[midx][starty] = avg + random(reduce, iter);
    }
    else{
        avg /= 3.0;
        z[midx][starty] = avg + random(reduce, iter);
    }
    
    //right point
    avg = 0.0;
    avg += z[startx][endy] + z[endx][endy] + z[midx][midy];
    if(midy + leng < size){
        avg += z[midx][midy + leng];
        avg /= 4.0;
        z[midx][endy] = avg + random(reduce, iter);
    }
    else{
        avg /= 3.0;
        z[midx][endy] = avg + random(reduce, iter);
    }
    
    //divide
    diamondSquare(startx, midx, starty, midy, reduce, iter++); //up-left
    diamondSquare(midx, endx, starty, midy, reduce, iter++); //down-left
    diamondSquare(startx, midx, midy, endy, reduce, iter++); //up-right
    diamondSquare(midx, endx, midy, endy, reduce, iter++); //down-right
}

float Terrain::random(float range, int times){
    int tmprand = rand() % int(10 * range - 2.0 * times+ 1);
    return tmprand / 10.0;
}

void Terrain::draw(){
    glBindVertexArray(vao);

    glDrawElements(GL_TRIANGLES, (int)faces.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

void Terrain::update(){

}
