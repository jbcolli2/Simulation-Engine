//
// Created by Jeb Collins on 2/27/22.
//

#include "ComponentManager.hpp"


using namespace pecs;

std::unordered_map<const char*, Signature> ComponentManager::m_componentList{};
int ComponentManager::numComponents = 0;







