#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <VertexArray.h>
#include <VertexBuffer.h>
#include <VertexBufferLayout.h>
#include <IndexBuffer.h>

class Cube
{
public:
    // Constructor
    Cube(float x, float y, float z, int id);

    // Getters for transformation matrices
    glm::mat4 getTranslationMatrix() const;
    glm::mat4 getRotationMatrix() const;
    glm::mat4 getScaleMatrix() const;
    void setTranslationMatrix(glm::mat4 &other);
    void setRotationMatrix(glm::mat4 &other);
    void setScaleMatrix(glm::mat4 &other);

    void rotateCube(float radians, glm::vec3 axis);
    void translateCube(glm::vec3 trans_vector, const glm::mat4 &viewMatrix);
    void rotateCubePickingMode(float radians, glm::vec3 axis);

    int id;

private:
    glm::mat4 trans; // Translation matrix
    glm::mat4 rot;   // Rotation matrix
    glm::mat4 scl;   // Scale matrix
};
