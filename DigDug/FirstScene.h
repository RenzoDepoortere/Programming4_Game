#pragma once
#include <Singleton.h>
#include <vector>

namespace dae
{
	class GameObject;
	class Scene;
}
namespace grid
{
	class GridComponent;
}

class CharacterComponent;

class FirstScene final : public dae::Singleton<FirstScene>
{
public:
	// Destructor and rule of five
	~FirstScene();

	FirstScene(const FirstScene& other) = delete;
	FirstScene(FirstScene&& other) = delete;
	FirstScene& operator=(const FirstScene& other) = delete;
	FirstScene& operator=(FirstScene&& other) = delete;

	// Public Functions
	void CreateGameObjects(dae::Scene& scene);

private:
	friend class Singleton<FirstScene>;
	FirstScene() = default;

	// Member Variables
	// ----------------
	
	grid::GridComponent* m_pGrid{ nullptr };
	std::vector<CharacterComponent*> m_pCharacters{};

	unsigned long m_ControllerIdx{};

	// Member Functions
	// ----------------
	void BaseObjects(dae::Scene& scene);
	void Map(dae::Scene& scene);

	void MainCharacter(dae::Scene& scene);
	void Enemies(dae::Scene& scene);
};

