#ifndef _POINT_CLOUD_H_
#define _POINT_CLOUD_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <vector>
#include <string>

#include "Object.h"

class PointCloud : public Object
{
private:
	std::vector<glm::vec3> points;
    std::vector<glm::vec3> points_norm;
    std::vector<unsigned int> faces;

    GLuint vao, vbo, vbo_n;
	GLfloat pointSize;
    unsigned int ebo;
    glm::vec3 rotate_vec;
    glm::vec3 translate_vec;
    glm::vec3 center_vec;
    glm::vec3 scale_vec;
    glm::vec3 norm_center_vec;
    glm::vec3 norm_scale_vec;
    glm::vec3 back_vec;
    float current_x, current_y, start_x, start_y;
    float width, height;
    
    bool reset;
    bool pressed;
    bool pressed_right;
public:
	PointCloud(std::string objFilename, GLfloat pointSize);
	~PointCloud();

	void draw();
	void update();

	void updatePointSize(GLfloat size);
	void spin(float deg);
    void setUp();
    void transform(std::string direction);
    void reset_position();
    void scaleUp(bool up);
    void reset_R();
    void press(bool pressing);
    void press_right(bool pressing);
    void set_cursor_position(float xpos, float ypos);
    void calculate_rotation();
    void calculate_translation();
    void move_z(float yoffset);
    void set_size(int width, int height);
    void calculate_light_rotation();
    std::vector<glm::vec3> get_points();
    std::vector<glm::vec3> get_points_norm();
    glm::vec3 get_pos();
};

#endif

