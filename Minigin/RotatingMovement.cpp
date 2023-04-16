 #include "RotatingMovement.h"
#include "GameObject.h"

#define CLOCKWISE 1
#define COUNTERCLOCKWISE -1

dae::RotatingMovement::RotatingMovement(dae::GameObject* pParentObject)
	: Component{ pParentObject }
{
	// Defaults
	m_RotationSpeed = 1.f;
	m_RotationDistance = 100.f;
	m_RotatingDirection = CLOCKWISE;
}

void dae::RotatingMovement::Update(float deltaTime)
{
	// Calculate angle
	m_AccumulatedFrames += deltaTime * m_RotationSpeed * m_RotatingDirection;

	// Rotate
	glm::vec3 newObjectPos{};
	newObjectPos.x = std::cosf(m_AccumulatedFrames) * m_RotationDistance;
	newObjectPos.y = std::sinf(m_AccumulatedFrames) * m_RotationDistance;

	// Set new pos
	GetGameObject()->SetLocalPosition(newObjectPos);
}

void dae::RotatingMovement::SetRotationSpeed(float rotationSpeed)
{
	m_RotationSpeed = rotationSpeed;
}
float dae::RotatingMovement::GetRotationSpeed() const
{
	return m_RotationSpeed;
}
void dae::RotatingMovement::SetRotationDistance(float rotationDistance)
{
	m_RotationDistance = rotationDistance;
}
float dae::RotatingMovement::GetRotationDistance() const
{
	return m_RotationDistance;
}

void dae::RotatingMovement::SetClockwiseRotationDirection(bool goClockwise)
{
	if (goClockwise)	m_RotatingDirection = CLOCKWISE;
	else				m_RotatingDirection = COUNTERCLOCKWISE;
}
bool dae::RotatingMovement::GetClockwiseRotationDirection() const
{
	if (m_RotatingDirection == CLOCKWISE)	return true;
	else									return false;
}