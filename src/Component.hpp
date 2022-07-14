//
// Created by Jeb Collins on 2/25/22.
//

#ifndef PECS_RENDERER_COMPONENT_HPP
#define PECS_RENDERER_COMPONENT_HPP


#include <unordered_map>

namespace pecs
{
class Component
{
private:

public:
    Component() = default;

    virtual ~Component() = default;

};







/***************** Pipeline Component  ******************
 * @brief Dummy component to let RenderPipeline know that the object should be sent to the
 *      Pipeline system.
******************************************************************/
struct Pipeline : public Component
{
    Pipeline() = default;
};
} // End namespace pecs
#endif //PECS_RENDERER_COMPONENT_HPP
