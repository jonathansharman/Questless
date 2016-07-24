/**
* @file    HUDController.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the HUDController, which manages the HUD model and view and handles input to the HUD.
*/

#ifndef HUD_CONTROLLER_H
#define HUD_CONTROLLER_H

#include <array>
using std::array;

#include "HUDModel.h"
#include "HUDView.h"
#include "items/Item.h"
#include "sdl-wrappers/Input.h"
using sdl::Input;

namespace questless
{
	class Being;

	class HUDController
	{
	public:
		HUDController(Game& game, const Window& window);

		/// @return A constant reference to the hotbar.
		const auto& hotbar() { return _hud.hotbar; }

		void update(const Input& input);

		void HUDController::draw() { _view.draw(_hud); }

		void player_id(Entity::id_t player_id);
	private:
		Game& _game;
		HUDModel _hud;
		HUDView _view;
	};
}

#endif