#pragma once

#ifndef CALLBACKS_COLLISION_H
#define CALLBACKS_COLLISION_H

#include <Camera.h>
#include <Utils.h>
#include <Skybox.h>
#include <Textures.h>
#include <Scenes/Collisions_Detection.hpp>
#include <GLFW/glfw3.h>
#include <iostream>

Camera* cam;
CameraPos* cam_pos;
bool wireframe = false;

// Function processes callbacks for mouse scroll
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    cam->ProcessMouseScroll(static_cast<float>(yoffset));
}

// Function processes callbacks for mouse movement
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (cam_pos->firstMouse)
    {
        cam_pos->lastX = xpos;
        cam_pos->lastY = ypos;
        cam_pos->firstMouse = false;
    }

    float xoffset = xpos - cam_pos->lastX;
    float yoffset = cam_pos->lastY - ypos; // reversed since y-coordinates go from bottom to top

    cam_pos->lastX = xpos;
    cam_pos->lastY = ypos;

    cam->ProcessMouseMovement(xoffset, yoffset);
}

// This callback function can't be used for things like movement because it doesn't work while pressing
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Leave the program
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

        // Keybinds
    if (key == GLFW_KEY_P && action == GLFW_PRESS)
    {
        wireframe = !wireframe;

        if (wireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow* window, Camera* camera)
{
    // Exit program
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Camera movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->ProcessKeyboard(FORWARD, camera->deltaTime);

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->ProcessKeyboard(BACKWARD, camera->deltaTime);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->ProcessKeyboard(LEFT, camera->deltaTime);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->ProcessKeyboard(RIGHT, camera->deltaTime);

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera->ProcessKeyboard(UP, camera->deltaTime);

    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        camera->ProcessKeyboard(DOWN, camera->deltaTime);

    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        ystep += 0.001f;

    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        ystep -= 0.001f;

    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        xstep += 0.001f;

    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        xstep -= 0.001f;
}

// Function that handles all the callbacks from the application
void processCallbacks(GLFWwindow* window, Camera* camera, CameraPos* cameraPos){
    cam = camera;
    cam_pos = cameraPos;

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);
}

#endif