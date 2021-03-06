//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "utility.hpp"

#include <functional>
#include <memory>
#include <vector>

namespace ql {
	enum class handled { yes, no };

	//! Holds a modifiable list of handler callbacks that can be invoked in order.
	//!
	//! Differs from delegate in that handlers may return false to indicate not to continue invoking callbacks.
	template <typename... Args>
	struct event {
		//! The event handler type. Return type indicates whether the event was handled.
		using handler_t = sptr<std::function<bool(Args...)>>; //! @todo This shared_ptr and the one in delegate are smelly.

		//! Adds a new event handler @p f at the end of the handlers list.
		auto add(handler_t const& f) -> void {
			_handlers.push_back(f);
		}

		//! Removes any instances of the event handler @p f.
		auto remove(handler_t const& f) -> void {
			erase_if(_handlers, [&f](handler_t const& x) { return x == f; });
		}

		auto& operator+=(handler_t const& f) {
			add(f);
			return *this;
		}
		auto& operator-=(handler_t const& f) {
			remove(f);
			return *this;
		}

		//! Calls each event handler in turn, passing @p args to each.
		//! @return Whether the callback marked the event as handled.
		[[nodiscard]] auto operator()(Args... args) -> handled {
			for (auto& handler : _handlers) {
				if (!(*handler)(args...)) { return false; }
			}
			return true;
		}

	private:
		std::vector<handler_t> _handlers; //!< The list of callbacks, in the order they will be called.
	};
}
