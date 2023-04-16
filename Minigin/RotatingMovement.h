#pragma once
#include "Component.h"
#include "Transform.h"

namespace dae
{

	class RotatingMovement final : public Component
	{
	public:
		// Rule of Five
		explicit RotatingMovement(dae::GameObject* pParentObject);
		virtual ~RotatingMovement() override = default;

		RotatingMovement(const RotatingMovement& other) = delete;
		RotatingMovement(RotatingMovement&& other) = delete;
		RotatingMovement& operator=(const RotatingMovement& other) = delete;
		RotatingMovement& operator=(RotatingMovement&& other) = delete;

		// Virtual functions
		virtual void Update(float deltaTime) override;

		// Rotation functions
		void SetRotationSpeed(float rotationSpeed);
		float GetRotationSpeed() const;
		void SetRotationDistance(float rotationDistance);
		float GetRotationDistance() const;

		void SetClockwiseRotationDirection(bool goClockwise);
		bool GetClockwiseRotationDirection() const;

	private:
		float m_RotationSpeed{};
		float m_RotationDistance{};
		int m_RotatingDirection{};

		float m_AccumulatedFrames{};
	};

}