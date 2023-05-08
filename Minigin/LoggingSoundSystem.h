#pragma once
#include "SoundSystem.h"

class LoggingSoundSystem final : public SoundSystem
{
public:
	// Destructor
	~LoggingSoundSystem();

	// Functionality
	virtual void Play(unsigned int ID, float volume) override;

private:
	// Member variables
	SoundSystem* m_pRealSystem;
};

