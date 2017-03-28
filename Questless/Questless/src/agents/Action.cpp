/**
* @file    Action.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*/

#include "agents/Action.h"
#include "entities/beings/Being.h"
#include "agents/Agent.h"

using namespace std;

namespace questless
{
	function<bool(RegionTileCoords)> Action::tile_in_range_predicate(Being& actor, int range)
	{
		return [&actor, range](RegionTileCoords tile_coords) {
			return actor.coords.distance_to(tile_coords) <= range;
		};
	}

	function<bool(Being&)> Action::being_in_range_predicate(Being& actor, int range)
	{
		return [&actor, range](Being& being) {
			return actor.coords.distance_to(being.coords) <= range;
		};
	}
}