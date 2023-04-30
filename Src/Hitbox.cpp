#include "Hitbox.h"

// Constructor for the hitbox class
Hitbox::Hitbox(std::vector<float> hitbox_coordinates){
    float* vertices = createVertices(hitbox_coordinates);
    int* indices = createIndices();
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
    glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
}

// generate the vertices for the hitbox based on the coordinates provided
// vector has the following order: [min_x, max_x, min_y, max_y, min_z, max_z]
float* Hitbox::createVertices(std::vector<float> hitbox_coordinates){
    float offset = 0.2f;

    float* vertices = new float[24];

    // Front face
    vertices[0] = hitbox_coordinates[0]; vertices[1] = hitbox_coordinates[2]; vertices[2] = hitbox_coordinates[4];
    vertices[3] = hitbox_coordinates[1]; vertices[4] = hitbox_coordinates[2]; vertices[5] = hitbox_coordinates[4];
    vertices[6] = hitbox_coordinates[0]; vertices[7] = hitbox_coordinates[3]; vertices[8] = hitbox_coordinates[4];
    vertices[9] = hitbox_coordinates[1]; vertices[10] = hitbox_coordinates[3]; vertices[11] = hitbox_coordinates[4];

    // Back face
    vertices[12] = hitbox_coordinates[0]; vertices[13] = hitbox_coordinates[2]; vertices[14] = hitbox_coordinates[5];
    vertices[15] = hitbox_coordinates[1]; vertices[16] = hitbox_coordinates[2]; vertices[17] = hitbox_coordinates[5];
    vertices[18] = hitbox_coordinates[0]; vertices[19] = hitbox_coordinates[3]; vertices[20] = hitbox_coordinates[5];
    vertices[21] = hitbox_coordinates[1]; vertices[22] = hitbox_coordinates[3]; vertices[23] = hitbox_coordinates[5];

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