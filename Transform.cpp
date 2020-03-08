//
//  Transform.cpp
//  CSE167 HW3
//
//  Created by 孙楚华 on 2/21/20.
//  Copyright © 2020 孙楚华. All rights reserved.
//

#include "Transform.hpp"

Transform::Transform(glm::mat4 transform){
    //M = transform;
    R = glm::mat4(1.0f);
    S = glm::mat4(1.0f);
    M = transform;
    rotate_vec = glm::vec3(1.0f);
    deg = 0.0f;
    spin = false;
    forward = true;
    limit = 30.0;
    origin = glm::vec3(0.0f);
}

Transform::~Transform(){}

void Transform::addChild(Node* child){
    children.push_back(child);
}

void Transform::removeChild(Node* child){
    for(unsigned int i = 0; i < children.size(); i++){
        if(children[i] == child){
            children.erase(children.begin() + i);
            delete child;
        }
    }
}

void Transform::draw(glm::mat4 C){
    glm::mat4 temp;
    if(spin){
        temp = C * glm::translate(glm::mat4(1.0f), -move_vec) * glm::rotate(glm::mat4(1.0f), glm::radians(deg), rotate_vec) * glm::translate(glm::mat4(1.0f), move_vec) * M;
    }
    else{
        temp = C * M;
    }
    temp = glm::translate(glm::mat4(1.0f), -origin) *  temp;
    temp = R * temp;
    temp = glm::translate(glm::mat4(1.0f), origin) * temp;
    for(unsigned int i = 0; i < children.size(); i++){
        children[i]->draw(temp);
    }
}

void Transform::update(){
    if(std::abs(deg) >= limit){
        forward = !forward;
    }
    if(forward){
        deg += 0.02f;
    }
    else{
        deg -= 0.02f;
    }
    for(unsigned int i = 0; i < children.size(); i++){
        children[i]->update();
    }
}

void Transform::set_spin(glm::vec3 vec){
    spin = true;
    rotate_vec = vec;
}

void Transform::set_vec(glm::vec3 vec){
    move_vec = vec;
}

void Transform::set_limit(float l){
    limit = l;
}

void Transform::set_translate(glm::vec3 vec){
    M = glm::translate(glm::mat4(1.0f), vec) * M;
}

void Transform::set_rotate(glm::vec3 vec, float deg){
    R = glm::rotate(glm::mat4(1.0f), glm::radians(100.0f * deg), vec) * R;
}

void Transform::set_values(glm::vec3 vec){
    origin = vec;
    for(unsigned int i = 0; i < children.size(); i++){
        children[i]->set_values(vec);
    }
}

void Transform::set_scale(double yoffset){
    if(yoffset < 0){
        M = glm::scale(glm::mat4(1.0f), glm::vec3(1.1f, 1.1f, 1.1f)) * M;
    }
    if(yoffset > 0){
        M = glm::scale(glm::mat4(1.0f), glm::vec3(0.9f, 0.9f, 0.9f)) * M;
    }
}
