//
// Created by jcollin2 on 6/30/22.
//

#include "SimEngine.h"

void SimEngine::Start(int width, int height)
{
    m_displayManager.StartUp(width, height);
    m_sceneManager.StartUp();
    m_renderer.StartUp(m_displayManager);
}
