//
// Created by jcollin2 on 7/18/22.
//

#ifndef SIM_ENGINE_ASSETS_H
#define SIM_ENGINE_ASSETS_H

#include "Misc/MeshCommon.h"
#include "Rendering/Shader.h"
#include "Engine/Component.h"
#include "assimp/scene.h"

struct aiMesh;

namespace seng
{

struct Texture
{
    unsigned int m_tbo{0};
    std::string m_fileName{""};
    unsigned int m_texUnit{0};
};






//***********************************************************
//       Material Assets
//***********************************************************


class Material
{

public:
    ShaderType m_shaderType;
    Shader* m_shader{nullptr};

    virtual ~Material() = default;

    virtual void SetupMaterial() = 0;
    virtual void ResetMaterial(){};

    static std::unique_ptr<Material> m_defaultMaterial;
};




class SolidMaterial : public Material
{
public:
    // Color information
    glm::vec3 m_diffuse{1.f, 0.f, .3f};
    glm::vec3 m_specular{1.f, 1.f, 1.f};
    unsigned int m_roughness{1};


    /***************** SolidMaterial ctor  ******************
     * @brief Set the color properties.
    ******************************************************************///
    SolidMaterial()
    {
        m_shaderType = ShaderType::VPASS_FLIT;
    }

    SolidMaterial(const glm::vec3& diffuse, const glm::vec3& specular, unsigned int roughness) :
            m_diffuse(diffuse), m_specular(specular), m_roughness(roughness)
            {
                m_shaderType = ShaderType::VPASS_FLIT;
            };


    /***************** SetupMaterial  ******************
     * @brief Set the uniforms for the color in the passed shader.
     *
     *     This assumed the shader program is already in use.
     *
     * @param shader Shader that holds the uniforms for color.
    ******************************************************************///
    void SetupMaterial() override;


    /***************** ResetMaterial  ******************
     * @brief Reset flags for solid material.
     *
     * @param shader Shader objects containing flag uniforms.
    ******************************************************************///
    void ResetMaterial() override;


};







class TextureMaterial : public Material
{
public:
    Texture m_diffTexture{};
    glm::vec3 m_specColor{1.f};
    float m_roughness{1.f};


    TextureMaterial()
    {
        m_shaderType = ShaderType::VPASS_FLIT;
    }
    TextureMaterial(const std::string& path, unsigned int texUnit, const glm::vec3& specColor = glm::vec3(1.f),
                    float roughness = 1.f);


    void SetupMaterial() override;


    void ResetMaterial() override;
};








//***********************************************************
//       Model Assets
//***********************************************************


/***************** ModelAsset Class  ******************
 * @brief Object to hold an imported model asset (.obj only for now).  The model version of a texture material.
 *      It will contain all the information necessary to create a Model component:
 *      - vertex information (possible multiple arrays)
 *      - element information (possible multiple arrays)
 *      - pointer to all materials (one for each array of vertex info)
 *
 *      ModelAsset will be owner of the material(s) for the meshes.  **All material must use
 *      the same ShaderType.**
 *
 *      All ModelAssets will be held in a hash table in the scene same as materials.
 *
 *
 * @param filename Name of the .obj file storing the model
******************************************************************///
class ModelAsset
{
public:
    std::vector< std::vector<Vert3x3x2f> > m_vertMeshData;          // Vertex data for each mesh
    std::vector< std::vector<unsigned int> > m_elementMeshData;     // Element data for each mesh
    std::vector< int > m_materialMeshData;    // Index into



    /***************** ModelAsset Ctor  ******************
     * @brief Everything is done in ctor.  Using Assimp ctor will:
     *
     *      - import all vertex info for each mesh
     *      - import all element info for each mesh
     *      - Generate the material for each mesh
     *
     * @returns
    ******************************************************************///
    ModelAsset(const std::string& filename);

    ~ModelAsset()
    {
        int x = 0;
    }


    /***************** GetAllMaterials  ******************
     * @brief Returns a vector containing all the materials used for this model.
    ******************************************************************///
    std::vector<Material*> GetAllMaterials();

private:
    std::string m_directory{};
    std::vector<std::unique_ptr<Material>> m_loadedMaterials{};  // Stores all the materials used by the mesh
    std::unordered_map<std::string, int> m_loadedTexPaths{};     // Key is texture filename, value is index into loadedMaterials for that texture
    /***************** LoadMesh  ******************
     * @brief Loads one particular mesh from the .obj file into an array of vertex info and element info.
     *      Also generates the material for this mesh.  Adds all this info to the members in the ModelAsset.
    ******************************************************************///
    void LoadMesh(aiMesh* mesh, const aiScene* scene);

    /***************** LoadMaterial  ******************
     * @brief Loads the material for a particular mesh in the assimp heirarchy.  If texture has not already been loaded
     *      a TextureMaterial is created and added to m_loadedMaterials.  SolidMaterial is always added new, even if it is the same
     *      as a texture already created.
     *
     *      Return index to the material in m_loadedMaterials
     *
     *      It is assumed that there is at most 1 texture per mesh.  Only the diffuse texture is checked to determine
     *      if there is a texture associated with the mesh.
     *
     *      If no texture is found, a SolidMaterial is created and the diffuse, specular, and shininess properties
     *      are obtained from the .obj file.
     *
     * @return Index into m_loadedMaterials for the material for this mesh.
    ******************************************************************///
    int LoadMaterial(const aiMaterial* meshMaterial);
};




} //end namespace seng


#endif //SIM_ENGINE_ASSETS_H
