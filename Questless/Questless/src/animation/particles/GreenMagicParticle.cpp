/**
* @file    GreenMagicParticle.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Green magic particle implementation.
*/

#include "animation/particles/GreenMagicParticle.h"
#include "Game.h"
#include "sdl-wrappers/resources.h"

using namespace sdl;
using namespace units;

namespace questless
{
	void GreenMagicParticle::subupdate()
	{
		if (uniform(0.0, 1.0) < _inflection_probability) {
			_turning_right = !_turning_right;
		}

		if (_turning_right) {
			_velocity.step().rotate(GameRadians{-1.0 * _turn_rate * Game::frame_duration});
		} else {
			_velocity.step().rotate(GameRadians{_turn_rate * Game::frame_duration});
		}
	}

	const Texture& GreenMagicParticle::texture() const
	{
		static bool first_call = true;
		static Handle<Texture> handle;
		if (first_call) {
			handle = texture_manager().add([] {
				return std::make_unique<Texture>("resources/textures/particles/magic/green.png", renderer(), SDL_BLENDMODE_BLEND);
			});
			first_call = false;
		}
		return texture_manager()[handle];
	}
}