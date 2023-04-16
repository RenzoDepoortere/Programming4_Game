#pragma once
#include <glm/vec2.hpp>

namespace dae
{
	class Controller final
	{
	public:
		// Rule of five
		explicit Controller(unsigned long controllerIndex);
		~Controller();

		Controller(const Controller& other) = delete;
		Controller(Controller&& other) = delete;
		Controller& operator=(const Controller& other) = delete;
		Controller& operator=(Controller&& other) = delete;

		// Public functions
		void ProcessInput();

		bool IsPressed(unsigned int button) const;
		bool IsReleased(unsigned int button) const;
		bool IsDown(unsigned int button) const;

		glm::vec2 GetLeftThumbInput() const;
		glm::vec2 GetRightThumbInput() const;

		float GetLeftTriggerInput() const;
		float GetRightTriggerInput() const;

		unsigned long GetControllerIndex() const;

	private:
		class ControllerImpl;
		ControllerImpl* m_pImpl{ nullptr };
	};
}