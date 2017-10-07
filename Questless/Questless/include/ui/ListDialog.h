//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <string>
#include <vector>

#include "Dialog.h"
#include "units/ScreenSpace.h"
#include "sdl/resources.h"

namespace questless
{
	//! Retrieves the player's choice from a list of options.
	class ListDialog : public Dialog
	{
	public:
		ListDialog
			( units::ScreenSpace::Point origin
			, std::string title
			, std::vector<std::string> options
			, std::function<void(std::optional<int>)> cont
			)
			: _bounds{origin, units::ScreenSpace::Vector::zero()}
			, _title{std::move(title)}
			, _options{std::move(options)}
			, _cont{std::move(cont)}
			, _selection{0}
		{
			load_textures();
		}

		State update() final
		{
			if (sdl::input().presses(SDLK_BACKSPACE) || sdl::input().presses(SDLK_ESCAPE)) {
				return _cont(std::nullopt);
			}

			int const option_count = static_cast<int>(_options.size());

			if (!_options.empty()) {
				_selection -= sdl::input().presses(SDLK_UP);
				_selection += sdl::input().presses(SDLK_DOWN);
				_selection += sdl::input().scroll();
				_selection = _selection % option_count;
				_selection = _selection < 0 ? _selection + option_count : _selection;

				int const end = option_count <= 10 ? option_count : 10;
				for (int i = 0; i < end; ++i) {
					if (sdl::input().presses(sdl::Input::index_to_num_key(i))) {
						if (_selection == i) {
							return _cont(_selection);
						} else {
							_selection = i;
							break;
						}
					}
				}

				if (sdl::input().pressed(sdl::MouseButton::left) || sdl::input().presses(SDLK_RETURN) || sdl::input().presses(SDLK_SPACE)) {
					return _cont(_selection);
				}
			}

			return State::open;
		}

		void draw() const final
		{
			// Draw background.
			sdl::renderer().draw_box(_bounds, 1, units::colors::black(), units::colors::Color{1.0f, 0.75f, 0.6f, 1.0f});

			{ // Draw highlight.
				units::ScreenSpace::Box bounds
					{ units::ScreenSpace::Point{left(_bounds) + _x_padding, top(_bounds) + _y_padding + _title_height + _selection * _option_height}
					, units::ScreenSpace::Vector{width(_bounds) - 2 * _x_padding, _option_height}
					};
				sdl::renderer().draw_box(bounds, units::colors::white());
			}

			// Draw title.
			_txt_title->draw(units::ScreenSpace::Point{left(_bounds) + _x_padding, top(_bounds) + _y_padding});

			// Draw options.
			for (size_t i = 0; i < _options.size(); ++i) {
				_txt_options[i].draw(units::ScreenSpace::Point
					{ left(_bounds) + _x_padding
					, top(_bounds) + _y_padding + _title_height + static_cast<int>(i) * _option_height
					});
			}
		}
	private:
		static constexpr int _title_height = 40;
		static constexpr int _option_height = 20;
		static constexpr int _x_padding = 10;
		static constexpr int _y_padding = 10;

		units::ScreenSpace::Box _bounds;
		std::string _title;
		std::vector<std::string> _options;
		Continuation<std::optional<int>> _cont;

		uptr<sdl::Texture> _txt_title;
		std::vector<sdl::Texture> _txt_options;

		int _selection;

		void refresh() final { load_textures(); }

		void load_textures()
		{
			static auto list_option_font_handle = sdl::font_manager().add("resources/fonts/dumbledor1.ttf", 20);

			_txt_title = make_title(_title.c_str(), units::colors::black());
			width(_bounds) = _txt_title->width();
			_txt_options.clear();
			for (auto const& option : _options) {
				_txt_options.push_back(sdl::font_manager()[list_option_font_handle].render(option.c_str(), units::colors::black()));
				width(_bounds) = std::max(width(_bounds), _txt_options.back().width());
			}
			width(_bounds) += 2 * _x_padding;
			height(_bounds) = _title_height + static_cast<int>(_options.size() * _option_height) + 2 * _y_padding;

			// Confine bounds to window.
			if (left(_bounds) + width(_bounds) > sdl::window().width()) {
				left(_bounds) -= width(_bounds);
			}
			if (top(_bounds) + height(_bounds) > sdl::window().height()) {
				top(_bounds) -= height(_bounds);
			}
		}
	};
}
