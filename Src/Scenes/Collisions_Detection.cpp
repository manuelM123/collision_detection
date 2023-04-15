#include <Scenes/Collisions_Detection.hpp>
#include <Terrain.h>
#include <Textures.h>

// Variables that control the dimension of the terrain
float xstep = 2.0f;
float ystep = 1.0f;
float zstep = 0.0f;
int stop = 0;

// Constructor
Collisions_Detection::Collisions_Detection()
{
    setupScene();
}

// Function that setups the terrain scene
void Collisions_Detection::setupScene()
{
    // Window initialization
    setupWindow();

    // Extra variables initialization
    cameraInitialPos = glm::vec3(-1.99221f, 1.42674f, 5.2215f);
	cameraInitialTarget = glm::vec3(7.0f, 2.0f, 0.0f);
    
    loadModels();
    loadShaders();
    //loadFramebuffers();
    setupLightingAndMaterials();
    setupCamera();
}

void Collisions_Detection::setupObjectProperties(Shader* objectShader, Camera camera, int objectID)
{
    float x = xstep;
    float y = ystep;
    float z = zstep;

    // static object
    if(objectID == 0)
    {
       x = 0.0f;
       y = 0.0f;
       z = 0.0f;
    }

    if(stop != 2)
    {
        std::cout << "ID: " << objectID << std::endl;
        std::cout << "x: " << x << " y: " << y << " z: " << z << std::endl;
        std::cout << "xstep: " << xstep << " ystep: " << ystep << " zstep: " << zstep << std::endl;
        std::cout << "-------------------------------------------------" << std::endl;
        stop = stop + 1;
    }

    objectShader->use();
    setView(objectShader, camera.GetViewMatrix());
    setProjection(objectShader, glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    setModel(
        objectShader, // shader
        glm::vec3(x,y,z), // translation
        glm::vec3(0.0f, 1.0f, 0.0f), // rotation axis
        0.0f,//(float)glfwGetTime() * 2.5f, // rotation angle
        glm::vec3(1.0f) // scale
        );
}

// Function that renders the scene
void Collisions_Detection::renderScene()
{
    // Update camera delta time
    camera.updateDeltaTime();

    // Bind to framebuffer
    //framebuffer->bind();

    // Clear the screen
    glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw objects
    // --------------------------------------
    setupObjectProperties(objectShader, camera, 0);
    cube->Draw(*objectShader);

    setupObjectProperties(objectShader, camera, 1);
    cube2->Draw(*objectShader);

    objectShader->setVec3("material.ambient", default_mat.Ambient);
    objectShader->setVec3("material.diffuse", default_mat.Diffuse);
    objectShader->setVec3("material.specular", default_mat.Specular);
    objectShader->setFloat("material.shininess", default_mat.Shininess);

    // Skybox 
    skybox->Draw(*skyboxShader, camera);

    // Bind to framebuffer
    //framebuffer->unbind();

    //framebuffer->drawQuad(*screenShader_noFilter);

    // Swap the buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
}

// Function that loads the necessary models for the scene
void Collisions_Detection::loadModels()
{
    cube = new Model(cubePath);
    cube->changeTexture("wood_floor.png", "Media/Textures");

    cube2 = new Model(cubePath);
    cube2->changeTexture("random.jpg", "Media/Textures");

    skybox = new Skybox(skyboxPath);
}

// Function that loads the necessary shaders for the scene
void Collisions_Detection::loadShaders()
{
    objectShader = new Shader("Shaders/targetShader.vert", "Shaders/targetShader.frag");
    skyboxShader = new Shader("Shaders/skyboxShader.vert", "Shaders/skyboxShader.frag");
    screenShader_noFilter = new Shader("Shaders/Screen/simpleScreen.vert", "Shaders/Screen/simpleScreen.frag");
}

// Function that loads the necessary framebuffers for the scene
void Collisions_Detection::loadFramebuffers()
{
    framebuffer = new Framebuffer();
}

void Collisions_Detection::setupLightingAndMaterials()
{
    // Target shader
    objectShader->use();

    //Lighting
    objectShader->setVec3("objectColor", 1.0f, 1.0f, 1.0f);
    objectShader->setVec3("light.position", 1.0f, 5.0f, 1.0f);
    objectShader->setVec3("light.ambient", 0.6f, 0.6f, 0.6f);
    objectShader->setVec3("light.diffuse", 1.0f, 1.0f, 1.0f);
    objectShader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);

    // Materials
    default_mat.Ambient   = glm::vec3(1.0f);
    default_mat.Diffuse   = glm::vec3(0.6f);
    default_mat.Specular  = glm::vec3(0.6f);
    default_mat.Shininess = 0.6f;
}

// Function that setups the camera
void Collisions_Detection::setupCamera()
{
    camera = Camera(
        cameraInitialPos,
		cameraInitialTarget 
    );
}

// Function that setups the window
void Collisions_Detection::setupWindow()
{
    glfw_setup();
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Collision Detection", NULL, NULL);
    window_setup(window);
    glad_setup();
	printGPUinfo();
}