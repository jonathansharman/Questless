//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/visitor-pattern.h"

namespace questless
{
	class Arrow;
	class Bow;
	class Gatestone;
	class Quarterstaff;
	class Quiver;
	class Scroll;

	using ItemSubtypeList = type_list::of_t
		< Arrow
		, Bow
		, Gatestone
		, Quarterstaff
		, Quiver
		, Scroll
		>;

	DEFINE_VISITORS(Item, ItemSubtypeList)
}
