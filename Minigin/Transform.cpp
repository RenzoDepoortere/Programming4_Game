#include "Transform.h"

void dae::Transform::SetPosition(const float x, const float y, const float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}
void dae::Transform::SetPosition(const glm::vec3& position)
{
	SetPosition(position.x, position.y, position.z);
}

void dae::Transform::SetRotation(float degrees)
{
	m_Rotation = degrees;
}