#include "State.h"
#include "SFML_ENGINE/RessourcesManager.h"


State::State(WindowManager& _window, std::stack<std::unique_ptr<State>>* stackState) : m_windowManager(_window), m_stackState(stackState)
{
}

void State::update()
{
}

Test::Test(WindowManager& _window, StateStack* stackState) : State(_window, stackState)
{
    m_animation = Animation(GET_MANAGER->getTexture("porte_intro_1224x855"),sf::IntRect(0,0,1224,855),0.5,16);
}

void Test::update()
{
    if (KEY(R) and m_windowManager.timer() > 0.2f)
    {
        m_windowManager.resetTimer();
        GET_MANAGER->loadScene("Test");
        pushState(1);
    }
}

void Test::render()
{
    m_animation.Animate(m_windowManager.getWindow(), 6);
}

void Test::pushState(char data)
{
    if (data == 1)
        m_stackState->push(std::make_unique<Test>(m_windowManager, m_stackState));
}
