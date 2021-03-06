//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "unit.hpp"

#include <gcem.hpp>

#include <compare>
#include <cmath>
#include <utility>

namespace cancel {
	//! Represents a quantity with the given data representation and unit.
	template <typename Rep, typename Unit>
	struct quantity {
		//! The data representation of this quantity type.
		using rep = Rep;
		//! The unit of this quantity type.
		using unit = Unit;

		//! The underlying value of this quantity.
		rep data{};

		//! Constructs a quantity from the default value of the underlying type.
		constexpr quantity() noexcept(std::is_nothrow_default_constructible_v<rep>) = default;

		//! Explicitly copy-constructs a quantity from a value.
		explicit constexpr quantity(rep const& value) noexcept(std::is_nothrow_copy_constructible_v<rep>) : data{value} {}

		//! Explicitly move-constructs a quantity from a value.
		explicit constexpr quantity(rep&& value) noexcept(std::is_nothrow_move_constructible_v<rep>) : data{std::move(value)} {}

		//! Copy-constructs a quantity from an object of compatible representation and the same unit.
		template <typename ThatRep, typename ThatUnit>
		constexpr quantity(quantity<ThatRep, ThatUnit> const& that) noexcept(std::is_nothrow_copy_constructible_v<rep>) : data{that.data} {
			static_assert(detail::is_same_unit_v<unit, ThatUnit>, "Attempted to initialize from quantity of a different unit.");
		};

		//! Move-constructs a quantity from an object of compatible representation and the same unit.
		template <typename ThatRep, typename ThatUnit>
		constexpr quantity(quantity<ThatRep, ThatUnit>&& that) noexcept(std::is_nothrow_move_constructible_v<rep>) : data{std::move(that.data)} {
			static_assert(detail::is_same_unit_v<unit, ThatUnit>, "Attempted to initialize from quantity of a different unit.");
		};

		//! Copy-assign from an object of compatible representation and the same unit.
		template <typename ThatRep, typename ThatUnit>
		constexpr auto& operator =(quantity<ThatRep, ThatUnit> const& that) noexcept(std::is_nothrow_copy_assignable_v<rep>) {
			static_assert(detail::is_same_unit_v<unit, ThatUnit>, "Attempted to assign from a quantity of a different unit.");
			data = rep{that.data};
			return *this;
		};

		//! Move-assign from an object of compatible representation and the same unit.
		template <typename ThatRep, typename ThatUnit>
		constexpr auto& operator =(quantity<ThatRep, ThatUnit>&& that) noexcept(std::is_nothrow_move_assignable_v<rep>) {
			static_assert(detail::is_same_unit_v<unit, ThatUnit>, "Attempted to assign from a quantity of a different unit.");
			data = rep{std::move(that.data)};
			return *this;
		};

		//! Add a quantity of compatible representation and the same unit.
		template <typename ThatRep, typename ThatUnit>
		constexpr auto& operator +=(quantity<ThatRep, ThatUnit> const& that) {
			static_assert(detail::is_same_unit_v<unit, ThatUnit>, "Attempted to add a quantity of a different unit.");
			data += rep{that.data};
			return *this;
		}

		//! Subtract a quantity of compatible representation and the same unit.
		template <typename ThatRep, typename ThatUnit>
		constexpr auto& operator -=(quantity<ThatRep, ThatUnit> const& that) {
			static_assert(detail::is_same_unit_v<unit, ThatUnit>, "Attempted to subtract a quantity of a different unit.");
			data -= rep{that.data};
			return *this;
		}

		//! Multiply by a scalar.
		template <typename K>
		constexpr auto& operator *=(K const& k) {
			data *= rep{k};
			return *this;
		}
		//! Multiply by a unitless quantity.
		template <typename K>
		constexpr auto& operator *=(quantity<K, unit_t<>> const& k) {
			data *= rep{k.data};
			return *this;
		}

		//! Divide by a scalar.
		template <typename K>
		constexpr auto& operator /=(K const& k) {
			data /= rep{k};
			return *this;
		}
		//! Divide by a unitless quantity.
		template <typename K>
		constexpr auto& operator /=(quantity<K, unit_t<>> const& k) {
			data /= rep{k.data};
			return *this;
		}

		//! Mod by a scalar.
		template <typename K>
		constexpr auto& operator %=(K const& k) {
			data %= rep{k};
			return *this;
		}

