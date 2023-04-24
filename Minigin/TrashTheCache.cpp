#include <chrono>
#include <list>
#include <algorithm>
#include <numeric>
#include <iostream>

#include "TrashTheCache.h"
#include "imgui.h"

dae::TrashTheCache::TrashTheCache(dae::GameObject* pParentObject)
	: Component{ pParentObject }
{
	// Defaults
    m_NrSamples_1 = 10;
    m_NrSamples_2 = 10;
	m_MinSamples = 3;   // 3 because of the pop-front and pop-back that's being used

    // Init arrays
    int index{};
    for (int stepSize{ 1 }; stepSize <= 1024; stepSize *= 2)
    {
        m_StepValues[index] = static_cast<float>(stepSize);
        ++index;
    }
}

void dae::TrashTheCache::RenderImGUI()
{
    // Exercise 1
    // ----------
    ImGui::Begin("Exercise 1");

    // Button
	ImGui::InputInt("# samples", &m_NrSamples_1);
	if (ImGui::Button("Trash the cache"))
	{
        CalculateFirstExercise();
	}

    // Plot
    ImGui::PlotLines
    (
        "Exercise 1",                                               // Name
        m_RecordedTimesFirstExercise.data(),                        // Data
        static_cast<int>(m_RecordedTimesFirstExercise.size()),      // #Data
        0,                                                          // Offset from start
        "ms",                                                       // Graph title
        FLT_MAX,                                                    // Min y-value
        FLT_MAX,                                                    // Max y-value, putting both on FLT_MAX scales the graph automatically
        ImVec2(0,100),                                              // Graph size
        sizeof(float)                                               // Size of data
    );

    ImGui::End();

    // Exercise 2
    // ----------
    ImGui::Begin("Exercise 2");

    // Button 1
    ImGui::InputInt("# samples", &m_NrSamples_2);
    if (ImGui::Button("Trash the cache with GameObject3D"))
    {
        CalculateSecondExercise();
    }

    // Plot 1
    ImGui::PlotLines
    (
        "GameObject3D",                                               
        m_RecordedTimesSecondExercise.data(),                        
        static_cast<int>(m_RecordedTimesSecondExercise.size()),
        0,                                                         
        "ms",                                                      
        FLT_MAX,                                                    
        FLT_MAX,                                                   
        ImVec2(0, 100),                                          
        sizeof(float)                                              
    );

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();

    // Button 2
    if (ImGui::Button("Trash the cache with GameObject3DAlt"))
    {
        CalculateSecondAltExercise();
    }

    // Plot 2
    ImGui::PlotLines
    (
        "GameObject3DAlt",
        m_RecordedTimesSecondAltExercise.data(),
        static_cast<int>(m_RecordedTimesSecondAltExercise.size()),
        0,
        "ms",
        FLT_MAX,
        FLT_MAX,
        ImVec2(0, 100),
        sizeof(float)
    );

    ImGui::End();
}

