//
// Created by jcollin2 on 9/29/22.
//

#include "PhysicsComponents.h"

namespace seng
{
//***********************************************************
//       RodCloth Methods
//***********************************************************

/////////////  RodCloth Ctor  ///////////////////
RodCloth::RodCloth(int numXNodes, int numYNodes, std::function<void(int, int, std::vector<Node>&)> initializer,
                   Material* material, float mass, unsigned int numIterations) :
        m_Nx(numXNodes), m_Ny(numYNodes), m_mass(mass), m_numConstraintIterations(numIterations), m_initializer(initializer),
        m_shuffleIndex{std::vector<int>(m_Nx*m_Ny)}, m_material{material}
{

}



void RodCloth::StartUp()
{
    // Shuffle the indices around
    for(int ii = 0; ii < m_shuffleIndex.size(); ++ii)
    {
        m_shuffleIndex[ii] = ii;
    }
    auto rd = std::random_device {};
    auto rng = std::default_random_engine { rd() };
    std::shuffle(m_shuffleIndex.begin(), m_shuffleIndex.end(), rng);

    // Initialize state of the cloth with given function pointer
    m_initializer(m_Nx, m_Ny, m_nodes);

    InitializeAdjNodes();

    // Create or clear Mesh component
    if(!parentObject->HasComponent<Mesh>())
    {
        parentObject->AddComponent(new Mesh());
    }
    Mesh& mesh = parentObject->GetComponent<Mesh>();
    mesh.m_meshes.clear();
    mesh.m_meshes.push_back(new MeshData());
    MeshData& meshData = *mesh.m_meshes[0];
    meshData.m_material = m_material;

    SetVertexData();


}

void RodCloth::Update(float deltaTime)
{
    Component::Update(deltaTime);
}



/////////////  Private Helper Methods for RodCloth  ///////////////////
void RodCloth::InitializeAdjNodes()
{
    int index{-1};
    // Inner nodes
    for(int ii = 1; ii < m_Nx-1; ++ii)
    {
        for(int jj = 1; jj < m_Ny-1; ++jj)
        {
            index = mat2VecIdx(ii, jj);

            m_nodes[index].adjIndices.push_back(mat2VecIdx(ii, jj - 1)); // S
            m_nodes[index].adjIndices.push_back(mat2VecIdx(ii + 1, jj)); // E
            m_nodes[index].adjIndices.push_back(mat2VecIdx(ii, jj + 1)); // N
            m_nodes[index].adjIndices.push_back(mat2VecIdx(ii - 1, jj)); // W
        }
    }

    // Inner top and bottom
    for(int ii = 1; ii < m_Nx-1; ++ii)
    {
        index = mat2VecIdx(ii, 0);
        m_nodes[index].adjIndices.push_back(mat2VecIdx(ii - 1, 0)); // W
        m_nodes[index].adjIndices.push_back(mat2VecIdx(ii + 1, 0)); // E
        m_nodes[index].adjIndices.push_back(mat2VecIdx(ii, 1)); // N

        index = mat2VecIdx(ii, m_Ny - 1);
        m_nodes[index].adjIndices.push_back(mat2VecIdx(ii - 1, m_Ny - 1)); // W
        m_nodes[index].adjIndices.push_back(mat2VecIdx(ii + 1, m_Ny - 1)); // E
        m_nodes[index].adjIndices.push_back(mat2VecIdx(ii, m_Ny - 2)); // S
    }

    // Inner left and right
    for(int jj = 1; jj < m_Ny-1; ++jj)
    {
        index = mat2VecIdx(0, jj);
        m_nodes[index].adjIndices.push_back(mat2VecIdx(0, jj + 1)); // N
        m_nodes[index].adjIndices.push_back(mat2VecIdx(0, jj - 1)); // S
        m_nodes[index].adjIndices.push_back(mat2VecIdx(1, jj)); // E

        index = mat2VecIdx(m_Nx - 1, jj);
        m_nodes[index].adjIndices.push_back(mat2VecIdx(m_Nx - 1, jj + 1)); // N
        m_nodes[index].adjIndices.push_back(mat2VecIdx(m_Nx - 1, jj - 1)); // S
        m_nodes[index].adjIndices.push_back(mat2VecIdx(m_Nx - 2, jj)); // W
    }

    // Corners
    index = mat2VecIdx(0, 0);
    m_nodes[index].adjIndices.push_back(mat2VecIdx(0, 1)); // N
    m_nodes[index].adjIndices.push_back(mat2VecIdx(1, 0)); // E

    index = mat2VecIdx(m_Nx - 1, 0);
    m_nodes[index].adjIndices.push_back(mat2VecIdx(m_Nx - 1, 1)); // N
    m_nodes[index].adjIndices.push_back(mat2VecIdx(m_Nx - 2, 0)); // W

    index = mat2VecIdx(0, m_Ny - 1);
    m_nodes[index].adjIndices.push_back(mat2VecIdx(0, m_Ny - 2)); // S
    m_nodes[index].adjIndices.push_back(mat2VecIdx(1, m_Ny - 1)); // E

    index = mat2VecIdx(m_Nx - 1, m_Ny - 1);
    m_nodes[index].adjIndices.push_back(mat2VecIdx(m_Nx - 1, m_Ny - 2)); // S
    m_nodes[index].adjIndices.push_back(mat2VecIdx(m_Nx - 2, m_Ny - 1)); // W

}


void RodCloth::SetVertexData()
{

}








} // seng