		//! Mod by a quantity of compatible representation and the same unit.
		template <typename ThatRep, typename ThatUnit>
		constexpr auto& operator %=(quantity<ThatRep, ThatUnit> const& that) const {
			static_assert(detail::is_same_unit_v<unit, ThatUnit>, "Attempted to mod by a quantity of a different unit.");
			data %= rep{that.data};
			return *this;
		}

		//! Pre-increment.
		constexpr auto& operator ++() {
			++data;
			return *this;
		}

		//! Post-increment.
		constexpr auto operator ++(int) { return quantity<rep, unit>{data++}; }

		//! Pre-decrement.
		constexpr auto& operator --() {
			--data;
			return *this;
		}

		//! Post-decrement.
		constexpr auto operator --(int) { return quantity<rep, unit>{data--}; }

		//! The negation of this quantity.
		constexpr auto operator -() const { return quantity<rep, unit>{-data}; }

		//! Implicit conversion to the representation type, if the quantity is unitless.
		constexpr operator rep() const noexcept requires detail::is_same_unit_v<unit, unit_t<>> { return data; }
	};

	//! A unit-less quantity with representation @p Rep.
	template <typename Rep>
	using unitless = quantity<Rep, unit_t<>>;

	template <typename Rep1, typename Unit1, typename Rep2, typename Unit2>
	constexpr auto operator ==(quantity<Rep1, Unit1> const& q1, quantity<Rep2, Unit2> const& q2) {
		static_assert(std::is_convertible_v<Rep1, Rep2> || std::is_convertible_v<Rep2, Rep1>, "Attempted to compare quantities with incompatible representations.");
		static_assert(detail::is_same_unit_v<Unit1, Unit2>, "Attempted to compare quantities of different units.");
		using common_rep = std::common_type_t<Rep1, Rep2>;
		return static_cast<common_rep>(q1.data) == static_cast<common_rep>(q2.data);
	}
	template <typename Rep1, typename Unit1, typename Rep2, typename Unit2>
	constexpr auto operator <=>(quantity<Rep1, Unit1> const& q1, quantity<Rep2, Unit2> const& q2) {
		static_assert(std::is_convertible_v<Rep1, Rep2> || std::is_convertible_v<Rep2, Rep1>, "Attempted to compare quantities with incompatible representations.");
		static_assert(detail::is_same_unit_v<Unit1, Unit2>, "Attempted to compare quantities of different units.");
		using common_rep = std::common_type_t<Rep1, Rep2>;
		return static_cast<common_rep>(q1.data) <=> static_cast<common_rep>(q2.data);
	}

	//! Add quantities of compatible representation and the same unit.
	template <typename Rep1, typename Unit1, typename Rep2, typename Unit2>
	constexpr auto operator +(quantity<Rep1, Unit1> const& q1, quantity<Rep2, Unit2> const& q2) {
		static_assert(std::is_convertible_v<Rep1, Rep2> || std::is_convertible_v<Rep2, Rep1>, "Attempted to add quantities with incompatible representations.");
		static_assert(detail::is_same_unit_v<Unit2, Unit2>, "Attempted to add quantities of different units.");
		using result_rep = std::common_type_t<Rep1, Rep2>;
		return quantity<result_rep, Unit1>{static_cast<result_rep>(q1.data) + static_cast<result_rep>(q2.data)};
	}

	//! Subtract quantities of compatible representation and the same unit.
	template <typename Rep1, typename Unit1, typename Rep2, typename Unit2>
	constexpr auto operator -(quantity<Rep1, Unit1> const& q1, quantity<Rep2, Unit2> const& q2) {
		static_assert(std::is_convertible_v<Rep1, Rep2> || std::is_convertible_v<Rep2, Rep1>, "Attempted to subtract quantities with incompatible representations.");
		static_assert(detail::is_same_unit_v<Unit2, Unit2>, "Attempted to subtract quantities of different units.");
		using result_rep = std::common_type_t<Rep1, Rep2>;
		return quantity<result_rep, Unit1>{static_cast<result_rep>(q1.data) - static_cast<result_rep>(q2.data)};
	}

	//! Multiply a quantity by a scalar.
	template <typename Rep, typename Unit, typename K>
	constexpr auto operator *(quantity<Rep, Unit> const& q, K const& k) requires std::is_convertible_v<K, Rep> {
		using result_rep = std::common_type_t<Rep, K>;
		return quantity<result_rep, Unit>{static_cast<result_rep>(q.data)* static_cast<result_rep>(k)};
	}
	//! Multiply a scalar by a quantity.
	template <typename K, typename Rep, typename Unit>
	constexpr auto operator *(K const& k, quantity<Rep, Unit> const& q) requires std::is_convertible_v<K, Rep> {
		using result_rep = std::common_type_t<Rep, K>;
		return quantity<result_rep, Unit>{static_cast<result_rep>(k)* static_cast<result_rep>(q.data)};
	}

