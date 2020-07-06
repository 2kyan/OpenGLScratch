#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <unordered_map>
#include "mesh.h"
class Shader;

class Model
{
public:
    Model(std::string model_name, std::string path, bool gamma = false);
    void Draw(glm::mat4 model, glm::mat4 view, glm::mat4 proj);

    std::string name()
    {
        return m_objname;
    };

    bool check(std::string attrib);
private:
    std::string m_objname;
    std::string m_path;

    std::shared_ptr<Shader> m_shader;
    std::vector<Texture_t> m_textures;
    std::vector<Mesh> m_meshes;
    std::string directory;
    bool gammaCorrection;
    /*  Functions   */
    void load(std::string model_name);
    void loadShader(std::string path);
    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture_t> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

};

