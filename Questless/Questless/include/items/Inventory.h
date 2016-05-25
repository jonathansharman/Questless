/**
* @file    Inventory.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Inventory class.
*/

#ifndef INVENTORY_H
#define INVENTORY_H

#include <vector>
#include <array>
#include <string>

#include "Item.h"
#include "utility/Stream.h"

namespace questless
{
	class Inventory
	{
	public:
		struct Page
		{
			static constexpr size_t rows = 8;
			static constexpr size_t columns = 12;

			std::string label = "";

			std::array<std::array<Item::ptr, columns>, rows> items;
		};

		struct Coords
		{
			size_t page;
			size_t row;
			size_t column;

			Coords() : page{0}, row{0}, column{0} {}
			Coords(size_t page, size_t row, size_t column) : page{page}, row{row}, column{column} {}
		};

		/// Adds the given item to the inventory in the first empty item slot.
		/// @param item An item to add to the inventory.
		void add(Item::ptr item);

		/// @return A stream of items in the inventory, ordered by page and then coordinates.
		Stream<Item*> items();

		/// @return The number of item pages in the inventory.
		size_t pages() const { return _pages.size(); }

		/// @return The inventory page at the given index.
		Page& page(int page_idx) { return _pages[page_idx]; }

		/// @return The item at the given inventory coordinates or nullptr if none present.
		/// @param coords Inventory coordinates (page, row, and column) of the requested item.
		Item* get(Coords coords) { return _pages[coords.page].items[coords.row][coords.column].get(); }

		/// @return The item at the given inventory coordinates or nullptr if none present.
		/// @param coords Inventory coordinates (page, row, and column) of the requested item.
		Item* operator [](Coords coords) { return get(coords); }
	private:
		std::vector<Page> _pages;

		/// @todo Is the convenience of streams really worth this monstrosity? Also, is the implementation even lazy?
		Stream<Item*> items(size_t page_start);
		Stream<Item*> items(size_t page, size_t row_start);
		Stream<Item*> items(size_t page, size_t row_start, size_t column_start);
	};
}

#endif
