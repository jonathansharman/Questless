//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include <unordered_map>

#include <boost/functional/hash.hpp>

#include "animation/ItemRenderer.h"
#include "animation/SpellTexturer.h"
#include "sdl/resources.h"
#include "items/magic/Gatestone.h"
#include "items/Scroll.h"

using namespace sdl;
using namespace units;

namespace questless
{
	void ItemRenderer::visit(Arrow const&)
	{
		static auto texture_handle = texture_manager().add("resources/textures/items/arrow.png");
		texture_manager()[texture_handle].draw(_position);
	}

	void ItemRenderer::visit(Bow const&)
	{
		static auto texture_handle = texture_manager().add("resources/textures/items/bow.png");
		texture_manager()[texture_handle].draw(_position);
	}

	void ItemRenderer::visit(Quarterstaff const&)
	{
		static auto texture_handle = texture_manager().add("resources/textures/items/quarterstaff.png");
		texture_manager()[texture_handle].draw(_position);
	}

	void ItemRenderer::visit(Quiver const&)
	{
		static auto texture_handle = texture_manager().add("resources/textures/items/quiver.png");
		texture_manager()[texture_handle].draw(_position);
	}

	void ItemRenderer::visit(Scroll const& scroll)
	{
		if (scroll.blank()) {
			static auto texture_handle = texture_manager().add("resources/textures/items/blank-scroll.png");
			texture_manager()[texture_handle].draw(_position);
		} else {
			static auto scroll_handle = texture_manager().add("resources/textures/items/uncharged-scroll.png");

			spell::Spell const& spell = scroll.spell();

			// Get the spell texture.
			SpellTexturer spell_texturer;
			spell.accept(spell_texturer);
			auto spell_handle = spell_texturer.texture_handle();

			// Static cache to hold previously computed texture handles.
			using handle_t = Handle<Texture>;
			static std::unordered_map<std::pair<handle_t, handle_t>, handle_t, boost::hash<std::pair<handle_t, handle_t>>> handle_cache;

			// The current scroll and spell handles together form the key for the cache.
			auto scroll_and_spell_handle = std::make_pair(scroll_handle, spell_handle);

			// Cache lookup.
			auto it = handle_cache.find(scroll_and_spell_handle); //! @todo Use initializer if here when available.
			if (it != handle_cache.end()) {
				// Cache hit. Return previously computed handle.
				texture_manager()[it->second].draw(_position);
			} else {
				// Cache miss. Create handle to texture formed by pasting the spell texture on top of the scroll texture.
				auto texture_handle = texture_manager().add_with_generator([=] {
					Texture& scroll_texture = texture_manager()[scroll_handle];
					auto texture = std::make_unique<Texture>(scroll_texture.width(), scroll_texture.height());
					texture->as_target([&] {
						scroll_texture.draw(ScreenSpace::Point{0, 0});
						texture_manager()[spell_handle].draw(ScreenSpace::Point{0, 0});
					});
					return texture;
				});

				// Cache and return the resulting handle.
				handle_cache.insert(std::make_pair(scroll_and_spell_handle, texture_handle));
				texture_manager()[texture_handle].draw(_position);
			}
		}
	}

	void ItemRenderer::visit(Gatestone const& gatestone)
	{
		if (gatestone.mana == 0.0) {
			static auto texture_handle = texture_manager().add("resources/textures/items/soul-gem-empty.png");
			texture_manager()[texture_handle].draw(_position);
		} else {
			switch (gatestone.color()) {
				case spell::Color::white:
				{
					static auto texture_handle = texture_manager().add("resources/textures/items/soul-gem-white.png");
					texture_manager()[texture_handle].draw(_position);
					break;
				}
				case spell::Color::black:
				{
					static auto texture_handle = texture_manager().add("resources/textures/items/soul-gem-black.png");
					texture_manager()[texture_handle].draw(_position);
					break;
				}
				case spell::Color::green:
				{
					static auto texture_handle = texture_manager().add("resources/textures/items/soul-gem-green.png");
					texture_manager()[texture_handle].draw(_position);
					break;
				}
				case spell::Color::red:
				{
					static auto texture_handle = texture_manager().add("resources/textures/items/soul-gem-red.png");
					texture_manager()[texture_handle].draw(_position);
					break;
				}
				case spell::Color::blue:
				{
					static auto texture_handle = texture_manager().add("resources/textures/items/soul-gem-blue.png");
					texture_manager()[texture_handle].draw(_position);
					break;
				}
				case spell::Color::yellow:
				{
					static auto texture_handle = texture_manager().add("resources/textures/items/soul-gem-yellow.png");
					texture_manager()[texture_handle].draw(_position);
					break;
				}
				case spell::Color::violet:
				{
					static auto texture_handle = texture_manager().add("resources/textures/items/soul-gem-violet.png");
					texture_manager()[texture_handle].draw(_position);
					break;
				}
				case spell::Color::orange:
				{
					static auto texture_handle = texture_manager().add("resources/textures/items/soul-gem-orange.png");
					texture_manager()[texture_handle].draw(_position);
					break;
				}
			}
		}
	}
}