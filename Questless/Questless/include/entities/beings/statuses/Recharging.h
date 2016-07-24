/**
* @file    Recharging.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Recharging status class.
*/

#ifndef RECHARGING_H
#define RECHARGING_H

#include "entities/beings/statuses/Status.h"

namespace questless
{
	class Recharging : public Status
	{
	public:
		/// @param magnitude How powerful the status modifier is.
		/// @param duration The number of turns remaining before the status modifier expires.
		/// @param source_id The ID of the being that caused the status modifier, if any.
		Recharging(double magnitude, unsigned duration, optional<BeingId> source_id = nullopt)
			: Status("Recharging", duration, source_id), _magnitude{magnitude}
		{}

		virtual Type type() const { return Type::buff; }

		void subupdate(Being& target) override;
	private:
		double _magnitude;
	};
}

#endif
