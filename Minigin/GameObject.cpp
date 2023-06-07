#include <string>
#include "GameObject.h"
#include "Scene.h"

#pragma region GameLoop
void dae::GameObject::Update(float deltaTime)
{
	if (m_IsActive)
	{
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

	// Delete marked objects
	DeleteMarkedComponents();
	DeleteMarkedChildren();
}
void dae::GameObject::FixedUpdate(float deltaTime)
{
	if (m_IsActive)
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
}
void dae::GameObject::Render() const
{
	if (m_Hide == false)
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
}
void dae::GameObject::RenderImGUI()
{
	if (m_Hide == false)
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
}
#pragma endregion

#pragma region ParentShip
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

		SetDirty();
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
void dae::GameObject::RemoveObject()
{
	// If scene is parent
	if (m_pParent == nullptr)
	{
		m_pScene->Remove(this);
	}
	// If gameObject is parent
	else
	{
		m_pParent->RemoveChildFromScene(shared_from_this());
	}
}
#pragma endregion

#pragma region Position
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

	SetDirty();
}
void dae::GameObject::SetWorldPosition(const glm::vec3& position)
{
	SetWorldPosition(position.x, position.y, position.z);
}
const glm::vec3& dae::GameObject::GetWorldPosition()
{
	if (m_IsDirty)
	{
		UpdateWorldTransform();
	}

	return m_WorldTransform.GetPosition();
}

void dae::GameObject::SetLocalPosition(float x, float y, float z)
{
	m_LocalTransform.SetPosition(x, y, z);
	SetDirty();
}
void dae::GameObject::SetLocalPosition(const glm::vec3 position)
{
	SetLocalPosition(position.x, position.y, position.z);
}
const glm::vec3& dae::GameObject::GetLocalPosition()
{
	return m_LocalTransform.GetPosition();
}

void dae::GameObject::UpdateWorldTransform()
{
	if (m_IsDirty)
	{
		if (m_pParent == nullptr)
		{
			m_WorldTransform.SetPosition(m_LocalTransform.GetPosition());
			m_WorldTransform.SetRotation(m_LocalTransform.GetRotation());
		}
		else
		{
			m_WorldTransform.SetPosition(m_pParent->GetWorldPosition() + m_LocalTransform.GetPosition());
			m_WorldTransform.SetRotation(m_pParent->GetRotation() + m_LocalTransform.GetRotation());
		}
	}

	m_IsDirty = false;
}
#pragma endregion

void dae::GameObject::SetRotation(float degrees)
{
	m_LocalTransform.SetRotation(degrees);
	SetDirty();
}
float dae::GameObject::GetRotation()
{
	if (m_IsDirty)
	{
		UpdateWorldTransform();
	}

	return m_WorldTransform.GetRotation();
}

void dae::GameObject::SetDirty()
{
	// Set own dirty
	m_IsDirty = true;

	// Set children dirty
	for (const auto& currentChild : m_pChildren)
	{
		currentChild->SetDirty();
	}
}

void dae::GameObject::AddChildToList(std::shared_ptr<dae::GameObject> pChild)
{
	pChild->SetDirty();
	m_pChildren.push_back(pChild);
}
void dae::GameObject::RemoveChildFromList(std::shared_ptr<dae::GameObject> pChild, bool setDirty)
{
	if (setDirty) pChild->SetDirty();

	// Add to children to delete
	m_ChildrenToDelete.push_back(pChild);
}

void dae::GameObject::DeleteMarkedComponents()
{
	// If there are components to delete
	if (m_ComponentsToDelete.size() > 0)
	{
		for (size_t idx{}; idx < m_ComponentsToDelete.size(); ++idx)
		{
			// Delete from normal vector
			m_pComponents.erase(m_ComponentsToDelete[idx]);
		}

		// Clear vector
		m_ComponentsToDelete.clear();
	}
}
void dae::GameObject::DeleteMarkedChildren()
{
	// If there are children to delete
	if (m_ChildrenToDelete.size() > 0)
	{
		for (size_t idx{}; idx < m_ChildrenToDelete.size(); ++idx)
		{
			// Delete from normal vector
			m_pChildren.erase(std::remove(m_pChildren.begin(), m_pChildren.end(), m_ChildrenToDelete[idx]), m_pChildren.end());
		}		

		// Clear vector
		m_ChildrenToDelete.clear();
	}
}