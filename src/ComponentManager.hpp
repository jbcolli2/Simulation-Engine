//
// Created by Jeb Collins on 2/27/22.
//

#ifndef PECS_RENDERER_COMPONENTMANAGER_HPP
#define PECS_RENDERER_COMPONENTMANAGER_HPP

#include "Util.hpp"

#include <unordered_map>


namespace pecs {
class ComponentManager {
private:
    // hash table to store each component type name with its integer identifier
    static std::unordered_map<const char*, Signature> m_componentList;
    // Number of components registered
    static int numComponents;

public:
    ComponentManager() = default;

    /***************** GetSignature  ******************
     * @brief Static method that returns the signature of the given component in template
     *      parameter.
    ***************************************///
    template <typename CompT>
    static Signature GetSignature();

};  // End of ComponentManager class









/***************** GetSignature  ******************
 * @brief Static method that returns the signature of the given component in template
 *      parameter.
***************************************///
template <typename CompT>
Signature ComponentManager::GetSignature()
{
    try
    {
        auto compSig = m_componentList.at(typeid(CompT).name());
        return compSig;
    }
    catch(const std::out_of_range& e)
    {
        ComponentManager::numComponents++;
        Signature newCompSig;
        newCompSig.set(numComponents);
        m_componentList.insert({typeid(CompT).name(), newCompSig});

        return newCompSig;
    }

}
















} // end of pecs namespace
#endif //PECS_RENDERER_COMPONENTMANAGER_HPP
