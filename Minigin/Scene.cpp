#include "Scene.h"
#include "GameObject.h"

using namespace dae;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name)
	: m_Name(name)
{}

Scene::~Scene() = default;

void Scene::Add(std::shared_ptr<GameObject> object)
{
	// Add gameObject
	assert(object->GetParent() == nullptr && "You can only add an object without a parent to the scene");
	object->SetScene(this);
	m_Objects.emplace_back(std::move(object));
}
void Scene::Remove(GameObject* pObject)
{
	// Get object
	// ----------
	auto samePointer = [pObject](const std::shared_ptr<GameObject>& sharedPointer)
	{
		return sharedPointer.get() == pObject;
	};
	auto sharedObjectIt = std::find_if(m_Objects.begin(), m_Objects.end(), samePointer);

	// Return if not found
	if (sharedObjectIt == m_Objects.end()) return;

	// Add to vectors
	// --------------

	// Add to objects to delete
	m_ObjectsToDelete.push_back(*sharedObjectIt);

	// Delete from normal vector
	m_Objects.erase(std::remove(m_Objects.begin(), m_Objects.end(), *sharedObjectIt), m_Objects.end());
}
void Scene::RemoveAll()
{
	// Add to objects to delete
	m_ObjectsToDelete = m_Objects;

	// Clear normal vector
	m_Objects.clear();
}

void Scene::Update(float deltaTime)
{
	// Delete marked objects
	DeleteMarkedObjects();

	// Update objects
	for(auto& object : m_Objects)
	{
		object->Update(deltaTime);
	}
}
void Scene::FixedUpdate(float deltaTime)
{
	// FixedUpdate objects
	for (auto& object : m_Objects)
	{
		object->FixedUpdate(deltaTime);
	}
}
void Scene::Render() const
{
	// Render objects
	for (const auto& object : m_Objects)
	{
		object->Render();
	}
}
void Scene::RenderImGUI()
{
	// Render objects
	for (const auto& object : m_Objects)
	{
		object->RenderImGUI();
	}
}

void Scene::DeleteMarkedObjects()
{
	// If there are objects to delete
	if (m_ObjectsToDelete.size() > 0)
	{
		// Clear vector
		m_ObjectsToDelete.clear();
	}
}