	//! Divide a quantity by a scalar.
	template <typename Rep, typename Unit, typename K>
	constexpr auto operator /(quantity<Rep, Unit> const& q, K const& k) requires std::is_convertible_v<K, Rep> {
		using result_rep = std::common_type_t<Rep, K>;
		return quantity<result_rep, Unit>{static_cast<result_rep>(q.data) / static_cast<result_rep>(k)};
	}
	//! Divide a scalar by a quantity.
	template <typename K, typename Rep, typename Unit>
	constexpr auto operator /(K const& k, quantity<Rep, Unit> const& q) requires std::is_convertible_v<K, Rep> {
		using result_rep = std::common_type_t<Rep, K>;
		return quantity<result_rep, inverse_t<Unit>>{static_cast<result_rep>(k) / static_cast<result_rep>(q.data)};
	}

	//! Mod a quantity by a scalar.
	template <typename Rep, typename Unit, typename K>
	constexpr auto operator %(quantity<Rep, Unit> const& q, K const& k) requires std::is_convertible_v<K, Rep> {
		using result_rep = std::common_type_t<Rep, K>;
		return quantity<result_rep, Unit>{static_cast<result_rep>(q.data) % static_cast<result_rep>(k)};
	}

	//! Mod quantities of compatible representation and the same unit.
	template <typename Rep1, typename Unit1, typename Rep2, typename Unit2>
	constexpr auto operator %(quantity<Rep1, Unit1> const& q1, quantity<Rep2, Unit2> const& q2) {
		static_assert(std::is_convertible_v<Rep1, Rep2> || std::is_convertible_v<Rep2, Rep1>, "Attempted to mod quantities with incompatible representations.");
		using result_rep = std::common_type_t<Rep1, Rep2>;
		return quantity<result_rep, Unit1>{static_cast<result_rep>(q1.data) % static_cast<result_rep>(q2.data)};
	}

	//! Multiply quantities of compatible representation and the same unit.
	template <typename Rep1, typename Unit1, typename Rep2, typename Unit2>
	constexpr auto operator *(quantity<Rep1, Unit1> const& q1, quantity<Rep2, Unit2> const& q2) {
		static_assert(std::is_convertible_v<Rep1, Rep2> || std::is_convertible_v<Rep2, Rep1>, "Attempted to multiply quantities with incompatible representations.");
		using result_rep = std::common_type_t<Rep1, Rep2>;
		return quantity<result_rep, product_t<Unit1, Unit2>>{static_cast<result_rep>(q1.data)* static_cast<result_rep>(q2.data)};
	}

	//! Divide quantities of compatible representation and the same unit.
	template <typename Rep1, typename Unit1, typename Rep2, typename Unit2>
	constexpr auto operator /(quantity<Rep1, Unit1> const& q1, quantity<Rep2, Unit2> const& q2) {
		static_assert(std::is_convertible_v<Rep1, Rep2> || std::is_convertible_v<Rep2, Rep1>, "Attempted to divide quantities with incompatible representations.");
		using result_rep = std::common_type_t<Rep1, Rep2>;
		return quantity<result_rep, quotient_t<Unit1, Unit2>>{static_cast<result_rep>(q1.data) / static_cast<result_rep>(q2.data)};
	}

	//! The square root of a quantity.
	//! @note The powers of the quantity's units must be divisible by two.
	//! @note Uses ADL to find a @p sqrt() function for the quantity's representation, defaulting to @p gcem::sqrt().
	template <typename Rep, typename Unit>
	constexpr auto sqrt(quantity<Rep, Unit> const& q) {
		using gcem::sqrt;
		return quantity<Rep, cancel::exponential_t<1, 2, Unit>>{static_cast<Rep>(sqrt(q.data))};
	}

	//! The floor of a quantity.
	//! @note Uses ADL to find a @p floor() function for the quantity's representation, defaulting to @p gcem::floor().
	template<typename Rep, typename Unit>
	constexpr auto floor(quantity<Rep, Unit> const& q) {
		using gcem::floor;
		return quantity<Rep, Unit>{static_cast<Rep>(floor(q.data))};
	}

