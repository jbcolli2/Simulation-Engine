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

    // Checkfor loading errors
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
    Material* material;

    // Load vertex data
    for(int ii = 0; ii < mesh->mNumVertices; ++ii)
    {
        vertexData.push_back(Vert3x3x2f(mesh->mVertices[ii].x, mesh->mVertices[ii].y, mesh->mVertices[ii].z,
                                        mesh->mNormals[ii].x, mesh->mNormals[ii].y, mesh->mNormals[ii].z,
                                        mesh->mTextureCoords[0][ii].x, mesh->mTextureCoords[0][ii].y));
    }
    m_vertMeshData.push_back(vertexData);

    // Load index data
    for(int ii = 0; ii < mesh->mNumFaces; ++ii)
    {
        aiFace face = mesh->mFaces[ii];
        for(int jj = 0; jj < face.mNumIndices; ++jj)
        {
            elementData.push_back(face.mIndices[jj]);
        }
    }
    m_elementMeshData.push_back(elementData);

    // Generate material
    const aiMaterial* meshMaterial = scene->mMaterials[mesh->mMaterialIndex];
    material = LoadMaterial(meshMaterial);
    m_materialMeshData.push_back(std::unique_ptr<Material>(material));
}


Material* ModelAsset::LoadMaterial(const aiMaterial* meshMaterial)
{
    Material* material;

    // Yes there is a texture
    if(meshMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
    {
         aiString* texturePath;
         aiReturn ret = meshMaterial->GetTexture(aiTextureType_DIFFUSE, 0, texturePath);
         assert(ret == AI_SUCCESS && "Assimp Error: Error loading texture");

         aiColor3D aiSpecularColor;
         meshMaterial->Get(AI_MATKEY_COLOR_SPECULAR, aiSpecularColor);
         glm::vec3 specularColor{aiSpecularColor.r, aiSpecularColor.g, aiSpecularColor.b};
         float roughness{1.f};
         meshMaterial->Get(AI_MATKEY_SHININESS, roughness);

         std::string fullpath = m_directory + "/" + std::string(texturePath->C_Str());
         material = new TextureMaterial(m_directory + "/" + std::string(texturePath->C_Str()), 0, specularColor, roughness);

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

        material = new SolidMaterial(diffuseColor, specularColor, roughness);
    }

    return material;
}


std::vector<Material*> ModelAsset::GetAllMaterials()
{
    std::vector<Material*> materialList(m_materialMeshData.size());

    for(int ii = 0; ii < m_materialMeshData.size(); ++ii)
    {
        if(m_materialMeshData[ii].get() != nullptr)
            materialList[ii] = m_materialMeshData[ii].get();
    }

    return materialList;
}

} // end namespace seng