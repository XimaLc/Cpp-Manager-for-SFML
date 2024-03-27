#pragma once
#include "Tools.h"

class SFMLENGINE_API SteamManager
{
private:

public:
	SteamManager();
	void update();
	~SteamManager();
};


class ManetteHandle
{
private:
	InputHandle_t m_manetteHandles[STEAM_INPUT_MAX_COUNT];
	int m_nb_manette;
	InputActionSetHandle_t m_actionSet;
	ScePadTriggerEffectParam param{};
	std::map<std::string, InputDigitalActionData_t> m_buttons_actions;
	std::map<std::string, std::tuple<InputAnalogActionData_t, std::array<sf::Vector2f, 10>, unsigned short>> m_analog_actions;
	std::map<std::string, InputActionSetHandle_t> m_actions;
	std::string m_bind_group_name;

	bool m_rebind_controller = false;

public:
	ManetteHandle();
	ManetteHandle(std::string _bind_group_name);
	~ManetteHandle();

	void create_analog_action(std::string _action);
	void create_button_action(std::string _action);

	void update();

	inline int& getNbController() { return m_nb_manette; }
	inline InputHandle_t* getHandleController() { return m_manetteHandles; }
	InputAnalogActionData_t& get_analog_action(std::string _action) { return std::get<0>(m_analog_actions[_action]); }
	InputDigitalActionData_t& get_button_action(std::string _action) { return m_buttons_actions[_action]; }
};