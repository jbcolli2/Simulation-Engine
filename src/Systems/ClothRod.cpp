//
// Created by jcollin2 on 8/9/22.
//

#include <array>
#include <random>
#include "Misc/Common.h"
#include "Engine/Input.h"


#include "Rendering/Mesh.h"
#include "Scene/GridMesh.h"

#include "Systems/ClothRod.h"


namespace seng
{




ClothRod::ClothRod(GridMesh* gridMesh)
{
    m_gridMesh = gridMesh;

    m_Nx = gridMesh->GetNx();
    m_Ny = gridMesh->GetNy();

    m_masses = std::vector<Node>(m_Nx*m_Ny);

    m_shuffleIndex = std::vector<int>(m_Nx*m_Ny);
    for(int ii = 0; ii < m_shuffleIndex.size(); ++ii)
    {
        m_shuffleIndex[ii] = ii;
    }

    /////////////  Fill in adjacency of the nodes  ///////////////////
    m_adjNodes = std::vector< std::vector<int> >(m_Nx*m_Ny);

    int index{-1};
    // Inner nodes
    for(int ii = 1; ii < m_Nx-1; ++ii)
    {
        for(int jj = 1; jj < m_Ny-1; ++jj)
        {
            index = mat2VecIdx(ii, jj);

            m_adjNodes[index].push_back(mat2VecIdx(ii, jj - 1)); // S
            m_adjNodes[index].push_back(mat2VecIdx(ii + 1, jj)); // E
            m_adjNodes[index].push_back(mat2VecIdx(ii, jj + 1)); // N
            m_adjNodes[index].push_back(mat2VecIdx(ii - 1, jj)); // W
        }
    }

    // Inner top and bottom
    for(int ii = 1; ii < m_Nx-1; ++ii)
    {
        index = mat2VecIdx(ii, 0);
        m_adjNodes[index].push_back(mat2VecIdx(ii - 1, 0)); // W
        m_adjNodes[index].push_back(mat2VecIdx(ii + 1, 0)); // E
        m_adjNodes[index].push_back(mat2VecIdx(ii, 1)); // N

        index = mat2VecIdx(ii, m_Ny - 1);
        m_adjNodes[index].push_back(mat2VecIdx(ii - 1, m_Ny - 1)); // W
        m_adjNodes[index].push_back(mat2VecIdx(ii + 1, m_Ny - 1)); // E
        m_adjNodes[index].push_back(mat2VecIdx(ii, m_Ny - 2)); // S
    }

    // Inner left and right
    for(int jj = 1; jj < m_Ny-1; ++jj)
    {
        index = mat2VecIdx(0, jj);
        m_adjNodes[index].push_back(mat2VecIdx(0, jj + 1)); // N
        m_adjNodes[index].push_back(mat2VecIdx(0, jj - 1)); // S
        m_adjNodes[index].push_back(mat2VecIdx(1, jj)); // E

        index = mat2VecIdx(m_Nx - 1, jj);
        m_adjNodes[index].push_back(mat2VecIdx(m_Nx - 1, jj + 1)); // N
        m_adjNodes[index].push_back(mat2VecIdx(m_Nx - 1, jj - 1)); // S
        m_adjNodes[index].push_back(mat2VecIdx(m_Nx - 2, jj)); // W
    }

    // Corners
    index = mat2VecIdx(0, 0);
    m_adjNodes[index].push_back(mat2VecIdx(0, 1)); // N
    m_adjNodes[index].push_back(mat2VecIdx(1, 0)); // E

    index = mat2VecIdx(m_Nx - 1, 0);
    m_adjNodes[index].push_back(mat2VecIdx(m_Nx - 1, 1)); // N
    m_adjNodes[index].push_back(mat2VecIdx(m_Nx - 2, 0)); // W

    index = mat2VecIdx(0, m_Ny - 1);
    m_adjNodes[index].push_back(mat2VecIdx(0, m_Ny - 2)); // S
    m_adjNodes[index].push_back(mat2VecIdx(1, m_Ny - 1)); // E

    index = mat2VecIdx(m_Nx - 1, m_Ny - 1);
    m_adjNodes[index].push_back(mat2VecIdx(m_Nx - 1, m_Ny - 2)); // S
    m_adjNodes[index].push_back(mat2VecIdx(m_Nx - 2, m_Ny - 1)); // W

}


/***************** ClothRod::Startup  ******************
 * @brief Initialize the positions of all the masses.  Set certain ones to be fixed.
******************************************************************///
void ClothRod::StartUp()
{
    /////////////////    Setup cloth parameters    ///////////////////////
//    m_mass = 100.f/(m_Nx*m_Ny);
    m_mass = 1.f;
    m_rodLength = 1.f/glm::min(m_Nx-1, m_Ny-1);
    m_updateIterations = 15;

    auto rd = std::random_device {};
    auto rng = std::default_random_engine { rd() };
    std::shuffle(m_shuffleIndex.begin(), m_shuffleIndex.end(), rng);


    /////////////  Initialize the positions of the masses  ///////////////////
    float x{0.f},y{0.f},z{0.f};
    int index{-1};
    for(int ii = 0; ii < m_Nx; ++ii)
    {
        y = 0.f;
        for(int jj = 0; jj < m_Ny; ++jj)
        {
            index = mat2VecIdx(ii, jj);
            m_masses[index].currentPosition.x = x;
            m_masses[index].currentPosition.y = y;
            m_masses[index].currentPosition.z = z;

            m_masses[index].prevPosition.x = x;
            m_masses[index].prevPosition.y = y;
            m_masses[index].prevPosition.z = z;

            // Increment y value by one rod length
            y += m_rodLength;
        }
        // Increment x value by one rod length
        x += m_rodLength;

        // Now fix every third node at top of cloth
        if(ii % 9 == 0)
        {
            m_masses[mat2VecIdx(ii, m_Ny - 1)].fixedFlag = true;
        }

    }



    for(int vertIdx = 0; vertIdx < m_masses.size(); ++vertIdx)
    {
        m_gridMesh->SetVertexPos(vertIdx, m_masses[vertIdx].currentPosition);
    }


}



/***************** ClothRod::Update  ******************
 * @brief Update the positions of all the masses, based on F=ma and rod constraints.
 *
 *      1. Integrate F=ma with Verlet method:
 *
 *          x_n = 2x_n - x_{n-1} + (dt^2) (F/m)
 *          x_{n-1} = x_n
 *
 *      2. Update positions based on rod constraints.  If two adjacent nodes are more or less than
 *          a rods length apart, adjust both nodes towards or away from each other by half the
 *          delta they are off from rod length.
 *
 *          Be sure that fixed nodes stay fixed
 *
 *      3. Iterate step 2. a fixed number of times.
 *
******************************************************************///
void ClothRod::Update(float deltaTime)
{
    int index{-1};
    /////////////  Integrate F=ma with Verlet  ///////////////////
    for(int nodeIdx = 0; nodeIdx < m_masses.size(); ++nodeIdx)
    {
        if(m_masses[nodeIdx].fixedFlag)
            continue;

        glm::vec3 forces = m_mass*m_g;
        if(Input::GetInstance().KeyPress(GLFW_KEY_SPACE) && nodeIdx % m_Nx < m_Nx - 2 && nodeIdx % m_Nx > 1)
            forces += glm::vec3(0.f, 0.f, 1.3f);

        glm::vec3 oldCurrentPos = m_masses[nodeIdx].currentPosition;
        m_masses[nodeIdx].currentPosition = 2.f*m_masses[nodeIdx].currentPosition - m_masses[nodeIdx].prevPosition;
        m_masses[nodeIdx].currentPosition += deltaTime*deltaTime*forces/m_mass;

        m_masses[nodeIdx].prevPosition = oldCurrentPos;
    }



    /////////////  Project contraints  ///////////////////
    Node* currentNode{nullptr};
    Node* adjNode{nullptr};
    for(int iter = 0; iter < m_updateIterations; ++iter)
    {
        for(int ii = 0; ii < m_masses.size(); ++ii)
        {
            int nodeIdx = m_shuffleIndex[ii];
            currentNode = &m_masses[nodeIdx];
//            if(currentNode->fixedFlag)
//                continue;



            for(const auto& adjIdx : m_adjNodes[nodeIdx])
            {
                adjNode = &m_masses[adjIdx];

                glm::vec3 deltaDir = currentNode->currentPosition - adjNode->currentPosition;
                float deltaDirLength = glm::length(deltaDir);
                deltaDir /= deltaDirLength;
                float delta = (deltaDirLength - m_rodLength);

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


    for(int vertIdx = 0; vertIdx < m_masses.size(); ++vertIdx)
    {
        m_gridMesh->SetVertexPos(vertIdx, m_masses[vertIdx].currentPosition);
    }
}

void ClothRod::ShutDown()
{

}


} // seng