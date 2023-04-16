#include "Component.h"
#include "GameObject.h"


Component::Component(dae::GameObject* pParentObject)
{
	m_pParentObject = pParentObject;
}

void Component::Update(float)
{

}
void Component::FixedUpdate(float)
{

}
void Component::Render() const
{

}
void Component::RenderImGUI()
{

}

dae::GameObject* Component::GetGameObject() const
{
	return m_pParentObject;
}