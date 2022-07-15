//
// Created by Jeb Collins on 2/25/22.
//

#include "ObjComp//Object.h"

using namespace seng;


/***************** Object dtor  ******************
 * @brief Loop through all components in the object and delete them.
******************************************************************///
Object::~Object()
{
    for (const auto it : m_components)
    {
        delete it.second;
    }
}


/***************** HasSignature  ******************
 * @brief Returns true if passed object contains a list of components
 *      denoted by  signature within the objects signature.
 *
 *    NOTE: Object could have more components, but must have at least all the
 *      components in the signature.
 *
 * @param signature Signature representing a list of components to check for
 *
 * @returns True if object has signature as part of it's signature, false otherwise.
******************************************************************/
bool seng::Object::HasSignature(const Signature &signature)
{
    return ((GetSignature() & signature) != 0);
}

