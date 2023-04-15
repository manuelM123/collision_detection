#include "Textures.h"

// Function that loads the heightmap 
void loadTexture(Texture* tex, Shader_t terrainShader) {
    glGenTextures(1, &tex->texture_ID); // How many textures and where to store them
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex->texture_ID);

    // Wrapping parameters Tex1
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Filtering parameters Tex1
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load and generate the texture
    unsigned char* data = stbi_load(tex->texturePath, &tex->width_image, &tex->height_image, &tex->nrChannels_image, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->width_image, tex->height_image, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        terrainShader.setInt("heightMap", 0);
        std::cout << "Height map loaded with height " << tex->height_image << " and width " << tex->width_image << std::endl;
    }
    else
    {
        std::cout << "Failed to load texture " << std::endl;
    }

    stbi_image_free(data);
}