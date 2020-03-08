#include "PointCloud.h"
#include <iostream>
#include <math.h>
using namespace std;

PointCloud::PointCloud(std::string objFilename, GLfloat pointSize)
	: pointSize(pointSize)
{
	/*
	 * TODO: Section 2: Currently, all the points are hard coded below.
	 * Modify this to read points from an obj file.
	 * Don't forget to load in the object normals for normal coloring as well
	 */
    translate_vec = glm::vec3(0.0f,0.0f,0.0f);
    back_vec = glm::vec3(0.0f,0.0f,0.0f);
    rotate_vec = glm::vec3(0.0f, 1.0f, 0.0f);
    reset = false;
    pressed = false;
    pressed_right = false;
    start_x = 0;
    start_y = 0;
    current_x = 0;
    current_y = 0;
    
    FILE* fp;
    
    fp = fopen(objFilename.c_str(),"rb");
    if(fp == NULL){cout << "error open file" << endl; exit(-1);}
    
    int c1, c2 , res;
    float x, y, z, r, g, b;
    while(1)
    {
        c1 = fgetc(fp);
        if (c1 == EOF)
        {
            break;
        }
        else if (c1 == '#')
        {
            while(res != '\n')
            {
                res = fgetc(fp);
            }
            continue;
        }
        c2 = fgetc(fp);
        if (c1 == 'v' && c2 == ' ')
        {
            res = fscanf(fp, "%f %f %f %f %f %f\r\n", &x, &y, &z, &r, &g, &b);
            points.push_back(glm::vec3(x,y,z));
        }
        else if (c1 == 'v' && c2 == 'n')
        {
            res = fscanf(fp, "%f %f %f\r\n", &x, &y, &z);
            points_norm.push_back(glm::vec3(x,y,z));
        }
        else if (c1 == 'f' && c2 == ' ')
        {
            unsigned int index[3], a, b, c;
            fscanf(fp, "%d//%d %d//%d %d//%d\r\n",
                   &index[0],&a,&index[1],&b,&index[2],&c);
            faces.push_back(index[0] - 1);
            faces.push_back(index[1] - 1);
            faces.push_back(index[2] - 1);
        }
    }


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
    float scale = max(max(x_scale, y_scale), z_scale);
    
    center_vec = glm::vec3(-x_mid,-y_mid,-z_mid);
    scale_vec = glm::vec3(1.0f/scale,1.0f/scale,1.0f/scale);
    
    
	 /* TODO: Section 4, you will need to normalize the object to fit in the
	 * screen.
	 */

	 // Set the model matrix to an identity matrix. 
	model = glm::mat4(1);
	// Set the color. 
	color = glm::vec3(1, 0, 0);

	// Generate a vertex array (VAO) and a vertex buffer objects (VBO).
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	// Bind to the VAO.
	// This tells OpenGL which data it should be paying attention to
	glBindVertexArray(vao);

	// Bind to the first VBO. We will use it to store the points.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(),
		points.data(), GL_STATIC_DRAW);
	// Enable vertex attribute 0. 
	// We will be able to access points through it.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	/*
	 * TODO: Section 2 and 3. 	 
	 * Following the above example but for vertex normals, 
	 * 1) Generate a new vertex bufferbuffer,
	 * 2) Bind it as a GL_ARRAY_BUFFER type, 
	 * 3) Pass in the data 
	 * 4) Enable the next attribute array (which is 1)
	 * 5) Tell it how to process each vertex using glVertexAttribPointer
	 */
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


    setUp();
}

