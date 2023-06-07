#pragma once
#include "Component.h"
#include "glm/vec2.hpp"
#include "Utils.h"
#include "Texture2D.h"

namespace dae
{
	class RenderTextureComponent : public Component
	{
	public:
		// Rule of five
		explicit RenderTextureComponent(dae::GameObject* pParentObject);
		virtual ~RenderTextureComponent() override = default;

		RenderTextureComponent(const RenderTextureComponent& other) = delete;
		RenderTextureComponent(RenderTextureComponent&& other) = delete;
		RenderTextureComponent& operator=(const RenderTextureComponent& other) = delete;
		RenderTextureComponent& operator=(RenderTextureComponent&& other) = delete;

		// Functionality
		virtual void Render() const override;
		void RenderManually(const glm::vec2& pos, const utils::Rect& srcRect) const; // Use this function when you want to have more control of the image, be sure to call it inside the render though

		void SetTexture(std::shared_ptr<Texture2D> pTexture);
		glm::ivec2 GetTextureSize() const { return m_pTexture2D->GetSize(); }

		void CenterTexture(bool centerTexture) { m_CenterTexture = centerTexture; }
		void SetManualRender(bool manualRender) { m_ManualRender = manualRender; }

	protected:
		bool GetCenteredTexture() const { return m_CenterTexture; }
		std::shared_ptr<Texture2D> GetTexture() const { return m_pTexture2D; }

	private:
		// Member variables
		// ----------------
		std::shared_ptr<Texture2D> m_pTexture2D{ nullptr };
		glm::ivec2 m_TextureSize{};

		bool m_CenterTexture{ false };
		bool m_ManualRender{ false };
	};
}