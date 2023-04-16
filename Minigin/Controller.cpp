#include "Controller.h"
#include "InputManager.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <thread>
#include <Xinput.h>
#include <math.h>

#define THUMB_AXIS_MAX 32767
#define TRIGGER_MAX 255

class dae::Controller::ControllerImpl
{
public:
	// Rule of five
	explicit ControllerImpl(DWORD controllerIndex);
	~ControllerImpl() = default;

	ControllerImpl(const ControllerImpl& other) = delete;
	ControllerImpl(ControllerImpl&& other) = delete;
	ControllerImpl& operator=(const ControllerImpl& other) = delete;
	ControllerImpl& operator=(ControllerImpl&& other) = delete;

	// Public functions
	void ProcessInput();

	bool IsPressed(unsigned int button) const;
	bool IsReleased(unsigned int button) const;
	bool IsDown(unsigned int button) const;

	glm::vec2 GetLeftThumbInput() const;
	glm::vec2 GetRightThumbInput() const;

	float GetLeftTriggerInput() const;
	float GetRightTriggerInput() const;

	DWORD GetControllerIndex() const;

private:
	// Member Variables
	// ----------------
	DWORD m_ControllerIndex{};

	XINPUT_STATE m_CurrentState{};
	XINPUT_STATE m_PreviousState{};

	int m_ButtonsPressed{};
	int m_ButtonsReleased{};

	float m_DeadZone{ 0.2f };
};

#pragma region Controller
dae::Controller::Controller(unsigned long controllerIndex)
{
	m_pImpl = new ControllerImpl{ controllerIndex };
}
dae::Controller::~Controller()
{
	delete m_pImpl;
	m_pImpl = nullptr;
}

void dae::Controller::ProcessInput()
{
	m_pImpl->ProcessInput();
}

bool dae::Controller::IsPressed(unsigned int button) const
{
	return m_pImpl->IsPressed(button);
}
bool dae::Controller::IsReleased(unsigned int button) const
{
	return m_pImpl->IsReleased(button);
}
bool dae::Controller::IsDown(unsigned int button) const
{
	return m_pImpl->IsDown(button);
}

glm::vec2 dae::Controller::GetLeftThumbInput() const
{
	return m_pImpl->GetLeftThumbInput();
}
glm::vec2 dae::Controller::GetRightThumbInput() const
{
	return m_pImpl->GetRightThumbInput();
}

float dae::Controller::GetLeftTriggerInput() const
{
	return m_pImpl->GetLeftTriggerInput();
}
float dae::Controller::GetRightTriggerInput() const
{
	return m_pImpl->GetRightTriggerInput();
}

unsigned long dae::Controller::GetControllerIndex() const
{
	return m_pImpl->GetControllerIndex();
}
#pragma endregion

#pragma region ControllerImpl
dae::Controller::ControllerImpl::ControllerImpl(DWORD controllerIndex)
	: m_ControllerIndex{ controllerIndex }
{
}

void dae::Controller::ControllerImpl::ProcessInput()
{
	// Store previousInput and get newInput
	CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	XInputGetState(m_ControllerIndex, &m_CurrentState);

	// Get button status
	int buttonChanges{ m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons };
	m_ButtonsPressed = buttonChanges & m_CurrentState.Gamepad.wButtons;
	m_ButtonsReleased = buttonChanges & (~m_CurrentState.Gamepad.wButtons);
}

bool dae::Controller::ControllerImpl::IsPressed(unsigned int button) const
{
	return m_ButtonsPressed & button;
}
bool dae::Controller::ControllerImpl::IsReleased(unsigned int button) const
{
	return m_ButtonsReleased & button;
}
bool dae::Controller::ControllerImpl::IsDown(unsigned int button) const
{
	return m_CurrentState.Gamepad.wButtons & button;
}

glm::vec2 dae::Controller::ControllerImpl::GetLeftThumbInput() const
{
	// Get input
	glm::vec2 input{ static_cast<float>(m_CurrentState.Gamepad.sThumbLX), static_cast<float>(m_CurrentState.Gamepad.sThumbLY) };
	
	// Convert to [-1,1]
	input.x /= THUMB_AXIS_MAX;
	input.y /= THUMB_AXIS_MAX;

	// If below deadZone treshold, put to 0
	if (std::abs(input.x) < m_DeadZone) input.x = 0;
	if (std::abs(input.y) < m_DeadZone) input.y = 0;

	// Return
	return input;
}
glm::vec2 dae::Controller::ControllerImpl::GetRightThumbInput() const
{
	// Get input
	glm::vec2 input{ static_cast<float>(m_CurrentState.Gamepad.sThumbRX), static_cast<float>(m_CurrentState.Gamepad.sThumbRY) };
	
	// Convert to [-1,1]
	input.x /= THUMB_AXIS_MAX;
	input.y /= THUMB_AXIS_MAX;

	// If below deadZone treshold, put to 0
	if (std::abs(input.x) < m_DeadZone) input.x = 0;
	if (std::abs(input.y) < m_DeadZone) input.y = 0;

	// return
	return input;
}

float dae::Controller::ControllerImpl::GetLeftTriggerInput() const
{
	float inputValue{ static_cast<float>(m_CurrentState.Gamepad.bLeftTrigger) };
	inputValue /= TRIGGER_MAX;

	return inputValue;
}
float dae::Controller::ControllerImpl::GetRightTriggerInput() const
{
	float inputValue{ static_cast<float>(m_CurrentState.Gamepad.bRightTrigger) };
	inputValue /= TRIGGER_MAX;

	return inputValue;
}

DWORD dae::Controller::ControllerImpl::GetControllerIndex() const
{
	return m_ControllerIndex;
}
#pragma endregion