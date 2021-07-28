#ifndef PANNEL_HPP
#define PANNEL_HPP

#include "extern.hpp"
#include "ui_element.hpp"
#include "layout.hpp"

class Pannel final : public UI_Element {
private:
    std::unique_ptr<Layout> layout { nullptr };

public:
    // Pannel() = default;
    Pannel(std::shared_ptr<GraphicsContext> context, Layout* layout, SDL_Rect rect, SDL_Texture* a_texture = nullptr)
        :   UI_Element(context, rect, a_texture), layout(layout) {}
    Pannel(std::shared_ptr<GraphicsContext> context, Layout* layout, SDL_Color color = SDL_Color({0, 0, 0, 0}), SDL_Rect rect = SDL_Rect({0, 0, 0, 0}))
        : UI_Element(context, rect, color), layout(layout) {}

    // Implement constructors using l/r-value references and std::unique_ptr as well for user personal preference

    // Copy constructor
    Pannel(const Pannel& pannel) = delete;

    // Move constructor
	Pannel(Pannel&& pannel) noexcept
		: UI_Element(pannel.context, pannel.rect, pannel.texture)
	{
        layout = std::move(pannel.layout);
        pannel.texture = nullptr;
	}
 
	// Copy assignment
	Pannel& operator=(const Pannel& pannel) = delete;
 
	// Move assignment
	Pannel& operator=(Pannel&& pannel) noexcept
	{
		if (&pannel == this)
			return *this;

        if(texture)
            SDL_DestroyTexture(texture);
        texture = nullptr;
 
		layout = std::move(pannel.layout); 
        context = pannel.context;
        texture = pannel.texture;
		return *this;
	}

    ~Pannel() {
        if(texture)
            SDL_DestroyTexture(texture);
        texture = nullptr;
    }

public:
    void updateSize() override;

    void render() override;
};

#endif