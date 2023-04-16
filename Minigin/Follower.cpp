#include "Follower.h"
#include "GameObject.h"

void Follower::Update(float)
{
}
void Follower::FixedUpdate(float)
{
}
void Follower::Render() const
{
}
void Follower::RenderImGUI()
{
}


void Listener::AddSubscription(const std::string& eventName)
{
	m_Subscriptions.push_back(eventName);
}

Observer::Observer(dae::GameObject* pGameObjectToFollow)
	: m_pGameObject{ pGameObjectToFollow }
{
}
Observer::~Observer()
{
	m_pGameObject->RemoveObserver(this);
}