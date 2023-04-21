#pragma once
#include <vector>
#include "Observer.h"

namespace dae
{
	template<typename... Args>
	class Subject final
	{
	public:
		// Rule of five
		Subject() = default;
		~Subject();	

		Subject(const Subject& other) = delete;
		Subject(Subject&& other) = delete;
		Subject& operator=(const Subject& other) = delete;
		Subject& operator=(Subject&& other) = delete;

		// Public functions
		void AddObserver(Observer<Args...>* pObserver);
		void RemoveObserver(Observer<Args...>* pObserver);
		void Notify(Args... args);

	private:
		// Member variables
		std::vector<Observer<Args...>*> m_pObservers{};
	};
}