void dae::TrashTheCache::CalculateFirstExercise()
{
    using namespace std::chrono;

    // Clamp nrSamples
    if (m_NrSamples_1 < m_MinSamples)
    {
        std::cout << "Next time, please use numbers bigger than 2" << std::endl;
        m_NrSamples_1 = m_MinSamples;
    }

    // Create array
    const int arraySize{ 1'000'000 };
    int* intArray{ new int[arraySize] {} };

    // Go through steps
    int index{};
    for (int stepSize{ 1 }; stepSize <= 1024; stepSize *= 2)
    {
        std::list<int> recordedTimes{};

        // Do n calculations
        for (int calculatedNr{}; calculatedNr < m_NrSamples_1; ++calculatedNr)
        {
            // Start Clock
            const auto start = high_resolution_clock::now();

            // Do operations
            for (int i{}; i < arraySize; i += stepSize)
            {
                intArray[i] *= 2;
            }

            // End Clock
            const auto end = high_resolution_clock::now();
            const auto deltaTime = duration_cast<microseconds>(end - start).count();

            // Store recordedTime
            recordedTimes.push_back(static_cast<int>(deltaTime));
        }


        // Sort
        recordedTimes.sort();

        // Remove front and back
        recordedTimes.pop_front();
        recordedTimes.pop_back();

        // Average
        const int accumTime{ std::accumulate(recordedTimes.cbegin(), recordedTimes.cend(), 0) };
        const float average{ accumTime / static_cast<float>(recordedTimes.size()) };
        
        // Store result
        m_RecordedTimesFirstExercise[index] = average;
        ++index;
    }

    // Delete array
    delete[] intArray;
    intArray = nullptr;
}
void dae::TrashTheCache::CalculateSecondExercise()
{
    using namespace std::chrono;

    // Clamp nrSamples
    if (m_NrSamples_2 < m_MinSamples)
    {
        std::cout << "Next time, please use numbers bigger than 2" << std::endl;
        m_NrSamples_2 = m_MinSamples;
    }

    // Create array
    const int arraySize{ 1'000'000 };
    GameObject3D* intArray{ new GameObject3D[arraySize] {} };

    // Go through steps
    int index{};
    for (int stepSize{ 1 }; stepSize <= 1024; stepSize *= 2)
    {
        std::list<int> recordedTimes{};

        // Do n calculations
        for (int calculatedNr{}; calculatedNr < m_NrSamples_2; ++calculatedNr)
        {
            // Start Clock
            const auto start = high_resolution_clock::now();

            // Do operations
            for (int i{}; i < arraySize; i += stepSize)
            {
                intArray[i].ID = 1;
            }

            // End Clock
            const auto end = high_resolution_clock::now();
            const auto deltaTime = duration_cast<microseconds>(end - start).count();

            // Store recordedTime
            recordedTimes.push_back(static_cast<int>(deltaTime));
        }

        // Sort
        recordedTimes.sort();

        // Remove front and back
        recordedTimes.pop_front();
        recordedTimes.pop_back();

        // Average
        const int accumTime{ std::accumulate(recordedTimes.cbegin(), recordedTimes.cend(), 0) };
        const float average{ accumTime / static_cast<float>(recordedTimes.size()) };

        // Store result
        m_RecordedTimesSecondExercise[index] = average;
        ++index;
    }

    // Delete array
    delete[] intArray;
    intArray = nullptr;
}
void dae::TrashTheCache::CalculateSecondAltExercise()
{
    using namespace std::chrono;

    // Clamp nrSamples
    if (m_NrSamples_2 < m_MinSamples)
    {
        std::cout << "Next time, please use numbers bigger than 2" << std::endl;
        m_NrSamples_2 = m_MinSamples;
    }

    // Create array
    const int arraySize{ 1'000'000 };
    GameObject3DAlt* intArray{ new GameObject3DAlt[arraySize] {} };

    // Go through steps
    int index{};
    for (int stepSize{ 1 }; stepSize <= 1024; stepSize *= 2)
    {
        std::list<int> recordedTimes{};

        // Do n calculations
        for (int calculatedNr{}; calculatedNr < m_NrSamples_2; ++calculatedNr)
        {
            // Start Clock
            const auto start = high_resolution_clock::now();

            // Do operations
            for (int i{}; i < arraySize; i += stepSize)
            {
                intArray[i].ID = 1;
            }

            // End Clock
            const auto end = high_resolution_clock::now();
            const auto deltaTime = duration_cast<microseconds>(end - start).count();

            // Store recordedTime
            recordedTimes.push_back(static_cast<int>(deltaTime));
        }

        // Sort
        recordedTimes.sort();

        // Remove front and back
        recordedTimes.pop_front();
        recordedTimes.pop_back();

        // Average
        const int accumTime{ std::accumulate(recordedTimes.cbegin(), recordedTimes.cend(), 0) };
        const float average{ accumTime / static_cast<float>(recordedTimes.size()) };

        // Store result
        m_RecordedTimesSecondAltExercise[index] = average;
        ++index;
    }

    // Delete array
    delete[] intArray;
    intArray = nullptr;
}