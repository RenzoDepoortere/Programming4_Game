#pragma once
#include "Component.h"
#include "glm/vec2.hpp"
#include "Utils.h"
#include "Texture2D.h"

namespace dae
{
	class RenderTextureComponent final : public Component
	{
	public:
		virtual void Render() const override;
		// Use this function when you want to have more control of the image, be sure to call it inside the render though
		void RenderManually(const utils::Rect& destRect, const utils::Rect& srcRect, float angle) const;

		void SetTexture(std::shared_ptr<Texture2D> pTexture);
		void CenterTexture(bool centerTexture) { m_CenterTexture = centerTexture; }
		glm::ivec2 GetTextureSize() const { return m_pTexture2D->GetSize(); }	// Todo: return half if this is centered

		void SetManualRender(bool manualRender) { m_ManualRender = manualRender; }

		explicit RenderTextureComponent(dae::GameObject* pParentObject);
		virtual ~RenderTextureComponent() override = default;

		RenderTextureComponent(const RenderTextureComponent& other) = delete;
		RenderTextureComponent(RenderTextureComponent&& other) = delete;
		RenderTextureComponent& operator=(const RenderTextureComponent& other) = delete;
		RenderTextureComponent& operator=(RenderTextureComponent&& other) = delete;

	private:
		std::shared_ptr<Texture2D> m_pTexture2D{ nullptr };
		glm::ivec2 m_TextureSize{};
		bool m_CenterTexture{ false };

		bool m_ManualRender{ false };
	};
}