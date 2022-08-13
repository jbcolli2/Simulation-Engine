//
// Created by jcollin2 on 8/9/22.
//

#ifndef SIM_ENGINE_CLOTHROD_H
#define SIM_ENGINE_CLOTHROD_H


#include "Systems/System.h"


namespace seng
{

/////////////////    Forward Declared Classes    ///////////////////////
template <typename T>
        class Grid;
class GridMesh;



//***********************************************************
//       Node class for masses in cloth
//***********************************************************
struct Node
{
    glm::vec3 currentPosition;
    glm::vec3 prevPosition;
    bool fixedFlag{false};
};




//***********************************************************
//       System for rod-based cloth simulation
//***********************************************************
class ClothRod : public System
{
private:
    // Position data for each mass in the cloth
    Grid<Node> m_masses;

    // Length of all axis-aligned rods connecting masses (assumed to be same among all masses)
    float m_rodLength{1.f};
    // Mass of all masses making up the cloth
    float m_mass{1.f};
    // Value of acceleration due to gravity
    const glm::vec3 m_g{glm::vec3(0.f, -9.8f, 0.f)};

    // pointer to the grid mesh
    GridMesh* m_gridMesh;
    // Dimensions of the GridMesh
    int m_Nx{0}, m_Ny{0};

    // Number of iterations to use when updating the mass positions
    int m_updateIterations{5};

    // Adjacency data of each node
    Grid< std::vector< std::array<int,2> > > m_adjNodes;


public:
    ClothRod(GridMesh* gridMesh);

    void StartUp() override;

    void Update(float deltaTime) override;

    void ShutDown() override;
};



#define X 0
#define Y 1
#define Z 2
inline void printClothPos(const Grid<Node>& grid, int coord)
{
    int Nx = grid.getNx();
    int Ny = grid.getNy();

    for(int jj = Ny-1; jj >=0; --jj)
    {
        for(int ii = 0; ii < Nx; ++ii)
        {
            switch(coord)
            {
                case X:
                    std::cout << grid.index(ii,jj).currentPosition.x << "  ";
                    break;
                case Y:
                    std::cout << grid.index(ii,jj).currentPosition.y << "  ";
                    break;
                case Z:
                    std::cout << grid.index(ii,jj).currentPosition.z << "  ";
                    break;
                default:
                    break;
            }
        }
        std::cout << "\n";
    }
}


inline void printClothPos(const Grid<Node>& grid)
{
    int Nx = grid.getNx();
    int Ny = grid.getNy();

    for(int jj = Ny-1; jj >=0; --jj)
    {
        for(int ii = 0; ii < Nx; ++ii)
        {
            std::cout << "(" << grid.index(ii,jj).currentPosition.x << ", " << grid.index(ii,jj).currentPosition.y <<
                ", " << grid.index(ii,jj).currentPosition.z << ")  ";
        }
        std::cout << "\n";
    }
}


} // seng

#endif //SIM_ENGINE_CLOTHROD_H
