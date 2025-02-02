#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <random>
#include <Debugger.h>
#include <Shader.h>
#include "RubiksCube.h"
#include <iostream>

class Camera
{
private:
    // View and Projection
    glm::mat4 m_View = glm::mat4(1.0f);
    glm::mat4 m_Projection = glm::mat4(1.0f);

    // View matrix paramters
    glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 m_Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 m_Up = glm::vec3(0.0f, 1.0f, 0.0f);

    // Projection matrix parameters
    float m_Near = 0.1f;
    float m_Far = 100.0f;
    int m_Width;
    int m_Height;

    RubiksCube *rubi;
    float rot_angle = 90.0f;
    bool clockwise = true;
    int right_state = 0;
    int left_state = 0;
    int down_state = 0;
    int up_state = 0;
    int front_state = 0;
    int back_state = 0;

    // Perspective Projection parameters
    float m_FOVdegree = 45.0f;
    float m_Aspect = 1.0f;

    // Orthographic Projection parameters
    float m_Left = -1.0f * 2.0f;
    float m_Right = 1.0f * 2.0f;
    float m_Bottom = -1.0f * 2.0f;
    float m_Top = 1.0f * 2.0f;

public:
    // Prevent the camera from jumping around when first clicking left click
    double m_OldMouseX = 0.0;
    double m_OldMouseY = 0.0;
    double m_NewMouseX = 0.0;
    double m_NewMouseY = 0.0;

    // Adjust the speed of the camera and it's sensitivity when looking around
    float m_KeySensitivity = 0.4f;
    float m_MouseSensitivity = 0.05f;
    float m_ScrollSensitivity = 1.0f;

public:
    Camera(int width, int height, RubiksCube *_rubi)
        : m_Width(width), m_Height(height), rubi(_rubi) {};

    // Update Projection matrix for Perspective mode
    void SetPerspective(float FOVdegree, float near, float far);

    // Update Projection matrix for Orthographic mode
    void SetOrthographic(float near, float far);

    void TranslateProjection(glm::vec3 trans);
    void TranslateView(glm::vec3 trans);

    void RotateProjection(float angle, glm::vec3 rot);
    void RotateView(float angle, glm::vec3 rot);

    // Handle camera inputs
    void EnableInputs(GLFWwindow *window);

    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix() const;
    RubiksCube &getRubiksCube() const;
    float getRotAngle() const;
    bool getClockwise() const;
    void setRotAngle(float angle);
    void setClockwise(bool flag);

    int getRightState() const;
    void setRightState(int state);
    int getLeftState() const;
    void setLeftState(int state);
    int getUpState() const;
    void setUpState(int state);
    int getDownState() const;
    void setDownState(int state);
    int getFrontState() const;
    void setFrontState(int state);
    int getBackState() const;
    void setBackState(int state);

    bool m_picking_mode = false;
    Cube *picked_cube = nullptr;
    float m_Depth = -1.0f;
    float get_near();
    float get_far();
    float get_width();
    float get_AspectRatio();
    glm::vec3 &get_position();

    static void RandomMixer(GLFWwindow *window, Camera *camera);
};