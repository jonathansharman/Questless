//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "entity.hpp"

namespace ql {
	//! Makes @p id an entity: a being or object that can exist in the world.
	void make_entity(ent id, location location, transparency transparency) {
		reg.assign<ql::location>(id, location);
		reg.assign<ql::transparency>(id, transparency);
	}
}
