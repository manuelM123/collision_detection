#include "Terrain.h"

// Constructor for the terrain class
Terrain::Terrain(unsigned int NUM_PATCHS_PTS, unsigned NUM_PATCHES, int width, int height){
    createVertices(NUM_PATCHES, width, height);
    setupTerrain(NUM_PATCHS_PTS);
}

// prepare buffers for the terrain
void Terrain::setupTerrain(unsigned int NUM_PATCHS_PTS){
    // Setup VAO for the terrain
    glGenVertexArrays(1, &terrainVAO);
    glBindVertexArray(terrainVAO);

    // Setup VBO for the terrain
    glGenBuffers(1, &terrainVBO);
    glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // texCoord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    // for tessellation, "GL_PATCH" is needed
    // PATCH => Set of N vertices that are interpolated between
    // below function specifies number of vertices per PATCH
    glPatchParameteri(GL_PATCH_VERTICES, NUM_PATCHS_PTS);
}

// draw the terrain according a number of patches and patch points
void Terrain::Draw(Shader_t terrainShader, Camera cam, unsigned int NUM_PATCHS_PTS, unsigned NUM_PATCHES){
    // activate specific shader for terrain
    terrainShader.use();

    // view/projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(cam.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100000.0f);
    glm::mat4 view = cam.GetViewMatrix();
    terrainShader.setMat4("projection", projection);
    terrainShader.setMat4("view", view);

    // world transformation
    glm::mat4 model = glm::mat4(1.0f);
    terrainShader.setMat4("model", model);

    // render the terrain
    glBindVertexArray(terrainVAO);
    glDrawArrays(GL_PATCHES, 0, NUM_PATCHS_PTS*NUM_PATCHES*NUM_PATCHES); 
}

// generate the vertices for the terrain based on heightmap
void Terrain::createVertices(unsigned NUM_PATCHES, int width, int height){
    // Loops that generate the vertices for the terrain based on the heightmap dimensions
    for(unsigned i = 0; i <= NUM_PATCHES-1; i++)
    {
    	for(unsigned j = 0; j <= NUM_PATCHES-1; j++)
    	{
            // position coordinates 1st vertice of the patch
    		vertices.push_back(-width/2.0f + width*i/(float)NUM_PATCHES); // v.x
    		vertices.push_back(0.0f); // v.y
    		vertices.push_back(-height/2.0f + height*j/(float)NUM_PATCHES); // v.z

            // texture coordinates 1s vertice of the patch
    		vertices.push_back(i / (float)NUM_PATCHES); // u
    		vertices.push_back(j / (float)NUM_PATCHES); // v

            // position coordinates 2nd vertice of the patch
    		vertices.push_back(-width/2.0f + width*(i+1)/(float)NUM_PATCHES); // v.x
    		vertices.push_back(0.0f); // v.y
    		vertices.push_back(-height/2.0f + height*j/(float)NUM_PATCHES); // v.z

            // texture coordinates 2nd vertice of the patch
    		vertices.push_back((i+1) / (float)NUM_PATCHES); // u
    		vertices.push_back(j / (float)NUM_PATCHES); // v

            // position coordinates 3rd vertice of the patch
    		vertices.push_back(-width/2.0f + width*i/(float)NUM_PATCHES); // v.x
    		vertices.push_back(0.0f); // v.y
    		vertices.push_back(-height/2.0f + height*(j+1)/(float)NUM_PATCHES); // v.z

            // texture coordinates 3rd vertice of the patch
    		vertices.push_back(i / (float)NUM_PATCHES); // u
    		vertices.push_back((j+1) / (float)NUM_PATCHES); // v

            // position coordinates 4th vertice of the patch
    		vertices.push_back(-width/2.0f + width*(i+1)/(float)NUM_PATCHES); // v.x
    		vertices.push_back(0.0f); // v.y
    		vertices.push_back(-height/2.0f + height*(j+1)/(float)NUM_PATCHES); // v.z

            // texture coordinates 4th vertice of the patch
    		vertices.push_back((i+1) / (float)NUM_PATCHES); // u
    		vertices.push_back((j+1) / (float)NUM_PATCHES); // v
    	}
    }
}
