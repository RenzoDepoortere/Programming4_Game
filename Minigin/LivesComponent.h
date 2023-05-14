#pragma once
#include "Component.h"
#include "Observer.h"
#include <string>

namespace dae
{
	class GameObject;
	class HealthComponent;
	class TextComponent;

	class LivesComponent final : public Component, public Observer<>
	{
	public:
		// Rule of Five
		explicit LivesComponent(GameObject* pParentObject);
		virtual ~LivesComponent() override;

		LivesComponent(const LivesComponent& other) = delete;
		LivesComponent(LivesComponent&& other) = delete;
		LivesComponent& operator=(const LivesComponent& other) = delete;
		LivesComponent& operator=(LivesComponent&& other) = delete;

		// Public functions
		virtual void HandleEvent(unsigned int eventID) override;
		virtual void OnSubjectDestroy() override;

		void SetComponentToFollow(HealthComponent* pToFollow);

	private:
		// Member variables
		HealthComponent* m_pToFollow{ nullptr };
		int m_NrLives{};
		std::string m_BaseString{};

		TextComponent* m_pTextComponent{ nullptr };

		// Member functions
		void UpdateText();
	};
}