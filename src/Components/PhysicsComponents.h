//
// Created by jcollin2 on 9/29/22.
//

#ifndef SIM_ENGINE_PHYSICSCOMPONENTS_H
#define SIM_ENGINE_PHYSICSCOMPONENTS_H


#include "Misc/MeshCommon.h"

#include "Engine/Component.h"
#include "Misc/DynamicMeshCommon.h"


namespace seng
{

class Material;
class Mesh;
//***********************************************************
//       System for rod-based cloth simulation
//***********************************************************
class RodCloth : public Component
{
private:

    int m_Nx{0}, m_Ny{0};                               // Dimensions of the GridMesh
    float m_mass{1.f};                                  // Mass of all nodes making up the cloth
    int m_numConstraintIterations{10};                   // Number of iterations to use when projecting the constraints
    std::function<std::array<float,2>(int, int, std::vector<Node>&)> m_initializer;     // Function pointer to definite initial state of cloth
    Material* m_material;                               // Material used to render the cloth

    std::vector<Node> m_nodes{};                          // State data for each node in the cloth
    std::vector<int> m_shuffleIndex{};                  // Shuffled indices to access the nodes in random order


    float m_rodLengthX{1.f};                             // Cache the length of all axis-aligned rods connecting masses
    float m_rodLengthY{1.f};
    // TODO: Change to allow for different lenghts in each direction
    // TODO: Add diagonal rod length


    const glm::vec3 m_g{glm::vec3(0.f, -9.8f, 0.f)};    // Value of acceleration due to gravity




    // pointer to the grid mesh
    DynamicGridMesh m_gridMesh;
    unsigned int m_meshVBO{0};

    int mat2VecIdx(int ii, int jj)
    {
        return Matrix2VectorIdx(ii, jj, m_Nx);
    };

    enum class InitialClothState
    {
        VERTICAL_FIX_ENDS
    };

    //***********************************************************
    //       Methods to break up StartUp and Update
    //***********************************************************
    void InitializeAdjNodes();
    void Integrate(float deltaTime);
    void ProjectConstraints();


    //***********************************************************
    //       DEMO
    //***********************************************************
    float timer = 0.f;
    float restTime = .5f;
    float pushTime = 1.5f;

public:
    RodCloth(int numXNodes, int numYNodes, std::function<std::array<float,2>(int, int, std::vector<Node>&)> initializer, Material* material, float mass = 1.f, unsigned int numIterations = 5);

    void StartUp() override;

    void Update(float deltaTime) override;

    //***********************************************************
    //       Initializer functions for Cloth
    //***********************************************************
    static std::function<std::array<float,2>(int, int, std::vector<Node>&)> m_fixAtTopEnds;

}; // End RodCloth class


} // seng

#endif //SIM_ENGINE_PHYSICSCOMPONENTS_H