PointCloud::~PointCloud()
{
	// Delete the VBO and the VAO.
	// Failure to delete your VAOs, VBOs and other data given to OpenGL
	// is dangerous and may slow your program and cause memory leaks
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void PointCloud::draw()
{
	// Bind to the VAO.
	glBindVertexArray(vao);

    glDrawElements(GL_TRIANGLES, (int)faces.size(), GL_UNSIGNED_INT, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);
}

void PointCloud::update()
{
	// Spin the cube by 1 degree.
	//spin(0.1f);
	/*
	 * TODO: Section 3: Modify this function to spin the dragon and bunny about
	 * different axes. Look at the spin function for an idea
	 */
    if(reset){
        model[3] = glm::vec4(glm::vec3(0.0f,0.0f,0.0f), 1.0f);
        translate_vec = glm::vec3(0.0f, 0.0f, 0.0f);
        reset = false;
    }
}

void PointCloud::updatePointSize(GLfloat size)
{
	/*
	 * TODO: Section 3: Implement this function to adjust the point size.
	 */
    if(size > 0){pointSize = pointSize + 1.0f;}
    else{pointSize = pointSize - 1.0f;}
}

void PointCloud::spin(float deg)
{
	// Update the model matrix by multiplying a rotation matrix
	model = glm::rotate(model, glm::radians(deg), rotate_vec);
    model[3] = glm::vec4(translate_vec, 1.0f);
}

void PointCloud::setUp(){
    model = glm::translate(glm::mat4(1.0f), center_vec) * model;
    model = glm::scale(glm::mat4(1.0f), scale_vec) * model;
}

void PointCloud::transform(std::string direction){
    if(direction == "left"){
        model = glm::translate(glm::mat4(1.0f), glm::vec3(-4.0f,0.0f,0.0f)) * model;
        back_vec += glm::vec3(-4.0f,0.0f,0.0f);
    }
    if(direction == "right"){
        model = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f,0.0f,0.0f)) * model;
        back_vec += glm::vec3(4.0f,0.0f,0.0f);
    }
    if(direction == "up"){
        model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,4.0f,0.0f)) * model;
        back_vec += glm::vec3(0.0f,4.0f,0.0f);
    }
    if(direction == "down"){
        model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,-4.0f,0.0f)) * model;
        back_vec += glm::vec3(0.0f,-4.0f,0.0f);
    }
    if(direction == "in"){
        model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,0.0f,-4.0f)) * model;
        back_vec += glm::vec3(0.0f,0.0f,-4.0f);
    }
    if(direction == "out"){
        model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,0.0f,4.0f)) * model;
        back_vec += glm::vec3(0.0f,0.0f,4.0f);
    }
}

void PointCloud::reset_position(){
    reset = true;
}

void PointCloud::reset_R(){
    model[0] = glm::vec4(1.0f,0.0f,0.0f,0.0f);
    model[1] = glm::vec4(0.0f,1.0f,0.0f,0.0f);
    model[2] = glm::vec4(0.0f,0.0f,1.0f,0.0f);
}

void PointCloud::scaleUp(bool up){
    if(up){
        model = glm::translate(glm::mat4(1.0f), -back_vec) * model;
        model = glm::scale(glm::mat4(1.0f),glm::vec3(2.0f)) * model;
        model = glm::translate(glm::mat4(1.0f), back_vec) * model;
    }
    else{
        model = glm::translate(glm::mat4(1.0f), -back_vec) * model;
        model = glm::scale(glm::mat4(1.0f),glm::vec3(0.5f)) * model;
        model = glm::translate(glm::mat4(1.0f), back_vec) * model;
    }
}

std::vector<glm::vec3> PointCloud::get_points(){
    return points;
}

std::vector<glm::vec3> PointCloud::get_points_norm(){
    return points_norm;
}

void PointCloud::press(bool pressing){
    if(pressing){
        pressed = true;
        start_x = min(max(0.0f,current_x), width);
        start_y = min(max(0.0f,current_y), height);
    }
    else{
        pressed = false;
    }
}

void PointCloud::set_cursor_position(float xpos, float ypos){
    current_x = min(max(0.0f,xpos), width);
    current_y = min(max(0.0f,ypos), height);
    //cout << "x: " <<current_x <<endl;
    //cout << "y: " <<current_y <<endl;
    
}

