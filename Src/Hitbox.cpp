#include "Hitbox.h"

// Constructor for the hitbox class
Hitbox::Hitbox(std::vector<float> hitbox_coordinates){
    float* vertices = createVertices(hitbox_coordinates);
    int* indices = createIndices();
    std::cout << "Hitbox Coordinates" << std::endl;
    
    // print vertices
    for(int i = 0; i < 24; i++){
        std::cout << vertices[i] << "|";
    }

    std::cout << std::endl;

    setupHitbox(vertices, indices);
}

// prepare buffers for the hitbox
void Hitbox::setupHitbox(float* vertices, int* indices){
    // Setup VAO for the hitbox
    glGenVertexArrays(1, &hitboxVAO);
    glBindVertexArray(hitboxVAO);

    // Setup VBO for the hitbox
    glGenBuffers(1, &hitboxVBO);
    glBindBuffer(GL_ARRAY_BUFFER, hitboxVBO);
    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertices, GL_STATIC_DRAW);

    // Setup EBO for the hitbox
    glGenBuffers(1, &hitboxEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, hitboxEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 24 * sizeof(float), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// draw the hitbox according a number of patches and patch points
void Hitbox::Draw(){
    // render the hitbox
    glBindVertexArray(hitboxVAO);
    glLineWidth(4.0f); // line width of 2 pixels
    glColor3f(1.0f, 0.0f, 0.0f); // red color
    glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
}

// generate the vertices for the hitbox based on the coordinates provided
// vector has the following order: [min_x, max_x, min_y, max_y, min_z, max_z]
float* Hitbox::createVertices(std::vector<float> hitbox_coordinates){
    float offset = 0.2f;

    static float vertices[] = {
        // Front face
        hitbox_coordinates[0] - offset, hitbox_coordinates[2] - offset, hitbox_coordinates[4] - offset, // Bottom left | 0
        hitbox_coordinates[1] + offset, hitbox_coordinates[2] - offset, hitbox_coordinates[4] - offset, // Bottom right | 1
        hitbox_coordinates[0] - offset, hitbox_coordinates[3] + offset, hitbox_coordinates[4] - offset, // Top left | 2
        hitbox_coordinates[1] + offset, hitbox_coordinates[3] + offset, hitbox_coordinates[4] - offset, // Top right | 3

        // Back face
        hitbox_coordinates[0] - offset, hitbox_coordinates[2] - offset, hitbox_coordinates[5] + offset, // Bottom left | 4
        hitbox_coordinates[1] + offset, hitbox_coordinates[2] - offset, hitbox_coordinates[5] + offset, // Bottom right | 5
        hitbox_coordinates[0] - offset, hitbox_coordinates[3] + offset, hitbox_coordinates[5] + offset, // Top left | 6
        hitbox_coordinates[1] + offset, hitbox_coordinates[3] + offset, hitbox_coordinates[5] + offset, // Top right | 7
    };

    return vertices;
}

// specify the indices to connect the points
/*
    Front face
    2 - 3
    |   |
    0 - 1

    Back face
    6 - 7
    |   |
    4 - 5 
*/
int* Hitbox::createIndices(){
    static int indices[] = {
        // Front
        0, 1, 1, 3, 3, 2, 2, 0,

        // Back
        4, 5, 5, 7, 7, 6, 6, 4,

        // Connecting Lines
        0, 4, 1, 5, 2, 6, 3, 7
    };

    return indices;
}