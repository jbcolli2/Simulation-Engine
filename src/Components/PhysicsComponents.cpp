//
// Created by jcollin2 on 9/29/22.
//

#include <random>

#include "Misc/Common.h"

#include "PhysicsComponents.h"
#include "Rendering/Mesh.h"
#include "Engine/Input.h"

namespace seng
{

std::array<float,2> fixAtTopEndsInitializer(int Nx, int Ny, std::vector<Node>& nodes)
{
    float x{0.f}, y{0.f}, z{0.f};
    int index{-1};
    float rodLength = 1.f/glm::min(Nx-1, Ny-1);
    for (int ii = 0; ii < Nx; ++ii) {
        y = 0.f;
        for (int jj = 0; jj < Ny; ++jj) {
            index = Matrix2VectorIdx(ii, jj, Nx);
            nodes[index].currentPosition.x = x;
            nodes[index].currentPosition.y = y;
            nodes[index].currentPosition.z = z;

            nodes[index].prevPosition.x = x;
            nodes[index].prevPosition.y = y;
            nodes[index].prevPosition.z = z;

            // Increment y value by one rod length
            y += rodLength;
        }
        // Increment x value by one rod length
        x += rodLength;
        if(ii % (Nx-1) == 0)
        {
            nodes[Matrix2VectorIdx(ii, Ny - 1, Nx)].fixedFlag = true;
        }
    }



    return std::array<float,2>{rodLength, rodLength};
}
std::function<std::array<float,2>(int, int, std::vector<Node>&)> RodCloth::m_fixAtTopEnds = fixAtTopEndsInitializer;



//***********************************************************
//       RodCloth Methods
//***********************************************************

/////////////  RodCloth Ctor  ///////////////////
RodCloth::RodCloth(int numXNodes, int numYNodes, std::function<std::array<float,2>(int, int, std::vector<Node>&)> initializer,
                   Material* material, float mass, unsigned int numIterations) :
        m_Nx(numXNodes), m_Ny(numYNodes), m_mass(mass), m_numConstraintIterations(numIterations), m_initializer(initializer),
        m_shuffleIndex{std::vector<int>(m_Nx*m_Ny)}, m_material{material}, m_gridMesh(numXNodes, numYNodes),
        m_nodes(std::vector<Node>(numXNodes*numYNodes))
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
    std::array<float,2> rodLengths = m_initializer(m_Nx, m_Ny, m_nodes);
    m_rodLengthX = rodLengths[0];
    m_rodLengthY = rodLengths[1];

    InitializeAdjNodes();

    // Create or clear Mesh component
    if(parentObject->HasComponent<Mesh>())
    {
        assert("Trying to add mesh in RodCloth when object already has Mesh component");
    }
    parentObject->AddComponent(new Mesh());
    Mesh& mesh = parentObject->GetComponent<Mesh>();
    std::vector<unsigned int> elements = std::vector<unsigned int>(m_gridMesh.GetNumElements());
    int elementIdx = 0;
    for(int jj = 0; jj < m_Ny - 1; ++jj)
    {
        for(int ii = 0; ii < m_Nx - 1; ++ii)
        {
            elements[elementIdx] = ii + jj*m_Nx;
            elements[elementIdx+1] = ii + 1 + jj*m_Nx;
            elements[elementIdx+2] = ii + (jj+1)*m_Nx;

            elements[elementIdx+3] = ii + 1 + jj*m_Nx;
            elements[elementIdx+4] = ii + 1 + (jj+1)*m_Nx;
            elements[elementIdx+5] = ii + (jj+1)*m_Nx;

            elementIdx += 6;
        }
    }
    m_gridMesh.SetGridUVCoords();

    mesh.AddMeshDataNewVAO(std::vector<Vert3x3x2f>(m_gridMesh.GetNumVertices()), elements, *m_material);

    m_gridMesh.UpdateVerticesAndReloadVBO(m_nodes, mesh);
}

void RodCloth::Update(float deltaTime)
{
    //***********************************************************
    //       DEMO
    //***********************************************************
    timer += deltaTime;
    Integrate(deltaTime);
    ProjectConstraints();
    m_gridMesh.UpdateVerticesAndReloadVBO(m_nodes, this->parentObject->GetComponent<Mesh>());

    if(timer > restTime + pushTime)
        timer = 0.0f;
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


void RodCloth::Integrate(float deltaTime)
{
    int index{-1};
    /////////////  Integrate F=ma with Verlet  ///////////////////
    for(int nodeIdx = 0; nodeIdx < m_nodes.size(); ++nodeIdx)
    {
        if(m_nodes[nodeIdx].fixedFlag)
            continue;

        glm::vec3 forces = m_mass*m_g;
//        if(Input::GetInstance().KeyPress(GLFW_KEY_SPACE) && nodeIdx % m_Nx < m_Nx - 2 && nodeIdx % m_Nx > 1)
//            forces += glm::vec3(0.f, 0.f, 1.3f);

//        if(timer < restTime+pushTime && timer >  restTime && nodeIdx % m_Nx < m_Nx - 2 && nodeIdx % m_Nx > 1)
//            forces += glm::vec3(0.f, 0.f, 1.3f);

        glm::vec3 oldCurrentPos = m_nodes[nodeIdx].currentPosition;
        m_nodes[nodeIdx].currentPosition = 2.f*m_nodes[nodeIdx].currentPosition - m_nodes[nodeIdx].prevPosition;
        m_nodes[nodeIdx].currentPosition += deltaTime*deltaTime*forces/m_mass;

        m_nodes[nodeIdx].prevPosition = oldCurrentPos;
    }
}


void RodCloth::ProjectConstraints()
{
    Node* currentNode{nullptr};
    Node* adjNode{nullptr};
    for(int iter = 0; iter < m_numConstraintIterations; ++iter)
    {
        for(int ii = 0; ii < m_nodes.size(); ++ii)
        {
            int nodeIdx = m_shuffleIndex[ii];
            currentNode = &m_nodes[nodeIdx];
//            if(currentNode->fixedFlag)
//                continue;



            for(int adjIdx : m_nodes[nodeIdx].adjIndices)
            {
                adjNode = &m_nodes[adjIdx];

                glm::vec3 deltaDir = currentNode->currentPosition - adjNode->currentPosition;
                float deltaDirLength = glm::length(deltaDir);
                deltaDir /= deltaDirLength;
                float delta = (deltaDirLength - m_rodLengthX);

                if(currentNode->fixedFlag)
                {
                    adjNode->currentPosition += delta*deltaDir;
                    continue;
                }
                if(adjNode->fixedFlag)
                {
                    currentNode->currentPosition -= delta*deltaDir;
                    continue;
                }

                currentNode->currentPosition -= 0.5f*delta*deltaDir;
                adjNode->currentPosition += 0.5f*delta*deltaDir;
            }

        }
    } // Iterations

}


} // seng