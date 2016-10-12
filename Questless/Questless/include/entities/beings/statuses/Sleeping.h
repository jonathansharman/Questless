/**
* @file    Sleeping.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Sleeping status class.
*/

#ifndef SLEEPING_H
#define SLEEPING_H

#include "entities/beings/statuses/Status.h"

namespace questless
{
	class Sleeping : public Status
	{
	public:
		/// @param magnitude How powerful the status modifier is.
		/// @param duration The number of turns remaining before the status modifier expires.
		/// @param source_id The ID of the being that caused the status modifier, if any.
		Sleeping(double magnitude, unsigned duration, boost::optional<BeingId> source_id = boost::none)
			: Status("Poisoned", duration, source_id), _magnitude{magnitude}
		{}

		virtual Type type() const { return Type::neutral; }

		void subupdate(Being& target) override;
	private:
		double _magnitude;
	};
}

#endif
