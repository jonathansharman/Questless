/**
* @file    Status.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Status class, which represents a status effect on a being.
*/

#ifndef STATUS_H
#define STATUS_H

#include <string>
#include <vector>
#include <memory>

#include "entities/beings/attributes/Attributes.h"

namespace questless
{
	class Being;

	class Status
	{
	public:
		enum class Type { debuff, neutral, buff };

		using modifiers_t = std::vector<std::unique_ptr<Attributes::Modifier>>;

		/// @return The name of the status modifier displayed to the player.
		std::string name() const { return _name; }

		/// @return The number of turns remaining before the status expires.
		int duration() const { return _duration; }

		/// @return The being that caused the status or null if none did.
		Being* source() const { return _source; }

		/// @return The type of the status: debuff, netural, or buff.
		virtual Type type() const = 0;

		/// @param name The name of the status modifier displayed to the player.
		/// @param duration The number of turns remaining before the status modifier expires.
		/// @param source The being that caused the status modifier, if any.
		Status(std::string name, int duration, Being* source = nullptr);

		/// @return The list of attribute modifiers associated with this status.
		virtual const modifiers_t& modifiers() const
		{
			// No modifiers by default.
			static modifiers_t modifiers;
			return modifiers;
		}

		/// Carries out any effects that should occur when the status is initially applied.
		/// @param target The being affected by the status.
		virtual void apply(Being& target);

		/// Carries out any effects that should occur each update cycle and decrements the duration of the status.
		/// @param target The being affected by the status.
		void update(Being& target);

		/// Carries out any effects that should occur when the status expires.
		/// @param target The being affected by the status.
		virtual void expire(Being& target);
	private:
		std::string _name;
		int _duration;
		Being* _source;

		virtual void subupdate(Being& target);
	};
}

#endif
