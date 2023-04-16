#pragma once
#include "Singleton.h"
#include "Follower.h"

#include <memory>
#include <vector>
#include <queue>

namespace dae
{
	class EventManager final : public Singleton<EventManager>
	{
	public:
		// Destructor and rule of five
		~EventManager() = default;

		EventManager(const EventManager& other) = delete;
		EventManager(EventManager&& other) = delete;
		EventManager& operator=(const EventManager& other) = delete;
		EventManager& operator=(EventManager&& other) = delete;

		// Public Functions
		void Update(float deltaTime);
		void FixedUpdate(float deltaTime);
		void Render() const;
		void RenderImGUI();

		void AddFollower(std::unique_ptr<Listener>&& pListener);
		void AddFollower(std::unique_ptr<Observer>&& pObserver);

		void RemoveAll();

		void SendEvent(const Event& event);

	private:
		friend class Singleton<EventManager>;
		explicit EventManager() = default;

		// Member Variables
		// ----------------
		std::vector<Listener*> m_pListeners{};
		std::vector<Observer*> m_pObservers{};

		std::vector<std::unique_ptr<Follower>> m_pFollowers{};
		std::vector<std::unique_ptr<Follower>> m_FollowersToDelete{};

		std::queue<Event> m_Events{};

		// Member Functions
		// ----------------
		void AddFollowerToVector(std::unique_ptr<Follower>&& pFollower);

		void DeleteMarkedFollowers();
		void HandleEventQueue();
	};
}