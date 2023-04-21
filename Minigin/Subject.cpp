#include "Subject.h"

template<typename... Args>
dae::Subject<Args...>::~Subject()
{
	for (auto& currentObersver : m_pObservers)
	{
		currentObersver->OnSubjectDestroy();
	}
}

template<typename... Args>
void dae::Subject<Args...>::AddObserver(Observer<Args...>* pObserver)
{
	m_pObservers.push_back(pObserver);
}
template<typename... Args>
void dae::Subject<Args...>::RemoveObserver(Observer<Args...>* pObserver)
{
	m_pObservers.erase(std::remove(m_pObservers.begin(), m_pObservers.end(), pObserver), m_pObservers.end());
}
template<typename... Args>
void dae::Subject<Args...>::Notify(Args... args)
{
	for (auto& currentObserver : m_pObservers)
	{
		currentObserver->HandleEvent(args...);
	}
}