void PointCloud::calculate_rotation(){
    if(pressed && (current_x != start_x && current_y != start_y)){
        
        glm::vec3 v;
        float d;
        v.x = (2.0f*start_x - width) / width;
        v.y = (height - 2.0f*start_y) / height;
        v.z = 0.0f;
        d = glm::length(v);
        d = (d < 1.0f) ? d : 1.0f;
        v.z = sqrtf(1.001f - d*d);
        v = glm::normalize(v);
        
        glm::vec3 w;
        w.x = (2.0f*current_x - width) / width;
        w.y = (height - 2.0f*current_y) / height;
        w.z = 0.0f;
        d = glm::length(w);
        d = (d < 1.0f) ? d : 1.0f;
        w.z = sqrtf(1.001f - d*d);
        w = glm::normalize(w);
        
        float deg = acos(glm::dot(v, w) / (glm::length(v) * glm::length(w)));
        
        glm::vec3 trackball = glm::cross(v, w);
        
        if(glm::length(trackball) > 0.001f && abs(deg) > 0.001f){
    
            model = glm::translate(glm::mat4(1.0f), -back_vec) * model;
            model = glm::rotate(glm::mat4(1.0f), glm::radians(100.0f * deg), trackball) * model;
            model = glm::translate(glm::mat4(1.0f), back_vec) * model;
        }
        start_x = current_x;
        start_y = current_y;
    }
}

void PointCloud::calculate_light_rotation(){
    if(pressed && (current_x != start_x && current_y != start_y)){
        
        glm::vec3 v;
        float d;
        v.x = (2.0f*start_x - width) / width;
        v.y = (height - 2.0f*start_y) / height;
        v.z = 0.0f;
        d = glm::length(v);
        d = (d < 1.0f) ? d : 1.0f;
        v.z = sqrtf(1.001f - d*d);
        v = glm::normalize(v);
        
        glm::vec3 w;
        w.x = (2.0f*current_x - width) / width;
        w.y = (height - 2.0f*current_y) / height;
        w.z = 0.0f;
        d = glm::length(w);
        d = (d < 1.0f) ? d : 1.0f;
        w.z = sqrtf(1.001f - d*d);
        w = glm::normalize(w);
        
        float deg = acos(glm::dot(v, w) / (glm::length(v) * glm::length(w)));
        
        glm::vec3 trackball = glm::cross(v, w);
        
        if(glm::length(trackball) > 0.001f && abs(deg) > 0.001f){
    
            model = glm::rotate(glm::mat4(1.0f), glm::radians(100.0f * deg), trackball) * model;
            back_vec = glm::rotate(glm::mat4(1.0f), glm::radians(100.0f * deg), trackball) * glm::vec4(back_vec, 1.0);
        }
        start_x = current_x;
        start_y = current_y;
    }
}

void PointCloud::press_right(bool pressing){
    if(pressing){
        pressed_right = true;
        start_x = min(max(0.0f,current_x), width);
        start_y = min(max(0.0f,current_y), height);
    }
    else{
        pressed_right = false;
    }
}

void PointCloud::calculate_translation(){
    if(pressed_right && (current_x != start_x && current_y != start_y)){
        glm::vec3 v;
        v.x = (current_x - start_x);
        v.y = (start_y - current_y);
        v.z = 0.0f;
        
        //v = glm::normalize(v);
        v = 0.05f * v;
        model = glm::translate(glm::mat4(1.0f), v) * model;
        back_vec += v;
        
        start_x = current_x;
        start_y = current_y;
    }
}



void PointCloud::move_z(float yoffset){
    glm::vec3 z = glm::vec3(0.0f, 0.0f, 0.5f*yoffset);
    model = glm::translate(glm::mat4(1.0f), z) * model;
    back_vec += z;
}

void PointCloud::set_size(int width, int height){
    this->width = float(width);
    this->height = float(height);
}

glm::vec3 PointCloud::get_pos(){
    return back_vec;
}
