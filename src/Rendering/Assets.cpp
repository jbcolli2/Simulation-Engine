//
// Created by jcollin2 on 7/29/22.
//

#include "Assets.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

namespace seng
{


//***********************************************************
//       Material Methods
//***********************************************************
std::unique_ptr<Material> Material::m_defaultMaterial{std::make_unique<SolidMaterial>(
        glm::vec3{.7f, 0.f, .4f}, glm::vec3{1.f}, 16
        )};



//***********************************************************
//       SolidMaterial Methods
//***********************************************************



void SolidMaterial::SetupMaterial()
{
    m_shader->m_solidMatComp.SetUniform(*this);
}


void SolidMaterial::ResetMaterial()
{
    m_shader->m_solidMatComp.ResetUniform();
}


//***********************************************************
//       Texture Material
//***********************************************************
TextureMaterial::TextureMaterial(const std::string& path, unsigned int texUnit, const glm::vec3& specColor,
                                 float roughness) :
    m_specColor(specColor), m_roughness(roughness)
{
    m_shaderType = ShaderType::VPASS_FLIT;

    m_diffTexture.m_fileName = path;
    m_diffTexture.m_texUnit = texUnit;
    m_diffTexture.m_tbo = GenAndLoadTBO(path);

}

void TextureMaterial::SetupMaterial()
{
    glActiveTexture(GL_TEXTURE0 + m_diffTexture.m_texUnit);
    glBindTexture(GL_TEXTURE_2D, m_diffTexture.m_tbo);
    m_shader->m_textureMatComp.SetUniform(*this);
}


void TextureMaterial::ResetMaterial()
{
    m_shader->m_textureMatComp.ResetUniform();
    glBindTexture(GL_TEXTURE_2D, 0);
}






//***********************************************************
//       ModelAsset Methods
//***********************************************************



ModelAsset::ModelAsset(const std::string& filepath)
{
    m_directory = filepath.substr(0, filepath.find_last_of('/'));
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs);

    // Check for loading errors
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
       !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }


    for(int meshID = 0; meshID < scene->mNumMeshes; ++meshID)
    {
        LoadMesh(scene->mMeshes[meshID], scene);
    }
}


void ModelAsset::LoadMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vert3x3x2f> vertexData(mesh->mNumVertices);
    std::vector<unsigned int> elementData(mesh->mNumFaces*3);
    int materialIdx;

    // Load vertex data
    for(int ii = 0; ii < mesh->mNumVertices; ++ii)
    {
        vertexData[ii] = Vert3x3x2f(mesh->mVertices[ii].x, mesh->mVertices[ii].y, mesh->mVertices[ii].z,
                                        mesh->mNormals[ii].x, mesh->mNormals[ii].y, mesh->mNormals[ii].z,
                                        mesh->mTextureCoords[0][ii].x, mesh->mTextureCoords[0][ii].y);
    }
    m_vertMeshData.push_back(vertexData);

    // Load index data
    int elementIdx = 0;
    for(int ii = 0; ii < mesh->mNumFaces; ++ii)
    {
        aiFace face = mesh->mFaces[ii];
        for(int jj = 0; jj < face.mNumIndices; ++jj)
        {
            elementData[elementIdx+jj] = face.mIndices[jj];
        }
        elementIdx += face.mNumIndices;
    }
    m_elementMeshData.push_back(elementData);

    // Generate material
    const aiMaterial* meshMaterial = scene->mMaterials[mesh->mMaterialIndex];
    materialIdx = LoadMaterial(meshMaterial);
    m_materialMeshData.push_back(materialIdx);
}


int ModelAsset::LoadMaterial(const aiMaterial* meshMaterial)
{
    int materialIdx;

    // Yes there is a texture
    if(meshMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
    {
         aiString texturePath;
         aiReturn ret = meshMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath);
         assert(ret == AI_SUCCESS && "Assimp Error: Error loading texture");

         std::string strTexturePath = std::string(texturePath.C_Str());
         // If this texture file has already been loaded
         try
         {
             int matIdx = m_loadedTexPaths.at(strTexturePath);
             return matIdx;
         }
         catch(std::out_of_range e)
         {
             aiColor3D aiSpecularColor;
             meshMaterial->Get(AI_MATKEY_COLOR_SPECULAR, aiSpecularColor);
             glm::vec3 specularColor{aiSpecularColor.r, aiSpecularColor.g, aiSpecularColor.b};
             float roughness{1.f};
             meshMaterial->Get(AI_MATKEY_SHININESS, roughness);

             std::string fullpath = m_directory + "/" + strTexturePath;
             m_loadedMaterials.push_back(std::unique_ptr<Material>(new TextureMaterial(fullpath, 0, specularColor, roughness)));
             m_loadedTexPaths[strTexturePath] = m_loadedMaterials.size() - 1;
             return m_loadedMaterials.size() - 1;  // Index of newly added texture
         }
    }
    // No, there is no texture
    else
    {
        aiColor3D color;
        meshMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
        glm::vec3 diffuseColor{color.r, color.g, color.b};
        meshMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
        glm::vec3 specularColor{color.r, color.g, color.b};
        float roughness{1.f};
        meshMaterial->Get(AI_MATKEY_SHININESS, roughness);

        m_loadedMaterials.push_back(std::unique_ptr<Material>(new SolidMaterial(diffuseColor, specularColor, roughness)));
        return m_loadedMaterials.size() - 1;  // Index of newly added texture
    }
}


std::vector<Material*> ModelAsset::GetAllMaterials() const
{
    std::vector<Material*> materialList(m_loadedMaterials.size());

    for(int ii = 0; ii < m_loadedMaterials.size(); ++ii)
    {
        if(m_loadedMaterials[ii].get() != nullptr)
            materialList[ii] = m_loadedMaterials[ii].get();
    }

    return materialList;
}

} // end namespace seng