//
// Created by Jeb Collins on 3/15/22.
//

#ifndef PECS_RENDERER_INPUT_HPP
#define PECS_RENDERER_INPUT_HPP



#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include <queue>
#include <unordered_map>
#include <array>
#include <iostream>


namespace seng
{




namespace Action
{
    constexpr unsigned int None = 0;
    constexpr unsigned int Down = 1;
    constexpr unsigned int Pressed = 2;
    constexpr unsigned int Up = 4;
}

struct KeyEvent
{
    int keyCode{0};
    int action{0};
    int mod{0};
};

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mod);

void cursorPositionCallback(GLFWwindow *window, double xPosition, double yPosition);

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);


class Input
{
private:
    // container for recent input events that have yet to be handled by the code
    std::queue<KeyEvent> m_unhandledKeys;
    // true means button pressed, false means button released
    std::array<bool, 4> m_unhandledMouseButtons{ {false, false, false, false} };
    // hash table of all current key inputs.  Engine uses this to query keyboard input.
    std::unordered_map<int, unsigned int> m_keyState;
    // Position of the mouse
    float m_mouseX, m_mouseY;
    // Whether left/right mouse button pressed
    std::array<unsigned int, 4> m_mouseButtonState;
public:



    // Get instance of the singleton
    static Input& GetInstance()
    {
        static Input instance{};
        return instance;
    }
    /***************** SetupCallbacks  ******************
     * @brief Setup all the callback functions for input using GLFW.
    ******************************************************************/
    void SetupCallbacks();

    /***************** HandleNewInputs  ******************
     * @brief Callbacks push new key states to unhandledKeys.  This method processes those
     *      new key states and updates keyState map with new states of the keys.  Having this
     *      intermediate step allows up to differentiate between keyDown and keyPressed and keyUp.
     *
    ******************************************************************/
    void PollInput();


    // Getters for the keyStates
    bool KeyDown(int keyCode) {return m_keyState[keyCode] == Action::Down;};

    bool KeyPress(int keyCode)
    { return m_keyState[keyCode] == Action::Pressed; };

    bool KeyUp(int keyCode)
    { return m_keyState[keyCode] == Action::Up; };

    // Getters for mouse states
    glm::vec2 MousePosition()
    { return glm::vec2{m_mouseX, m_mouseY};};
    bool LeftMouseDown()
    { return m_mouseButtonState[GLFW_MOUSE_BUTTON_LEFT] & Action::Down;};

    bool LeftMousePressed()
    { return m_mouseButtonState[GLFW_MOUSE_BUTTON_LEFT] & Action::Pressed; };

    bool LeftMouseUp()
    { return m_mouseButtonState[GLFW_MOUSE_BUTTON_LEFT] & Action::Up; };

    bool RightMouseDown()
    { return m_mouseButtonState[GLFW_MOUSE_BUTTON_RIGHT] & Action::Down; };

    bool RightMousePressed()
    { return m_mouseButtonState[GLFW_MOUSE_BUTTON_RIGHT] & Action::Pressed; };

    bool RightMouseUp()
    { return m_mouseButtonState[GLFW_MOUSE_BUTTON_RIGHT] & Action::Up; };


    /***************** Input callback functions  ******************
     * @brief All the callback functions that will simply pass the input the the Input class.
    ******************************************************************/
    friend void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mod);
    friend void cursorPositionCallback(GLFWwindow* window, double xPosition, double yPosition);
    friend void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);

private:
    // Hide or delete all constructors to make this a singleton
    // TODO: How can I write this input system without using singleton pattern?
    Input() = default;
    Input(const Input&) = delete;
    void operator=(const Input&) = delete;
    Input(Input&&) = delete;
    void operator=(Input&&) = delete;
};




} // end namespace pecs
#endif //PECS_RENDERER_INPUT_HPP
