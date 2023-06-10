#pragma once
#include "Component.h"

#include <vector>

namespace dae
{
	class GameObject;
	class RenderTextureComponent;
}

class LivesComponent final : public Component
{
public:
	// Rule of Five
	explicit LivesComponent(dae::GameObject* pParentObject);
	virtual ~LivesComponent() override = default;

	LivesComponent(const LivesComponent& other) = delete;
	LivesComponent(LivesComponent&& other) = delete;
	LivesComponent& operator=(const LivesComponent& other) = delete;
	LivesComponent& operator=(LivesComponent&& other) = delete;

	// Functionality	
	void SetMaxNrLives(int maxLives);
	bool LoseLive();					// Returns whether the player died completely or not
	void ResetLives();

private:
	// Member variables
	// ----------------
	int m_MaxLives{};
	int m_CurrentLives{};

	std::vector<dae::GameObject*> m_pRenders{};
};

