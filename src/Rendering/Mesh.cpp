//
// Created by jcollin2 on 9/19/22.
//


#include "Rendering/Mesh.h"
#include "Rendering/Material.h"


namespace seng
{


VAO* VAOManager::CreateVAO(std::string vaoKey, std::vector<Vert3x3x2f> vertList, std::vector<unsigned int> elementList)
{
    assert(m_vaoList.find(vaoKey) == m_vaoList.end() && "Key already exists in VAOManager!!!");

    //TODO: Add drawstyle option to the constructor
    m_vaoList[vaoKey] = std::unique_ptr<VAO>{new VAO(vertList, elementList)};
    printVAOList();
    return m_vaoList[vaoKey].get();
}

VAO* VAOManager::GetVAO(std::string vaoKey)
{
    try {
        return m_vaoList.at(vaoKey).get();
    }
    catch(std::out_of_range e)
    {
        return nullptr;
    }
}

void VAOManager::DestroyVAO(std::string vaoKey)
{
    try {
        m_vaoList.at(vaoKey).reset();
        printVAOList();
    }
    catch (std::out_of_range e)
    {
        assert("Trying to destroy vao from Manager with incorrect key");
    }
}


void VAOManager::printVAOList()
{
    std::string spacer = "---------------------------------------------------------";
    std::cout << "============== VAOManager List ==========================\n";
    for(auto iter = m_vaoList.begin(); iter != m_vaoList.end(); iter++)
    {
        std::cout << "Key = " << iter->first << "\n";
        iter->second->PrintData();
        std::cout << spacer << "\n";
    }
    std::cout << "============== End VAOManager List ======================\n\n";
}


//***********************************************************
//       Mesh Methods
//***********************************************************
void Mesh::Render(Shader& shader)
{
    for(const MeshData& meshdata : m_meshes)
    {
        VAO* thisVAO = meshdata.m_vao;
        meshdata.m_material.SetupMaterial(shader);
        glBindVertexArray(thisVAO->m_vaoID);
        if(thisVAO->m_eboID != 0)
        {
            glDrawElements(GL_TRIANGLES, thisVAO->m_numElements, GL_UNSIGNED_INT, 0);
        }
        else
        {
            glDrawArrays(GL_TRIANGLES, 0, thisVAO->m_numVerts);
        }
        meshdata.m_material.ResetMaterial(shader);
    }

}




}