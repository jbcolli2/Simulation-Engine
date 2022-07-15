//
// Created by jcollin2 on 7/3/22.
//

#include "Renderer.h"

using namespace seng;





int Renderer::StartUp(const SceneManager& sceneManager)
{
//    Shader shader{"../assets/shaders/PosNormUVVert.glsl", "../assets/shaders/SolidColorFrag.glsl"};

    return 1;
}






void Renderer::Render()
{
    glClearColor(r, g, b, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

}


