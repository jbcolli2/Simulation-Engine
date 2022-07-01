//
// Created by jcollin2 on 6/30/22.
//

#ifndef SIM_ENGINE_SIMENGINE_H
#define SIM_ENGINE_SIMENGINE_H


class SimEngine
{
private:
    DisplayManager m_displayManager{};
    SceneManager m_sceneManager{};
    Renderer m_renderer{};

public:
    SimEngine() {};

    void Start(int width, int height);
    void Run();
    void Stop();

};


#endif //SIM_ENGINE_SIMENGINE_H
