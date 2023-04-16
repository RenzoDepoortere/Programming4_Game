#pragma once
#include <string>
#include <list>
#include <any>
#include <vector>

namespace dae { class GameObject; }

struct Event
{
	std::string eventName{};
	dae::GameObject* pGameObject{ nullptr };
	std::vector<std::any> extraArguments{};
};

class Follower
{
public:
	// Rule of five
	explicit Follower() = default;
	virtual ~Follower() = default;

	Follower(const Follower& other) = delete;
	Follower(Follower&& other) = delete;
	Follower& operator=(const Follower& other) = delete;
	Follower& operator=(Follower&& other) = delete;

	// Public functions
	virtual void Update(float deltaTime);
	virtual void FixedUpdate(float deltaTime);
	virtual void Render() const;
	virtual void RenderImGUI();

	virtual void Notify(const Event& event) = 0;

private:
};

class Listener : public Follower
{
public:
	// Rule of five
	explicit Listener() = default;
	virtual ~Listener() = default;

	Listener(const Listener& other) = delete;
	Listener(Listener&& other) = delete;
	Listener& operator=(const Listener& other) = delete;
	Listener& operator=(Listener&& other) = delete;

	// Public functions
	virtual void Notify(const Event& event) = 0;
	virtual void AddSubscription(const std::string& eventName);

	virtual const std::list<std::string>& GetSubscriptions() const { return m_Subscriptions; }

private:
	// Member variables
	// ----------------
	std::list<std::string> m_Subscriptions{};
};

class Observer : public Follower
{
public:
	// Rule of five
	explicit Observer(dae::GameObject* pGameObjectToFollow);
	virtual ~Observer();

	Observer(const Observer& other) = delete;
	Observer(Observer&& other) = delete;
	Observer& operator=(const Observer& other) = delete;
	Observer& operator=(Observer&& other) = delete;

	// Public functions
	virtual void Notify(const Event& event) = 0;

protected:
	dae::GameObject* GetGameObject() const { return m_pGameObject; }

private:
	// Member variables
	// ----------------
	dae::GameObject* m_pGameObject{ nullptr };
};