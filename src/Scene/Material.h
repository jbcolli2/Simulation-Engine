//
// Created by jcollin2 on 7/18/22.
//

#ifndef SIM_ENGINE_MATERIAL_H
#define SIM_ENGINE_MATERIAL_H

#include "Rendering/Shader.hpp"

namespace seng
{
class Material
{

public:
    void virtual SetupMaterial(Shader& shader) = 0;
};
} //end namespace seng


#endif //SIM_ENGINE_MATERIAL_H
