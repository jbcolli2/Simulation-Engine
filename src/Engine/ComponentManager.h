//
// Created by Jeb Collins on 2/27/22.
//

#ifndef seng_RENDERER_COMPONENTMANAGER_HPP
#define seng_RENDERER_COMPONENTMANAGER_HPP

#include "Misc/Util.h"

#include <unordered_map>


namespace seng {
class ComponentManager {
private:
    // hash table to store each component type name with its Signature
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
     * @brief Returns the signature for a particular component CompT.
     *
     *      If this CompT is not in m_componentList, i.e. if this is the
     *      first time the component signature has been requested,
     *      then ia new signature will be given to it and added to m_componentList.
     *      Then this new signature will be returned.
     *
     * @tparam CompT Component type for which the signature is requested
     *
     * @returns Signature of CompT
******************************************************************///
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
















} // end of seng namespace
#endif //seng_RENDERER_COMPONENTMANAGER_HPP
