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

	player.setSize(sf::Vector2f(50,50));
	player.setPosition(sf::Vector2f(100, 100));
	player.setFillColor(sf::Color(255, 0, 0, 255));
	posPlayer = player.getPosition();
}

void Test::update()
{
    if (KEY(R) and m_windowManager.timer() > 0.2f)
    {
        m_windowManager.resetTimer();
        GET_MANAGER->loadScene("Test");
        pushState(1);
    }

	GET_MANAGER->getSteam().getManette().update();

	if (GET_MANAGER->getSteam().getManette().get_analog_action("Move").y > 0.3f)
	{
		posPlayer.y -= 0.2;
	}
	else if (GET_MANAGER->getSteam().getManette().get_analog_action("Move").y < -0.3f)
	{
		posPlayer.y += 0.2;
	}
	else if (GET_MANAGER->getSteam().getManette().get_analog_action("Move").x < -0.3f)
	{
		posPlayer.x -= 0.2;
	}
	else if (GET_MANAGER->getSteam().getManette().get_analog_action("Move").x > 0.3f)
	{
		posPlayer.x += 0.2;
	}

	player.setPosition(posPlayer);
}

void Test::render()
{
    m_animation.Animate(m_windowManager.getWindow(), 6);
	m_windowManager.draw(player);
}

void Test::pushState(char data)
{
    if (data == 1)
        m_stackState->push(std::make_unique<Test>(m_windowManager, m_stackState));
}
