#pragma once
#include "SceneManager.h"

namespace dae
{
	class GameObject;
	class Scene final
	{
	public:
		void Add(std::shared_ptr<GameObject> object);
		void Remove(GameObject* pObject);
		void RemoveAll();

		void Update(float deltaTime);
		void FixedUpdate(float deltaTime);
		void Render() const;
		void RenderImGUI();

		explicit Scene(const std::string& name);
		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 

		// Member variables
		// ----------------
		std::string m_Name;
		std::vector<std::shared_ptr<GameObject>> m_Objects{};
		std::vector<std::shared_ptr<GameObject>> m_ObjectsToDelete{};

		static unsigned int m_IdCounter; 

		// Member Functions
		// ----------------
		void DeleteMarkedObjects();
	};

}
