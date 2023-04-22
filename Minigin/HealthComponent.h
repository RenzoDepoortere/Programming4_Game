#pragma once
#include "Component.h"
#include "glm/vec3.hpp"
#include "Subject.h"

namespace dae
{
	class GameObject;

	class HealthComponent final : public Component
	{
	public:
		// Rule of Five
		explicit HealthComponent(dae::GameObject* pParentObject);
		virtual ~HealthComponent() override = default;

		HealthComponent(const HealthComponent& other) = delete;
		HealthComponent(HealthComponent&& other) = delete;
		HealthComponent& operator=(const HealthComponent& other) = delete;
		HealthComponent& operator=(HealthComponent&& other) = delete;

		// Public functions
		void SetMaxHealth(float amount) { m_MaxHealth = amount; }
		float GetMaxHealth() const { return m_MaxHealth; }

		void SetCurrentHealth(float amount);
		float GetCurrentHealth() const { return m_CurrentHealth; }

		template<typename... Args>
		Subject<>* GetSubject() { return &m_HasDied; }

		void Respawn();

	private:
		// Member variables
		float m_MaxHealth{};
		float m_CurrentHealth{};

		glm::vec3 m_StartPos{};

		dae::Subject<> m_HasDied{};
	};
}