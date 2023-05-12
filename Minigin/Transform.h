#pragma once
#include <glm/glm.hpp>

namespace dae
{
	class Transform final
	{
	public:
		// Positions
		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(float x, float y, float z);
		void SetPosition(const glm::vec3& position);

		// Rotations
		float GetRotation() const { return m_Rotation; }
		void SetRotation(float degrees);

	private:
		// Member variables
		glm::vec3 m_Position;
		float m_Rotation;
	};
}
