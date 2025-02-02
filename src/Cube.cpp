#include "Cube.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Debugger.h>
#include <VertexBuffer.h>
#include <VertexBufferLayout.h>
#include <IndexBuffer.h>
#include <VertexArray.h>
#include <Shader.h>
#include <Texture.h>
#include <Camera.h>

#include <iostream>

// Constructor

Cube::Cube(float x, float y, float z, int _id) : id(_id)
{
    // Initialize translation matrix

    this->trans = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));

    // Initialize rotation matrix as identity
    this->rot = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(1.0f));

    // Initialize scale matrix to represent a 1.0f x 1.0f x 1.0f cube
    this->scl = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
}

// Get translation matrix
glm::mat4 Cube::getTranslationMatrix() const
{
    return trans;
}

// Get rotation matrix
glm::mat4 Cube::getRotationMatrix() const
{
    return rot;
}

// Get scale matrix
glm::mat4 Cube::getScaleMatrix() const
{
    return scl;
}

void Cube::setTranslationMatrix(glm::mat4 &other)
{
    this->trans = other;
}

void Cube::setRotationMatrix(glm::mat4 &other)
{
    this->rot = other;
}
void Cube::setScaleMatrix(glm::mat4 &other)
{
    this->scl = other;
}

void Cube::rotateCube(float radians, glm::vec3 axis)
{
    glm::mat3 rotation = glm::mat3(rot);                                      // Extract the rotation component from the current transformation matrix
    glm::mat3 rot_inverse = glm::transpose(rotation);                         // Get the transpose of the rotation matrix (equivalent to the inverse for rotation-only matrices)
    glm::vec3 rot_vector = rot_inverse * axis;                                // transforms the axis from local space to world space, ensuring the rotation respects the cube's orientation.
    glm::mat4 target_rot = glm::rotate(glm::mat4(1.0f), radians, rot_vector); // Create a new rotation matrix
    rot = rot * target_rot;                                                   // Combine the new rotation with the current rotation matrix
}

void Cube::translateCube(glm::vec3 trans_vector, const glm::mat4 &viewMatrix)
{
    // Calculate the view rotation inverse to apply transformations in camera space
    glm::mat3 viewRotateInverse = glm::transpose(glm::mat3(viewMatrix));

    // Extract the rotation component from the cube's transformation matrix
    glm::mat3 rotation = glm::mat3(rot);
    glm::mat3 rot_inverse = glm::transpose(rotation);

    // Transform the vector into world space:
    // - First, transform into world space (camera's view inverse)
    // - Then, transform into local space (cube's rotation)
    trans_vector = rot_inverse * viewRotateInverse * trans_vector;

    // Create the translation matrix using the transformed vector
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), trans_vector);

    // Combine the new translation with the cube's current transformation
    this->trans = this->trans * translation; // Apply translation relative to current transformations
}

void Cube::rotateCubePickingMode(float radians, glm::vec3 axis)
{

    // Translate the cube back to the origin
    glm::mat4 translateToOrigin = glm::translate(glm::mat4(1.0f), -glm::vec3(trans[3]));
    //  Apply the rotation-pure rot
    glm::mat4 pure_rot = glm::rotate(glm::mat4(1.0f), radians, axis);

    // Translate the cube back to its original position
    glm::mat4 translateBack = glm::translate(glm::mat4(1.0f), glm::vec3(trans[3]));

    // Combine the transformations: T_back * R * T_origin
    glm::mat4 combinedTransformation = translateBack * pure_rot * translateToOrigin;

    // Apply the combined transformation to the rotation matrix
    rot = rot * combinedTransformation;
}
