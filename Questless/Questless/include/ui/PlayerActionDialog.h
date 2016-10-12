/**
* @file    PlayerActionDialog.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the PlayerActionDialog class.
*/

#ifndef PLAYER_ACTION_DIALOG_H
#define PLAYER_ACTION_DIALOG_H

#include <boost/optional.hpp>

#include "Dialog.h"
#include "HUDController.h"
#include "utility/utility.h"

namespace questless
{
	class PlayerActionDialog : public Dialog
	{
	public:
		struct Choice
		{
			enum class Type { idle, move, use };

			Type type;
			int data;

			Choice(Type type = Type::idle, int data = 0) : type{type}, data{data} {}
		};

		PlayerActionDialog(HUDController& hud, std::function<void(Choice)> cont) : _hud{hud}, _cont{std::move(cont)} {}

		/// Updates the menu state based on input.
		/// @param input User input used to update the dialog.
		void update(const sdl::Input& input) override;

		/// Draws the dialog to the screen.
		/// @param window The window.
		void draw(const sdl::Window&) override
		{}
	private:
		HUDController& _hud;
		std::function<void(Choice)> _cont;

		void refresh() override {}
	};
}

#endif