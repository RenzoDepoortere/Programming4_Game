#pragma once
#include "Component.h"
#include <memory>
#include <unordered_map>
#include <iostream>
#include <typeindex>

#include "Transform.h"
#include "Utils.h"
#include "Follower.h"

namespace dae
{
	class GameObject final : public std::enable_shared_from_this<GameObject>
	{
	public:
		// Rule of Five
		GameObject() = default;
		~GameObject();

		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		// Updates and Render
		void Update(float deltaTime);
		void FixedUpdate(float deltaTime);
		void Render() const;
		void RenderImGUI();

		// Components
		template <typename T> T* AddComponent();
		template <typename T> T* GetComponent() const;
		template <typename T> void RemoveComponent();
		template <typename T> bool HasComponent() const;

		// Observers
		void AddObserver(Observer* pObserver);
		void RemoveObserver(Observer* pObserver);
		void NotifyObservers(const Event& event);

		// Ownership
		void SetParent(GameObject* pParent, bool keepWorldPos);
		GameObject* GetParent() const;
		std::pair<std::list<std::shared_ptr<GameObject>>::const_iterator, std::list<std::shared_ptr<GameObject>>::const_iterator> GetChildren() const;
		bool RemoveChildFromScene(std::shared_ptr<GameObject> pChild);

		// Positions
		void SetWorldPosition(float x, float y, float z);
		void SetWorldPosition(const glm::vec3& position);
		const glm::vec3& GetWorldPosition();

		void SetLocalPosition(float x, float y, float z);
		void SetLocalPosition(const glm::vec3 position);
		const glm::vec3& GetLocalPosition();

	private:
		// Member Variables
		// ----------------

		// Ownership
		GameObject* m_pParent{ nullptr };
		std::list<std::shared_ptr<GameObject>> m_pChildren{};

		// Position
		Transform m_WorldTransform{};
		Transform m_LocalTransform{};

		bool m_PositionIsDirty{ false };

		// Component
		std::unordered_map<std::type_index,std::shared_ptr<Component>> m_pComponents{};
		std::vector<std::type_index> m_ComponentsToDelete{};

		// Observers
		std::vector<Observer*> m_pObservers{};

		// Member Functions
		// ----------------
		void UpdateWorldPosition();
		void SetPositionDirty();

		void AddChildToList(std::shared_ptr<GameObject> pChild);
		void RemoveChildFromList(std::shared_ptr<GameObject> pChild, bool setPositionDirty = true);

		void DeleteMarkedComponents();
	};

	// TEMPLATED FUNCTIONS
	// -------------------
#pragma region TemplatedFunctions
	template<typename T>
	inline T* GameObject::AddComponent()
	{
		// Check if actually is a Component
		static_assert(std::is_base_of<Component, T>::value, "AddComponent failed: you have to add a class derived from Component");

		// Create Component and add to container
		std::shared_ptr<T> pCreatedComponent{ std::make_shared<T>(this) };
		m_pComponents[std::type_index(typeid(T))] = pCreatedComponent;

		// Return pointer
		return pCreatedComponent.get();
	}

	template <typename T>
	inline T* GameObject::GetComponent() const
	{
		std::shared_ptr<T> pComponent{ nullptr };

		// Try to find component and set pointer
		if (HasComponent<T>())
		{
			pComponent = std::dynamic_pointer_cast<T>(m_pComponents.at(std::type_index(typeid(T))));
		}
		// Return nullptr
		else
		{
			std::cout << "GetComponent failed: given component was not found" << std::endl;
			return nullptr;
		}

		// Return pointer
		return pComponent.get();
	}

	template <typename T>
	inline void dae::GameObject::RemoveComponent()
	{
		// Try to find component
		if (HasComponent<T>())
		{
			// Add to components to delete
			m_ComponentsToDelete.push_back(std::type_index(typeid(T)));

			// Delete from normal vector
			m_pComponents.erase(std::type_index(typeid(T)));
		}
		// Else report not found
		else
		{
			std::cout << "RemoveComponent failed: given component was not found" << std::endl;
		}
	}

	template <typename T>
	bool dae::GameObject::HasComponent() const
	{
		bool hasFoundComponent{ false };

		// Loop through components
		for (const auto& currentComponent : m_pComponents)
		{
			// If they have the same name
			if (currentComponent.first == std::type_index(typeid(T)))
			{
				// Component found
				hasFoundComponent = true;
				break;
			}
		}

		// Return bool
		return hasFoundComponent;
	}
#pragma endregion
	
}
