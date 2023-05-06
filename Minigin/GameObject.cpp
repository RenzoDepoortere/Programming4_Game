#include <string>
#include "GameObject.h"

void dae::GameObject::Update(float deltaTime)
{
	// Delete marked components
	DeleteMarkedComponents();

	// Update Components
	for (const auto& currentComponent : m_pComponents)
	{
		currentComponent.second->Update(deltaTime);
	}

	// Update Children
	for (const auto& currentChild : m_pChildren)
	{
		currentChild->Update(deltaTime);
	}
}
void dae::GameObject::FixedUpdate(float deltaTime)
{
	// Fixed-Update Components
	for (const auto& currentComponent : m_pComponents)
	{
		currentComponent.second->FixedUpdate(deltaTime);
	}

	// Fixed-Update Children
	for (const auto& currentChild : m_pChildren)
	{
		currentChild->FixedUpdate(deltaTime);
	}
}
void dae::GameObject::Render() const
{
	// Render Components
	for (const auto& currentComponent : m_pComponents)
	{
		currentComponent.second->Render();
	}

	// Render Children
	for (const auto& currentChild : m_pChildren)
	{
		currentChild->Render();
	}
}
void dae::GameObject::RenderImGUI()
{
	// Render Components
	for (const auto& currentComponent : m_pComponents)
	{
		currentComponent.second->RenderImGUI();
	}

	// Render Children
	for (const auto& currentChild : m_pChildren)
	{
		currentChild->RenderImGUI();
	}
}

void dae::GameObject::SetParent(GameObject* pParent, bool keepWorldPos)
{
	// If no new parent
	if (pParent == nullptr)
	{
		// Check whether to keep worldPos
		if (keepWorldPos)
		{
			SetLocalPosition(GetWorldPosition());
		}
		else
		{
			SetLocalPosition(0, 0, 0);
		}
	}
	// If has new parent
	else
	{
		// Check whether to keep worldPos
		if (keepWorldPos)
		{
			SetLocalPosition(GetWorldPosition() - pParent->GetWorldPosition());
		}
		else
		{
			SetLocalPosition(0, 0, 0);
		}

		SetPositionDirty();
	}

	// If had parent, remove from parent
	if (m_pParent)
	{
		m_pParent->RemoveChildFromList(shared_from_this());
	}

	// Assign new parent
	m_pParent = pParent;

	// If new parent exists, add to parent
	if (m_pParent)
	{
		m_pParent->AddChildToList(shared_from_this());
	}
}
dae::GameObject* dae::GameObject::GetParent() const
{
	return m_pParent;
}
const std::list<std::shared_ptr<dae::GameObject>>& dae::GameObject::GetChildren() const
{
	return m_pChildren;
}
bool dae::GameObject::RemoveChildFromScene(std::shared_ptr<dae::GameObject> pChild)
{
	// Check if Child is in list
	bool isInList{ false };
	for (const auto& currentChild : m_pChildren)
	{
		if (currentChild == pChild)
		{
			isInList = true;
			break;
		}
	}

	// If was in list, remove and return true
	if (isInList)
	{
		RemoveChildFromList(pChild, false);
		return true;
	}
	
	// Else, return false
	return false;
}

void dae::GameObject::SetWorldPosition(float x, float y, float z)
{
	m_WorldTransform.SetPosition(x, y, z);
	
	if (m_pParent == nullptr)
	{
		m_LocalTransform.SetPosition(x, y, z);
	}
	else
	{
		m_LocalTransform.SetPosition(glm::vec3{ x, y, z } - m_pParent->GetWorldPosition());
	}

	SetPositionDirty();
}
void dae::GameObject::SetWorldPosition(const glm::vec3& position)
{
	SetWorldPosition(position.x, position.y, position.z);
}
const glm::vec3& dae::GameObject::GetWorldPosition()
{
	if (m_PositionIsDirty)
	{
		UpdateWorldPosition();
	}

	return m_WorldTransform.GetPosition();
}

void dae::GameObject::SetLocalPosition(float x, float y, float z)
{
	m_LocalTransform.SetPosition(x, y, z);
	SetPositionDirty();
}
void dae::GameObject::SetLocalPosition(const glm::vec3 position)
{
	SetLocalPosition(position.x, position.y, position.z);
}
const glm::vec3& dae::GameObject::GetLocalPosition()
{
	return m_LocalTransform.GetPosition();
}

void dae::GameObject::UpdateWorldPosition()
{
	if (m_PositionIsDirty)
	{
		if (m_pParent == nullptr)
		{
			m_WorldTransform.SetPosition(m_LocalTransform.GetPosition());
		}
		else
		{
			m_WorldTransform.SetPosition(m_pParent->GetWorldPosition() + m_LocalTransform.GetPosition());
		}
	}

	//m_Transform.SetPosition(m_WorldPosition.x, m_WorldPosition.y, m_WorldPosition.z);
	m_PositionIsDirty = false;
}
void dae::GameObject::SetPositionDirty()
{
	// Set own position dirty
	m_PositionIsDirty = true;

	// Set children position dirty
	for (const auto& currentChild : m_pChildren)
	{
		currentChild->SetPositionDirty();
	}
}

void dae::GameObject::AddChildToList(std::shared_ptr<dae::GameObject> pChild)
{
	pChild->SetPositionDirty();
	m_pChildren.push_back(pChild);
}
void dae::GameObject::RemoveChildFromList(std::shared_ptr<dae::GameObject> pChild, bool setPositionDirty)
{
	if (setPositionDirty) pChild->SetPositionDirty();
	m_pChildren.erase(std::remove(m_pChildren.begin(), m_pChildren.end(), pChild), m_pChildren.end());
}

void dae::GameObject::DeleteMarkedComponents()
{
	// If there are components to delete
	if (m_ComponentsToDelete.size() > 0)
	{
		// Clear vector
		m_ComponentsToDelete.clear();
	}
}