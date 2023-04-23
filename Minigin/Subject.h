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
		~Subject()
		{
			for (auto& currentObersver : m_pObservers)
			{
				currentObersver->OnSubjectDestroy();
			}
		}

		Subject(const Subject& other) = delete;
		Subject(Subject&& other) = delete;
		Subject& operator=(const Subject& other) = delete;
		Subject& operator=(Subject&& other) = delete;

		// Public functions
		void AddObserver(Observer<Args...>* pObserver)
		{
			m_pObservers.push_back(pObserver);
		}
		void RemoveObserver(Observer<Args...>* pObserver)
		{
			m_pObservers.erase(std::remove(m_pObservers.begin(), m_pObservers.end(), pObserver), m_pObservers.end());
		}
		void Notify(Args... args)
		{
			for (auto& currentObserver : m_pObservers)
			{
				currentObserver->HandleEvent(-1, args...);
			}
		}

	private:
		// Member variables
		std::vector<Observer<Args...>*> m_pObservers{};
	};
}