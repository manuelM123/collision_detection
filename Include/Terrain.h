#pragma once

#ifndef TERRAIN_H
#define TERRAIN_H

#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader_t.h"
#include "Camera.h"
#include "Textures.h"

#include <iostream>
#include <vector>

// Class that generates the terrain
class Terrain {
public: 
    Terrain(unsigned int NUM_PATCHS_PTS, unsigned NUM_PATCHES, int width, int height);
    void setupTerrain(unsigned int NUM_PATCHS_PTS);
    void Draw(Shader_t terrainShader, Camera cam, unsigned int NUM_PATCH_PTS, unsigned NUM_PATCHES);
    void createVertices(unsigned NUM_PATCHES, int width, int height);

    std::vector<float> vertices;

private:
    // VAO and VBO for the terrain
    unsigned int terrainVAO, terrainVBO;
};

#endif