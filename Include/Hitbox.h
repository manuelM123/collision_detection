#pragma once

#ifndef HITBOX_H
#define HITBOX_H

#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader_t.h"
#include "Camera.h"
#include "Textures.h"

#include <iostream>
#include <vector>

// Class that generates the hitbox
class Hitbox {
public: 
    Hitbox(std::vector<float> hitbox_coordinates);
    void setupHitbox(float* vertices, int* indices);
    void Draw();
    float* createVertices(std::vector<float> hitbox_coordinates);
    int* createIndices();
private:
    // VAO, VBO and EBO for the hitbox
    unsigned int hitboxVAO, hitboxVBO, hitboxEBO;
};

#endif