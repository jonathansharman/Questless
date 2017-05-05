//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <string>
#include <functional>
#include <optional>

#include "Dialog.h"

namespace questless
{
	//! Retrieves a floating-point value from the player.
	class MagnitudeDialog : public Dialog
	{
	public:
		MagnitudeDialog
			( std::string title
			, std::string prompt
			, double default
			, std::optional<double> min
			, std::optional<double> max
			, std::function<void(std::optional<double>)> cont
			)
			: _title{std::move(title)}
			, _prompt{std::move(prompt)}
			, _magnitude{default}
			, _min{min}
			, _max{max}
			, _cont{std::move(cont)}
		{
			load_textures();
		}

		State update() final;

		void draw() const final;
	private:
		std::string _title;
		std::string _prompt;
		double _magnitude;
		std::optional<double> _min;
		std::optional<double> _max;
		Continuation<std::optional<double>> _cont;

		uptr<sdl::Texture> _txt_title;
		uptr<sdl::Texture> _txt_prompt;
		uptr<sdl::Texture> _txt_selector;

		void refresh() final { load_textures(); }

		void load_textures();
	};
}
