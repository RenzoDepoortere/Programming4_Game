#pragma once
#include "Component.h"
#include <array>

namespace dae
{
	class TrashTheCache final : public Component
	{
	public:
		explicit TrashTheCache(dae::GameObject* pParentObject);
		virtual ~TrashTheCache() override = default;

		TrashTheCache(const TrashTheCache& other) = delete;
		TrashTheCache(TrashTheCache&& other) = delete;
		TrashTheCache& operator=(const TrashTheCache& other) = delete;
		TrashTheCache& operator=(TrashTheCache&& other) = delete;

		virtual void RenderImGUI() override;

	private:
		// Member-Variables
		// ----------------
		static const int m_NrSteps{ 11 };
		std::array<float, m_NrSteps> m_RecordedTimesFirstExercise{};
		std::array<float, m_NrSteps> m_RecordedTimesSecondExercise{};
		std::array<float, m_NrSteps> m_RecordedTimesSecondAltExercise{};
		std::array<float, m_NrSteps> m_StepValues{};

		int m_NrSamples_1{};
		int m_NrSamples_2{};
		int m_MinSamples{};

		// Member-Functions
		// ----------------
		void CalculateFirstExercise();
		void CalculateSecondExercise();
		void CalculateSecondAltExercise();

		// Test-Classes
		// ------------
		struct Transform
		{
			float matrix[16] =
			{
				1,0,0,0,
				0,1,0,0,
				0,0,1,0,
				0,0,0,1
			};
		};
		class GameObject3D
		{
		public:
			Transform Transform;
			int ID;
		};
		class GameObject3DAlt
		{
		public:
			Transform* Transform;
			int ID;
		};
	};
}