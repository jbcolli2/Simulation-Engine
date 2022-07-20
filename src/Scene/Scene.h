//
// Created by jcollin2 on 7/3/22.
//

#ifndef SIM_ENGINE_SCENE_H
#define SIM_ENGINE_SCENE_H

#include "ObjComp/Object.h"

#include "Rendering/Shader.hpp"
#include "Rendering/Renderable.h"



namespace seng
{


class Scene
{
private:
    std::vector<Object*> m_renderables{};
    std::vector<Object*> m_lights{};
    std::vector<Object*> m_cameras{};


public:
    /***************** AddObject  ******************
     * @brief Adds an object to the scene.  Sorts object into the correct object list of
     *      either renderables, a type of light, or camera.
     *
     * @param object Object to add.
    ******************************************************************///
    void AddObject(Object* object);


    /***************** DrawScene  ******************
     * @brief Call draw methods in all renderable objects.  Assume shader program is
     *      already in use.
    ******************************************************************///
    void DrawScene(Shader& shader);
}; // end class Scene


} // end namespace seng

#endif //SIM_ENGINE_SCENE_H
