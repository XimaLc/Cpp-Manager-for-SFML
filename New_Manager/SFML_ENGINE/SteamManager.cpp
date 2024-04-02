#include "SteamManager.h"

#pragma region API

SteamManager::SteamManager()
{
    std::cout << "------- Starting steam API load ------- \n";

    if (SteamAPI_Init())
        SteamInput()->Init(true);

    std::cout << "------- Steam API loading finish ------- \n\n\n";
}
void SteamManager::update()
{
    SteamAPI_RunCallbacks();
}
SteamManager::~SteamManager()
{
    SteamAPI_Shutdown();
}

ManetteHandle& SteamManager::getManette()
{
	return m_manetteH;
}

#pragma endregion

#pragma region MANETTE

ManetteHandle::ManetteHandle()
{
}

ManetteHandle::~ManetteHandle() 
{
}

void ManetteHandle::create_analog_action(std::string _action)
{
	InputAnalogActionHandle_t tmp_analog_action = SteamInput()->GetAnalogActionHandle(_action.c_str());
	m_analog_actions[_action] = std::make_tuple(SteamInput()->GetAnalogActionData(m_manetteHandles[0], tmp_analog_action), std::array<sf::Vector2f, 10>{}, 0u);
}

void ManetteHandle::create_button_action(std::string _action)
{
	InputDigitalActionHandle_t tmp_button_action = SteamInput()->GetDigitalActionHandle(_action.c_str());
	m_buttons_actions[_action] = SteamInput()->GetDigitalActionData(m_manetteHandles[0], tmp_button_action);
}

void ManetteHandle::init(std::string _bind_group_name)
{
	m_nb_manette = SteamInput()->GetConnectedControllers(m_manetteHandles);
	m_actionSet = SteamInput()->GetActionSetHandle(_bind_group_name.c_str());
	SteamInput()->ActivateActionSet(m_manetteHandles[0], m_actionSet);
	m_bind_group_name = _bind_group_name;
	m_rebind_controller = false;
	
}

void ManetteHandle::update()
{
	if (!SteamInput()->GetConnectedControllers(m_manetteHandles))
		m_rebind_controller = true;

	if (m_rebind_controller)
	{
		m_nb_manette = SteamInput()->GetConnectedControllers(this->getHandleController());
		m_actionSet = SteamInput()->GetActionSetHandle(m_bind_group_name.c_str());
		SteamInput()->ActivateActionSet(m_manetteHandles[0], m_actionSet);

		std::map<std::string, InputDigitalActionData_t> tmp_bind_buttons;
		for (auto& button : m_buttons_actions)
		{
			InputDigitalActionHandle_t tmp_button_action = SteamInput()->GetDigitalActionHandle(button.first.c_str());
			tmp_bind_buttons[button.first] = SteamInput()->GetDigitalActionData(m_manetteHandles[0], tmp_button_action);
		}

		m_buttons_actions.clear();
		m_buttons_actions = tmp_bind_buttons;

		std::map<std::string, std::tuple<InputAnalogActionData_t, std::array<sf::Vector2f, 10>, unsigned short>> tmp_bind_analog;
		for (auto& analog : m_analog_actions)
		{
			InputAnalogActionHandle_t tmp_analog_action = SteamInput()->GetAnalogActionHandle(analog.first.c_str());
			tmp_bind_analog[analog.first] = std::make_tuple(SteamInput()->GetAnalogActionData(m_manetteHandles[0], tmp_analog_action), std::array<sf::Vector2f, 10>{}, 0u);
		}

		m_analog_actions.clear();
		m_analog_actions = tmp_bind_analog;

		if (m_nb_manette > 0)
			m_rebind_controller = false;
	}

	InputDigitalActionHandle_t tmp_button_action;
	for (auto& m_button : m_buttons_actions)
	{
		tmp_button_action = SteamInput()->GetDigitalActionHandle(m_button.first.c_str());
		m_button.second = SteamInput()->GetDigitalActionData(m_manetteHandles[0], tmp_button_action);
	}

	InputAnalogActionHandle_t tmp_analog_action;
	InputActionSetHandle_t tmp_action_set_handle;
	InputAnalogActionData_t tmp_analog_position;
	for (auto& analog : m_analog_actions)
	{
		tmp_analog_action = SteamInput()->GetAnalogActionHandle(analog.first.c_str());
		tmp_analog_position = SteamInput()->GetAnalogActionData(m_manetteHandles[0], tmp_analog_action);

		std::get<1>(analog.second)[std::get<2>(analog.second)] = sf::Vector2f(tmp_analog_position.x, tmp_analog_position.y);
		std::get<2>(analog.second)++;

		if (std::get<2>(analog.second) == static_cast<unsigned short>(std::get<1>(analog.second).size()))
		{
			if (std::find_if(std::get<1>(analog.second).begin(), std::get<1>(analog.second).end(), [](sf::Vector2f _position)
				{
					if (_position == sf::Vector2f())
						return false;
					else
						return true;
				}) == std::get<1>(analog.second).end())
			{
				std::get<0>(analog.second) = tmp_analog_position;
			}

			std::get<2>(analog.second) = 0u;
		}

		if (tmp_analog_position.x != 0 || tmp_analog_position.y != 0)
		{
			std::get<0>(analog.second) = tmp_analog_position;
		}

		memset(&param, 0, sizeof(param));
		param.triggerMask = SCE_PAD_TRIGGER_EFFECT_TRIGGER_MASK_R2;
		param.command[SCE_PAD_TRIGGER_EFFECT_PARAM_INDEX_FOR_R2].mode = SCE_PAD_TRIGGER_EFFECT_MODE_WEAPON;
		param.command[SCE_PAD_TRIGGER_EFFECT_PARAM_INDEX_FOR_R2].commandData.weaponParam.startPosition = 2;
		param.command[SCE_PAD_TRIGGER_EFFECT_PARAM_INDEX_FOR_R2].commandData.weaponParam.endPosition = 8;
		param.command[SCE_PAD_TRIGGER_EFFECT_PARAM_INDEX_FOR_R2].commandData.weaponParam.strength = 5;
		SteamInput()->SetDualSenseTriggerEffect(m_manetteHandles[0], &param);


		tmp_action_set_handle = SteamInput()->GetActionSetHandle(analog.first.c_str());
		SteamInput()->ActivateActionSet(m_manetteHandles[0], tmp_action_set_handle);
	}
}

void ManetteHandle::setVibration()
{
	param.triggerMask = SCE_PAD_TRIGGER_EFFECT_TRIGGER_MASK_R2;
	param.command[SCE_PAD_TRIGGER_EFFECT_PARAM_INDEX_FOR_R2].mode = SCE_PAD_TRIGGER_EFFECT_MODE_WEAPON;
	param.command[SCE_PAD_TRIGGER_EFFECT_PARAM_INDEX_FOR_R2].commandData.weaponParam.startPosition = 2;
	param.command[SCE_PAD_TRIGGER_EFFECT_PARAM_INDEX_FOR_R2].commandData.weaponParam.endPosition = 8;
	param.command[SCE_PAD_TRIGGER_EFFECT_PARAM_INDEX_FOR_R2].commandData.weaponParam.strength = 5;
}


#pragma endregion