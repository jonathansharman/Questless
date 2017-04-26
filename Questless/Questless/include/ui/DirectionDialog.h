//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <optional>

#include "Dialog.h"
#include "world/coordinates.h"

namespace questless
{
	//! Retrieves a direction from the player.
	class DirectionDialog : public Dialog
	{
	public:
		DirectionDialog
			( std::string title
			, std::string prompt
			, std::function<void(std::optional<RegionTileCoords::Direction>)> cont
			)
			: _title{std::move(title)}
			, _prompt{std::move(prompt)}
			, _cont{std::move(cont)}
		{
			load_textures();
		}

		State update() override;

		void draw() const override;
	private:
		std::string _title;
		std::string _prompt;
		Continuation<std::optional<RegionTileCoords::Direction>> _cont;

		uptr<sdl::Texture> _txt_title;
		uptr<sdl::Texture> _txt_prompt;

		void refresh() override { load_textures(); }

		void load_textures();

	};
}
