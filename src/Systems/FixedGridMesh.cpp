//
// Created by jcollin2 on 8/3/22.
//



#include "Scene/GridMesh.h"

#include "Systems/FixedGridMesh.h"

namespace seng
{



void FixedGridMesh::StartUp()
{
    int Nx = m_gridMesh->GetNx();
    int Ny = m_gridMesh->GetNy();

    float x, z, y;

    for(int ii = 0; ii < Nx; ++ii)
    {
        for(int jj = 0; jj < Ny; ++jj)
        {
            x = static_cast<float>(ii)/(Nx-1);
            z = static_cast<float>(jj)/(Ny-1);
            y = glm::sqrt(m_radius*m_radius - x*x - z*z);
            m_gridMesh->SetVertexPos(Matrix2VectorIdx(ii, jj, Nx), glm::vec3(x,y,z));
        }
    }

    m_gridMesh->SetupForDraw();
}





} // seng