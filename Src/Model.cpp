#include "Model.h"

Model::Model()
{
}

Model::Model(string const& path, bool gamma) : gammaCorrection(gamma)
{
    loadModel(path);
}

/*
Model::Model(string const& path, std::vector<float> hitbox_coordinates, bool gamma) : gammaCorrection(gamma)
{
    loadModel(path, hitbox_coordinates);
}
*/

void Model::Draw(Shader& shader)
{
    for (GLuint i = 0; i < meshes.size(); i++)
        meshes[i].Draw(shader);
}

void Model::loadModel(string const& path)
{
    // read file via ASSIMP
    Assimp::Importer importer;
    scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
        return;
    }
    // retrieve the directory path of the filepath
    directory = path.substr(0, path.find_last_of('/'));
    
    // process ASSIMP's root node recursively
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    // process each mesh located at the current node
    for (GLuint i = 0; i < node->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene. 
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        //aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        mesh_to_use = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh_to_use, scene));
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for (GLuint i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }

}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    // data to fill
    vector<Vertex> vertices;
    vector<GLuint> indices;
    vector<TextureMesh> textures;

    // walk through each of the mesh's vertices
    for (GLuint i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        // normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }
        // texture coordinates
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
            // tangent
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;
            // bitangent
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.Bitangent = vector;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
        vertices_sat.push_back(mesh->mVertices[i]);
    }

    // now wak through each of the mesh's faces and retrieve the corresponding vertex indices.
    for (GLuint i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];

        // retrieve all indices of the face and store them in the indices vector
        for (GLuint j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    
    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    model_material = loadMaterial(material);

    // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
    // Same applies to other texture as the following list summarizes:
    // diffuse: texture_diffuseN
    // specular: texture_specularN
    // normal: texture_normalN

    // 1. diffuse maps
    vector<TextureMesh> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    vector<TextureMesh> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    std::vector<TextureMesh> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    std::vector<TextureMesh> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
    // 5. emissive maps
    std::vector<TextureMesh> emissiveMaps = loadMaterialTextures(material, aiTextureType_EMISSIVE, "texture_emissive");
    textures.insert(textures.end(), emissiveMaps.begin(), emissiveMaps.end());

    // determine hitbox coordinates of the model
    hitboxCoordinates(mesh);

    // get face normals
    getFaceNormals(mesh);

    // return a mesh object created from the extracted mesh data
    return Mesh(vertices, indices, textures);
}

vector<TextureMesh> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
    vector<TextureMesh> textures;

    // if the material has no diffuse textures, return default texture  ================== eu
    if (typeName == "texture_diffuse" && mat->GetTextureCount(type) == 0) {
        TextureMesh texture;
        texture.id = TextureFromFile(DEFAULT_TEXTURE_NAME, DEFAULT_TEXTURE_DIR);
        texture.type = typeName;
        texture.path = "default.png";
        textures.push_back(texture);
        return textures;
    }

    // Get the number of textures of the given type
    for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        for (GLuint j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }
        if (!skip)
        {   // if texture hasn't been loaded already, load it
            TextureMesh texture;
            texture.id = TextureFromFile(str.C_Str(), this->directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
        }
    }
    return textures;
}

Material Model::loadMaterial(aiMaterial* mat) {
    Material material;
    aiColor3D color(0.f, 0.f, 0.f);
    float shininess;

    mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    material.Diffuse = glm::vec3(color.r, color.b, color.g);

    mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
    material.Ambient = glm::vec3(color.r, color.b, color.g);

    mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
    material.Specular = glm::vec3(color.r, color.b, color.g);

    mat->Get(AI_MATKEY_SHININESS, shininess);
    material.Shininess = shininess;

    return material;
}

GLuint Model::TextureFromFile(const char* path, const string& directory, bool gamma)
{
    string filename = string(path);
    filename = directory + '/' + filename;

    GLuint textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;

    // flip the texture on the y axis
    //stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data); // <------------------ LINHA QUE D� BARRACADA
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "TextureMesh failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

void Model::changeTexture(const char* name, string dir) {
    // go through all meshes
    for (auto& mesh : meshes) {
        // go through all textures of each mesh
        for (auto& texture : mesh.textures) {
            // if the texture is a diffuse texture
            if (texture.type == "texture_diffuse") {
                // change the texture
                texture.id = TextureFromFile(name, dir);
                break;
            }
        }
    }
}

// calculate hitbox coordinates based on min and max coordinates of the mesh on x,y,z axis
void Model::hitboxCoordinates(aiMesh* mesh){
    aiVector3D min_pos, max_pos;
    min_pos = max_pos = mesh->mVertices[0];

    for(GLuint i = 0; i < mesh->mNumVertices; i++){
        // verifying the minimum and maximum values of the x axis
        if(mesh->mVertices[i].x < min_pos.x) min_pos.x = mesh->mVertices[i].x;
        if(mesh->mVertices[i].x > max_pos.x) max_pos.x = mesh->mVertices[i].x;

        // verifying the minimum and maximum values of the y axis
        if(mesh->mVertices[i].y < min_pos.y) min_pos.y = mesh->mVertices[i].y;
        if(mesh->mVertices[i].y > max_pos.y) max_pos.y = mesh->mVertices[i].y;

        // verifying the minimum and maximum values of the z axis
        if(mesh->mVertices[i].z < min_pos.z) min_pos.z = mesh->mVertices[i].z;
        if(mesh->mVertices[i].z > max_pos.z) max_pos.z = mesh->mVertices[i].z;
    }

    float offset = 0.2f;
    // vector has the following order: [min_x, max_x, min_y, max_y, min_z, max_z]
    // offset to make hitbox larger compared to the model
    hitbox_coordinates.push_back(min_pos.x - offset);
    hitbox_coordinates.push_back(max_pos.x + offset);
    hitbox_coordinates.push_back(min_pos.y - offset);
    hitbox_coordinates.push_back(max_pos.y + offset);
    hitbox_coordinates.push_back(min_pos.z - offset);
    hitbox_coordinates.push_back(max_pos.z + offset);
}

void Model::getFaceNormals(aiMesh* mesh){
    // Get face normals for the mesh
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        faces_sat.push_back(face);
        
        // vertices_sat => [mesh->mVertices[0], ...]
        aiVector3D vertex1 = mesh->mVertices[face.mIndices[0]];
        aiVector3D vertex2 = mesh->mVertices[face.mIndices[1]];
        aiVector3D vertex3 = mesh->mVertices[face.mIndices[2]];

        aiVector3D faceNormal =  (vertex2 - vertex1) ^ (vertex3 - vertex1);
        faceNormal.Normalize();
        faceNormals.push_back(faceNormal);
    }
}