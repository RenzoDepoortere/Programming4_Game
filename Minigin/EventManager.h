#pragma once
#include "Singleton.h"
#include "Observer.h"

#include <map>
#include <list>
#include <iostream>

namespace dae
{
	template<typename... Args>
	class EventManager final : public Singleton<EventManager<Args...>>
	{
	public:
		// Functionality
		// -------------
		void Subscribe(unsigned int eventID, Observer<Args...>* pObserver)
		{
			// Add to list
			m_pSubscribers[eventID].push_back(pObserver);
		}
		void Unsubscribe(unsigned int eventID, Observer<Args...>* pObserver)
		{
			// If no subcribers to event, return
			if (m_pSubscribers.find(eventID) == m_pSubscribers.end()) return;

			// Remove from list
			auto& subscribers{ m_pSubscribers[eventID] };
			subscribers.remove(pObserver);
		}

		void SendEvent(unsigned int eventID, Args... args)
		{
			// If are subscribers
			auto listIt{ m_pSubscribers.find(eventID) };
			if (listIt != m_pSubscribers.end())
			{
				// Send event to observers
				for (auto& currentObserver : listIt->second)
				{
					currentObserver->HandleEvent(eventID, args...);
				}
			}
		}

	private:
		// Member variables
		std::map<unsigned int, std::list<Observer<Args...>*>> m_pSubscribers{};
	};
}