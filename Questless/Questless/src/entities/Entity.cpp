/**
* @file    Entity.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the Entity class.
*/

#include "entities/Entity.h"
#include "world/Region.h"
#include "world/Section.h"

namespace questless
{
	Entity::Entity(std::istream& in)
	{
		in >> _id;
		in >> _coords.q;
		in >> _coords.r;
		in >> _coords.s;
	}

	void Entity::serialize(std::ostream& out) const
	{
		out << static_cast<unsigned>(entity_class()) << ' ' << entity_id() << ' ' << _coords.q << ' ' << _coords.r << ' ' << _coords.s << ' ';
	}
}
