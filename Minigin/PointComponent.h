#pragma once
#include "Component.h"

namespace dae
{
	class GameObject;

	class PointComponent final : public Component
	{
	public:
		// Rule of Five
		explicit PointComponent(dae::GameObject* pParentObject);
		virtual ~PointComponent() override = default;

		PointComponent(const PointComponent& other) = delete;
		PointComponent(PointComponent&& other) = delete;
		PointComponent& operator=(const PointComponent& other) = delete;
		PointComponent& operator=(PointComponent&& other) = delete;

		// Public functions
		void SetCurrentPoint(int amount);
		int GetCurrentPoint() const { return m_CurrentPoints; }

	private:
		// Member variables
		int m_CurrentPoints{};

		// Member functions
	};
}