#pragma once
#include <Singleton.h>

namespace dae
{
	class GameObject;
	class Scene;
}

namespace grid
{
	class GridComponent;
}

class FirstScene final : public dae::Singleton<FirstScene>
{
public:
	// Destructor and rule of five
	~FirstScene() = default;

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
	unsigned long m_ControllerIdx{};

	// Member Functions
	// ----------------
	void Grid(dae::Scene& scene);
	void MainCharacter(dae::Scene& scene);
};

