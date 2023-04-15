// Include Scenes
#include <Scenes/Collisions_Detection.hpp>

// Include Callbacks
#include <Callbacks/Collisions_Callback.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>

int main()
{
    // Initialize class object 
    Collisions_Detection scene;

    // Function that handles all the callbacks from the application
    processCallbacks(scene.window, &(scene.camera), &(scene.cameraPos));

    // Running the application
    while (!glfwWindowShouldClose(scene.window))
    {
        // Function that deals with the input from the user
        processInput(scene.window, &(scene.camera), scene.terrainShader, scene.default_texture, scene.terrain_element);

        // Rendering the scene
        scene.renderScene();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}