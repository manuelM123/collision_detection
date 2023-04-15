#pragma once
#ifndef COLLISION_SCENE_H
#define COLLISION_SCENE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Configuration.h"
#include "Shader.h"
#include "Shader_t.h"
#include "Camera.h"
#include "Utils.h"
#include "Scene.h"
#include "Textures.h"
#include "Model.h"
#include "Skybox.h"
#include "Framebuffer.h"
#include "Hitbox.h"
#include "Textures.h"

#include <iostream>
#include <thread>

// Variables that control movement of objects
extern float xstep;
extern float ystep;
extern float zstep;

class Collisions_Detection
{
public:
    GLFWwindow* window;

    // Camera
    Camera camera;
    CameraPos cameraPos;

    // Shaders
    //Shader_t* terrainShader;

    // Textures
    Texture* default_texture;

    // Hitbox
    Hitbox* hitbox;
    Hitbox* hitbox2;

    Collisions_Detection();
    void setupScene();
    void renderScene();
private:
    // Camera variables
    glm::vec3 cameraInitialPos;
    glm::vec3 cameraInitialTarget;

    // File paths
    const char* cubePath = "Media/Objects/Cube/cube.obj";
    const char* skyboxPath = "Media/Skyboxes/skybox_sky/";

    // Shaders
    Shader* objectShader;
    Shader* skyboxShader;
    Shader* screenShader;
    Shader* screenShader_noFilter;

    // Models
    Model* cube;
    Model* cube2;
    Skybox* skybox;

    // Materials
    Material default_mat;

    // Framebuffers
    Framebuffer* framebuffer;

    // Setup Methods
    void setupWindow();
    void loadModels();
    void loadShaders();
    void loadFramebuffers();
    void setupCamera();
    void setupLightingAndMaterials();
    void setupObjectProperties(Shader* objectShader, Camera camera, int objectID);
};

#endif