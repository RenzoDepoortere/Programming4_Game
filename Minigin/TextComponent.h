#pragma once
#include <string>
#include <memory>

#include "SDL.h"
#include "Component.h"
#include "glm/vec2.hpp"

namespace dae
{
	class Font;
	class Texture2D;
	class GameObject;

	class TextComponent final : public Component
	{
	public:
		virtual void Update(float deltaTime) override;
		virtual void Render() const override;

		void SetText(const std::string& text);
		const std::string& GetText() const { return m_Text; }

		void SetFont(std::shared_ptr<Font> font);
		void SetColor(const SDL_Color& color);
		void SetTexture(std::shared_ptr<Texture2D> pTexture);
		glm::ivec2 GetTextureSize() const;

		explicit TextComponent(dae::GameObject* pParentObject);
		virtual ~TextComponent() override = default;

		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;
	private:

		bool m_NeedsUpdate;
		std::string m_Text{};
		std::shared_ptr<Font> m_Font{};
		SDL_Color m_TextColor{};

		std::shared_ptr<Texture2D> m_pTexture2D{ nullptr };
	};
}