	//! The ceiling of a quantity.
	//! @note Uses ADL to find a @p ceil() function for the quantity's representation, defaulting to @p gcem::ceil().
	template<typename Rep, typename Unit>
	constexpr auto ceil(quantity<Rep, Unit> const& q) {
		using gcem::ceil;
		return quantity<Rep, Unit>{static_cast<Rep>(ceil(q.data))};
	}

	//! Floating-point mod a quantity by a scalar.
	template<typename Rep, typename Unit, typename K>
	constexpr auto fmod(quantity<Rep, Unit> const& q, K const& k) requires std::is_floating_point_v<Rep>&& std::is_convertible_v<K, Rep> {
		using result_rep = std::common_type_t<Rep, K>;
		return quantity<result_rep, Unit>{static_cast<result_rep>(std::fmodl(q.data, k))};
	}

	//! Floating-point mod quantities of compatible representation and the same unit.
	template<typename Rep1, typename Unit1, typename Rep2, typename Unit2>
	constexpr auto fmod(quantity<Rep1, Unit1> const& q1, quantity<Rep2, Unit2> const& q2) requires std::is_floating_point_v<std::common_type_t<Rep1, Rep2>> {
		static_assert(std::is_convertible_v<Rep1, Rep2> || std::is_convertible_v<Rep2, Rep1>, "Attempted to mod quantities with incompatible representations.");
		using result_rep = std::common_type_t<Rep1, Rep2>;
		return quantity<result_rep, Unit1>{static_cast<result_rep>(fmodl(q1.data, q2.data))};
	}

	//! Computes the ratio of quantities @p q1 and @p q2 after casting their representations to @p RatioType.
	//! Useful when the ratio between two quantities needs to use a wider type than @p Rep.
	template <typename RatioType, typename Rep, typename Unit>
	constexpr auto ratio(cancel::quantity<Rep, Unit> const& q1, cancel::quantity<Rep, Unit> const& q2) {
		return static_cast<RatioType>(q1.data) / static_cast<RatioType>(q2.data);
	}

	//! Scales @p q by @p scale via static casting. Useful for scaling operations that require narrowing conversions.
	template <typename ScaleType, typename Rep, typename Unit>
	constexpr auto scale(cancel::quantity<Rep, Unit> const& q, ScaleType const& scale) {
		using common = std::common_type_t<ScaleType, Rep>;
		return cancel::quantity<Rep, Unit>(static_cast<Rep>(static_cast<common>(q.data)* static_cast<common>(scale)));
	}

	//! Scales @p q by @p scale via static casting. Useful for scaling operations that require narrowing conversions.
	template <typename ScaleType, typename Rep, typename Unit>
	constexpr auto scale(ScaleType const& scale, cancel::quantity<Rep, Unit> const& q) {
		using common = std::common_type_t<ScaleType, Rep>;
		return cancel::quantity<Rep, Unit>(static_cast<Rep>(static_cast<common>(scale)* static_cast<common>(q.data)));
	}

	//! Cast a quantity to a quantity with the same unit but a different representation.
	template <typename ToQuantity, typename FromRep, typename FromUnit>
	constexpr auto quantity_cast(quantity<FromRep, FromUnit> const& q) {
		static_assert(detail::is_same_unit_v<FromUnit, typename ToQuantity::unit>, "Cannot cast quantity to a different unit.");
		return ToQuantity{static_cast<typename ToQuantity::rep>(q.data)};
	}

	namespace detail {
		//! Extend product to quantities.
		template <typename Rep1, typename Unit1, typename Rep2, typename Unit2>
		struct detail::product<quantity<Rep1, Unit1>, quantity<Rep2, Unit2>> {
			static_assert(std::is_convertible_v<Rep1, Rep2> || std::is_convertible_v<Rep2, Rep1>, "Quantity types have incompatible representations.");
			using type = quantity<std::common_type_t<Rep1, Rep2>, product_t<Unit1, Unit2>>;
		};

		//! Extend inverse to quantities.
		template <typename Rep, typename Unit>
		struct detail::inverse<quantity<Rep, Unit>> {
			using type = quantity<Rep, detail::inverse_t<Unit>>;
		};

		//! Extend quotient to quantities.
		template <typename Rep1, typename Unit1, typename Rep2, typename Unit2>
		struct detail::quotient<quantity<Rep1, Unit1>, quantity<Rep2, Unit2>> {
			static_assert(std::is_convertible_v<Rep1, Rep2> || std::is_convertible_v<Rep2, Rep1>, "Quantity types have incompatible representations.");
			using type = quantity<std::common_type_t<Rep1, Rep2>, quotient_t<Unit1, Unit2>>;
		};
	}
}
