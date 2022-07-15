//
// Created by Jeb Collins on 2/24/22.
//

#ifndef seng_RENDERER_OBJECT_HPP
#define seng_RENDERER_OBJECT_HPP

#include "Misc/Util.h"
#include "ObjComp/Component.h"
#include "ObjComp/ComponentManager.h"


#include <memory>
#include <vector>
#include <iostream>

namespace seng
{


// ///////////// CLASS: Object   ////////////////
/**
 \brief Most general 

 \param  -

 \returns
 */
class Object
{
private:
    // all the components that make up the object
    std::unordered_map<const char*, Component*> m_components{};

    // Signature listing what components are attached
    Signature m_signature;

public:

    Object() = default;

    /***************** Object dtor  ******************
         * @brief Loop through all components in the object and delete them.
    ******************************************************************///
    ~Object();

    //***********************************************************
    //       Getters and Setters
    //***********************************************************
    Signature GetSignature() const {return m_signature;};











    //***********************************************************
    //      Component Methods
    //***********************************************************

    /***************** GetComponent  ******************
     * @brief Returns the component T if it exists in `m_components`.  For now
    we assume there only exists one of each component in an object.
     *
     * @tparam CompT Component type to check for and return if in object.
     *
     * @returns Pointer to CompT component attached to object.
    ******************************************************************///
    template <typename CompT>
    CompT* GetComponent();



    /***************** AddComponent  ******************
     * @brief Add a component to the object.  If a component of that type already exists, then it
     *  will not be added.
     *  TODO: Modify if code changes to allow multiple of the same component to an object.
     *
     * @tparam CompT Component type
     *
     * @param component A pointer to the component being added.
    ***************************************///
    template <typename CompT>
    void AddComponent(CompT* component);



    /***************** HasComponent  ******************
     * @brief Returns true if passed object has component in its signature.
     *
     * @tparam CompT Component type to check for in object.
     *
     * @param object Object to check for component.
     *
     * @return True if object has CompT attached, false otherwise.
    ******************************************************************/
    template<class CompT>
    bool HasComponent();



    /***************** HasSignature  ******************
     * @brief Returns true if passed object contains a list of components
     *      denoted by  signature within the objects signature.
     *
     *    Object could have more components, but must have at least all the
     *      components in the signature.
     *
     * @param signature Signature representing a list of components to check for
     *
     * @returns True if object has signature as part of it's signature, false otherwise.
    ******************************************************************/
    bool HasSignature(const Signature& signature);
}; // End Object class










//***********************************************************
//       Templated Method Definitions
//***********************************************************



/***************** GetComponent  ******************
 * @brief Returns the component T if it exists in `m_components`.  For now
we assume there only exists one of each component in an object.
 *
 * @tparam CompT Component type to check for and return if in object.
 *
 * @returns Pointer to CompT component attached to object.
******************************************************************///
template <typename CompT>
CompT* Object::GetComponent()
{
    auto compMapEntry = m_components.find(typeid(CompT).name());
    // Be sure that object actually has the component before trying to return it
    assert(compMapEntry != m_components.end() && "SENG Error: Component not found in Object::GetComponent");
    CompT* component = dynamic_cast<CompT*>(compMapEntry->second);

    return component;
}



/***************** AddComponent  ******************
 * @brief Add a component to the object.  If a component of that type already exists, then it
 *  will not be added.
 *  ComponentManager::GetSignature will add a component to the signature list if this is the
 *      first time that component has been added to an object.
 *
 * @tparam CompT Component type
 *
 * @param component A pointer to the component being added.
***************************************///
template <typename CompT>
void Object::AddComponent(CompT* component)
{
    m_components.insert({typeid(CompT).name(), component});

    m_signature |= ComponentManager::GetSignature<CompT>();
}



/***************** HasComponent  ******************
 * @brief Returns true if passed object has component in its signature.
 *
 * @tparam CompT Component type to check for in object.
 *
 * @param object Object to check for component.
 *
 * @return True if object has CompT attached, false otherwise.
******************************************************************/
template<typename CompT>
bool Object::HasComponent()
{
    return ((GetSignature() & ComponentManager::GetSignature<CompT>()) != 0);
}






} // End seng namespace
#endif //seng_RENDERER_OBJECT_HPP
