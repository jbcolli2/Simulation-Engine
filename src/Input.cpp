//
// Created by Jeb Collins on 3/15/22.
//

#include "Input.h"



using namespace seng;

//std::queue<KeyEvent> Input::m_unhandledKeys{};
//std::unordered_map<int, unsigned int> keyState{};


/***************** Input callback functions  ******************
 * @brief All the callback functions that will simply pass the input the the Input class.
******************************************************************/
void seng::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mod)
{
    Input::GetInstance().m_unhandledKeys.push(KeyEvent{key, action, mod});
}

void seng::cursorPositionCallback(GLFWwindow *window, double xPosition, double yPosition)
{
    Input::GetInstance().m_mouseX = static_cast<float>(xPosition);
    Input::GetInstance().m_mouseY = static_cast<float>(yPosition);
}

void seng::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    Input::GetInstance().m_unhandledMouseButtons[button] = (action == GLFW_PRESS);
}

/***************** HandleNewInputs  ******************
 * @brief Callbacks push new key states to unhandledKeys.  This method processes those
 *      new key states and updates keyState map with new states of the keys.  Having this
 *      intermediate step allows up to differentiate between keyDown and keyPressed and keyUp.
 *
******************************************************************/
void Input::PollInput()
{
    glfwPollEvents();

    //***********************************************************
    //       Handle keyboard events
    //***********************************************************
    for(auto& keyState : m_keyState)
    {
        if(keyState.second & Action::Down)
        {
            keyState.second = Action::Pressed;
        }

        if(keyState.second & Action::Up)
        {
            keyState.second = Action::None;
        }
    }
    while(!m_unhandledKeys.empty())
    {
        KeyEvent keyEvent = m_unhandledKeys.front();
        m_unhandledKeys.pop();

        if(keyEvent.action == GLFW_RELEASE)
        {
            m_keyState[keyEvent.keyCode] = Action::Up;
        }
        if(keyEvent.action == GLFW_PRESS && !(m_keyState[keyEvent.keyCode] & Action::Pressed))
        {
            m_keyState[keyEvent.keyCode] = Action::Down;
        }

    }


    //***********************************************************
    //       Handle mouse button events
    //***********************************************************
    for(auto& buttonState : m_mouseButtonState)
    {
        if(buttonState & Action::Down)
        {
            buttonState = Action::Pressed;
        }
        if(buttonState & Action::Up)
        {
            buttonState = Action::None;
        }
    }


    for(int ii = 0; ii < m_unhandledMouseButtons.size(); ++ii)
    {
        if(m_mouseButtonState[ii] & Action::Pressed && !m_unhandledMouseButtons[ii])
        {
            m_mouseButtonState[ii] |= Action::Up;
        }
        if(!m_mouseButtonState[ii] && m_unhandledMouseButtons[ii])
        {
            m_mouseButtonState[ii] |= (Action::Down | Action::Pressed);
        }
    }
}
