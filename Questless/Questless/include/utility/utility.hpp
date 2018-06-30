//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief Miscellaneous utility functions.

#pragma once

#include <range/v3/all.hpp>

#include "reference.hpp"
#include "units/game_space.hpp"

namespace ql
{
	//! Conditionally removes elements from a container.
	//! @param container An iterable container.
	//! @param predicate A predicate over elements of the container. Elements for which the predicate is true are erased.
	template <typename Container, typename Predicate>
	void erase_if(Container& container, Predicate&& predicate)
	{
		container.erase
			( std::remove_if
				( container.begin()
				, container.end()
				, std::forward<Predicate>(predicate)
				)
			, container.end()
			);
	};

	//! Moves @p pointer into a new vector and returns it.
	template <typename T, typename U>
	auto make_uptr_vector(uptr<U> pointer)
	{
		std::vector<uptr<T>> modifiers;
		modifiers.push_back(std::move(pointer));
		return modifiers;
	}

	//! Moves the given pointers into a new vector and returns it.
	template <typename T, typename First, typename... Rest>
	auto make_uptr_vector(First first, Rest... rest)
	{
		std::vector<uptr<T>> modifiers;
		modifiers.push_back(std::move(first));
		auto rest = make_uptr_vector<T>(std::forward<Rest>(rest)...);
		modifiers.insert(modifiers.end(), std::make_move_iterator(rest.begin()), std::make_move_iterator(rest.end()));
		return modifiers;
	}

	//! Generates a new vector by applying @p f to each element of @p source_vector.
	//! @param source_vector The vector from which to generate the transformed vector.
	//! @param f A function from the source vector element type to the result vector element type.
	//! @return The transformed vector.
	template <typename T, typename UnaryFunction>
	auto transformed_vector(std::vector<T> source_vector, UnaryFunction&& f)
	{
		std::vector<std::invoke_result_t<UnaryFunction, T>> result;
		for (T const& element : source_vector) {
			result.push_back(f(element));
		};
		return result;
	}

	//! Returns a transform view to statically cast elements to type @p T.
	template <typename T>
	static auto cast_transform()
	{
		return ranges::view::transform([](auto arg) { return static_cast<T>(arg); });
	}

	//! @todo Why do the following lambda overload helpers not work?

	//template<typename... Ts>
	//struct overloaded : Ts... {
	//	overloaded(Ts... t) : Ts(t)... {};
	//	using typename Ts::operator()...;
	//};

	//template<typename... Ts>
	//overloaded(Ts...) -> overloaded<Ts...>;
}
