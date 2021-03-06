//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "point.hpp"
#include "vector.hpp"

#include <range/v3/algorithm/all_of.hpp>
#include <range/v3/view/zip.hpp>

namespace vecx {
	//! An n-dimensional rectangular volume.
	template <typename Quantity, std::size_t N>
	struct box {
		//! The type of scalars in this box type.
		using scalar_t = Quantity;

		//! The number of dimensions of this box type.
		static constexpr std::size_t n = N;

		//! The axis with index @p index.
		template <std::size_t Index>
		struct axis {
			static constexpr std::size_t index = Index;
			enum class align_t { near, mid, far } align;
		};

		//! The position of the minimal corner of this box.
		point<scalar_t, n> position{};

		//! The size or extent of this box.
		vector<scalar_t, n> size{};

		constexpr box() = default;
		constexpr box(box const&) = default;
		constexpr box(box&&) = default;

		//! @param position The position of the minimal corner of this box.
		//! @param size The size or extent of this box.
		constexpr box(point<scalar_t, n> position, vector<scalar_t, n> size)
			: position{position}, size{size}
		{}

		constexpr auto operator =(box const& other) -> box& {
			this->position = other.position;
			this->size = other.size;
			return *this;
		}
		constexpr auto operator =(box&& other) -> box& {
			this->position = std::move(other.position);
			this->size = std::move(other.size);
			return *this;
		}

		constexpr bool operator ==(box const& that) const = default;

		//! Creates a box of size @p size at @p position with alignment @p alignment.
		//! @param position The position of the box relative to its origin.
		//! @param size The size of the box.
		//! @param aligns A variadic list of alignments to apply to the box.
		template <typename Quantity, std::size_t N, typename... Aligns>
		static constexpr auto aligned(point<Quantity, N> position, vector<Quantity, N> size, Aligns... aligns) {
			auto result = box{position, size};
			auto apply_align = [&result](auto align) {
				using t = decltype(align);
				switch (align.align) {
					case t::near:
						break;
					case t::mid:
						result.position[t::axis] -= size[t::axis] / 2 - 1;
						break;
					case t::far:
						result.position[t::axis] -= size[t::axis] - 1;
						break;
				}
			};
			(apply_align(aligns), ...);
			return result;
		}

		//! The center point of this box.
		constexpr auto center(box const& box) { return box.position + box.size / 2; }

		//! The n-dimensional volume of this box.
		constexpr auto volume() const {
			return size.reduce(scalar_t(1), std::multiplies{});
		}

		//! Whether this box contains @p point.
		//! @note Lower bounds are inclusive, and upper bounds are exclusive.
		constexpr auto contains(point<scalar_t, n> const& point) -> bool {
			return ranges::all_of(
				ranges::views::zip(point.components, position.components, size.components),
				[](auto const& element) {
					auto [i_point, i_pos, i_size] = element;
					return i_pos <= i_point && i_point < i_pos + i_size;
				});
		}

		//! Extends this box, if necessary, to include the given point.
		auto extend(point<scalar_t, n> const& point) -> void {
			for (auto&& [i_point, i_pos, i_size] : ranges::views::zip(point.components, position.components, size.components)) {
				if (i_point < i_pos) {
					i_size += i_pos - i_point;
					i_pos = i_point;
				} else if (i_point > i_pos + i_size) {
					i_size = i_point - i_pos;
				}
			}
		}
	};

	template <typename Quantity, std::size_t N>
	box(point<Quantity, N>, vector<Quantity, N>)->box<Quantity, N>;
}

#include "doctest_wrapper/test.hpp"

TEST_CASE("[box] operations") {
	//! @todo Box tests.
}
