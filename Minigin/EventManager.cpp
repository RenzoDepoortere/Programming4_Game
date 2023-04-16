#include "EventManager.h"

void dae::EventManager::Update(float deltaTime)
{
	// Delete marked followers
	DeleteMarkedFollowers();

	// Handle interested listeners with events
	HandleEventQueue();

	// Update followers
	for (const auto& currentFollower : m_pFollowers)
	{
		currentFollower->Update(deltaTime);
	}
}
void dae::EventManager::FixedUpdate(float deltaTime)
{
	// FixedUpdate followers
	for (const auto& currentFollower : m_pFollowers)
	{
		currentFollower->FixedUpdate(deltaTime);
	}
}
void dae::EventManager::Render() const
{
	// Render followers
	for (const auto& currentFollower : m_pFollowers)
	{
		currentFollower->Render();
	}
}
void dae::EventManager::RenderImGUI()
{
	// Render ImGUI followers
	for (const auto& currentFollower : m_pFollowers)
	{
		currentFollower->RenderImGUI();
	}
}

void dae::EventManager::AddFollower(std::unique_ptr<Listener>&& pListener)
{
	// Add to listeners
	m_pListeners.push_back(pListener.get());

	// Add to followers
	AddFollowerToVector(std::move(pListener));
}
void dae::EventManager::AddFollower(std::unique_ptr<Observer>&& pObserver)
{
	// Add to observers
	m_pObservers.push_back(pObserver.get());

	// Add to followers
	AddFollowerToVector(std::move(pObserver));
}

void dae::EventManager::RemoveAll()
{
	// Add to followers to delete
	m_FollowersToDelete.insert(m_FollowersToDelete.begin(), std::make_move_iterator(m_pFollowers.begin()), std::make_move_iterator(m_pFollowers.end()));

	// Clear normal containers
	m_pListeners.clear();
	m_pObservers.clear();
	m_pFollowers.clear();
}

void dae::EventManager::SendEvent(const Event& event)
{
	// Adds event to the queue
	m_Events.push(event);
}

void dae::EventManager::AddFollowerToVector(std::unique_ptr<Follower>&& pFollower)
{
	// Add to followers
	m_pFollowers.push_back(std::move(pFollower));
}

void dae::EventManager::DeleteMarkedFollowers()
{
	// If there are followers to delete
	if (m_FollowersToDelete.size() > 0)
	{
		// Clear vector
		m_FollowersToDelete.clear();
	}
}
void dae::EventManager::HandleEventQueue()
{
	// While there are events
	while (m_Events.empty() == false)
	{
		// Get first event
		const Event currentEvent{ m_Events.front() };

		// Loop through listeners
		for (const auto& currentListener : m_pListeners)
		{
			// Get subscriptions
			const std::list<std::string>& subscriptions{ currentListener->GetSubscriptions() };

			// Lambda: check for same name
			auto hasSameEventName = [currentEvent](const std::string& currentElement)
			{
				return currentEvent.eventName == currentElement;
			};

			// Check if listener is interested in event
			bool isInterested{ false };
			isInterested = std::find_if(subscriptions.cbegin(), subscriptions.cend(), hasSameEventName) != subscriptions.cend();
			if (isInterested == false) continue;

			// Notify
			currentListener->Notify(currentEvent);
		}

		// Remove event
		m_Events.pop();
	}
}