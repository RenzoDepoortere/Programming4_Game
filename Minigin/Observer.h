#pragma once

namespace dae
{
	template<typename... Args>
	class Observer
	{
	public:
		// Rule of five
		Observer() = default;
		virtual ~Observer() = default;

		Observer(const Observer& other) = delete;
		Observer(Observer&& other) = delete;
		Observer& operator=(const Observer& other) = delete;
		Observer& operator=(Observer&& other) = delete;

		// Public functions
		virtual void HandleEvent(unsigned int eventID, Args... args) = 0;
		virtual void OnSubjectDestroy() = 0;
	};
}