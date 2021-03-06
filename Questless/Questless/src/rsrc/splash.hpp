//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "splash_fwd.hpp"
#include "utility.hpp"

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

namespace ql::rsrc {
	//! Contains resources for the splash screen.
	struct splash {
		struct {
			sf::Texture logo = load<sf::Texture>("resources/textures/splash/logo.png");
			sf::Texture flame = load<sf::Texture>("resources/textures/splash/flame.png");
		} txtr;

		struct {
			sf::SoundBuffer flame = load<sf::SoundBuffer>("resources/sounds/splash.wav");
		} sfx;
	};
}
