#include "PointComponent.h"
#include "GameObject.h"

dae::PointComponent::PointComponent(dae::GameObject* pParentObject)
	: Component{ pParentObject }
{

}

void dae::PointComponent::SetCurrentPoint(int amount)
{
	// Set points
	m_CurrentPoints = amount;
}