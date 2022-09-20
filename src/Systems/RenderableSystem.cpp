//
// Created by jcollin2 on 7/23/22.
//

#include "RenderableSystem.h"

namespace seng
{


//***********************************************************
//       RenderSystem Class
//***********************************************************
void RenderableSystem::Update(float deltaTime)
{
    for(Object* object : m_objects)
    {
        Transform& transform{object->GetTransform()};
        if(transform.updateModelMatrix)
        {
            // TODO: Create reference to m_model and just use that
            Mesh* renderable = object->GetComponent<Mesh>();

            // TODO: create a constant identity mat4 rather than recreating it each time
            renderable->m_model = glm::translate(glm::mat4(1.f), transform.position);
            renderable->m_model = glm::rotate(renderable->m_model, glm::radians(transform.rotation.x),
                                              glm::vec3(1.f, 0.f, 0.f));
            renderable->m_model = glm::rotate(renderable->m_model, glm::radians(transform.rotation.y),
                                              glm::vec3(0.f, 1.f, 0.f));
            renderable->m_model = glm::rotate(renderable->m_model, glm::radians(transform.rotation.z),
                                              glm::vec3(0.f, 0.f, 1.f));
            renderable->m_model = glm::scale(renderable->m_model, transform.scale);

            transform.updateModelMatrix = false;
        }
    }

}




//***********************************************************
//       MoveObject Class
//***********************************************************
void MoveObject::StartUp()
{
    time = 0.f;
    m_amplitude = 1.f;
    m_frequency = 1.f;
}

void MoveObject::Update(float deltaTime)
{
    time += deltaTime;
    Transform& transform = m_objects[0]->GetTransform();
    transform.position.x = m_amplitude*glm::sin(time*m_frequency);
}


} // seng