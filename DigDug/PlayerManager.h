#pragma once
#include "Component.h"

#include <vector>

class CharacterComponent;

class PlayerManager final : public Component 
{
public:
	// Rule of five
	explicit PlayerManager(dae::GameObject* pParentObject);
	virtual ~PlayerManager() override = default;

	PlayerManager(const PlayerManager& other) = delete;
	PlayerManager(PlayerManager&& other) = delete;
	PlayerManager& operator=(const PlayerManager& other) = delete;
	PlayerManager& operator=(PlayerManager&& other) = delete;

	// Functionality
	void Reset();

	void SetNrPlayers(int nrPlayers) { m_NrPlayers = nrPlayers; }
	void SpawnPlayers();

	const std::vector<CharacterComponent*>& GetCharacters() const { return m_pCharacters; }

private:
	// Member variables
	// ----------------
	int m_NrPlayers{};
	std::vector<CharacterComponent*> m_pCharacters{};

	// Member functions
	// ----------------
	void Spawn(int index);
};

