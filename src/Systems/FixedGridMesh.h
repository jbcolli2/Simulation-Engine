//
// Created by jcollin2 on 8/3/22.
//

#ifndef SIM_ENGINE_FIXEDGRIDMESH_H
#define SIM_ENGINE_FIXEDGRIDMESH_H

namespace seng
{

class GridMesh;

class FixedGridMesh
{
private:
    GridMesh* m_gridMesh;
    float m_radius{1.f};

public:
    FixedGridMesh(GridMesh* gridMesh) : m_gridMesh(gridMesh){};

    void StartUp();
};

} // seng

#endif //SIM_ENGINE_FIXEDGRIDMESH_H
