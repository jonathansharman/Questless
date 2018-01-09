//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <stdexcept>
#include <utility>

#include "sdl/input.hpp"
#include "sdl/renderable.hpp"
#include "utility/initializer.hpp"
#include "utility/reference.hpp"
#include "utility/utility.hpp"

namespace ql
{
	//! A menu consisting of linked pages of options.
	class digraph_menu : public sdl::renderable
	{
	public:
		//! @param min_width The minimum width of the menu, including margins. If necessary, the menu will stretch to fit its contents.
		//! @param min_height The minimum height of the menu, including margins. If necessary, the menu will stretch to fit its contents.
		digraph_menu(int min_width, int min_height);

		//! Adds a blank page to the menu.
		//! @param title The page title.
		//! @note Throws a std::invalid_argument if a page with the same title already exists.
		void add_page(std::string title);

		//! Adds a terminal option (one which doesn't lead to another page) to the specified page of the menu.
		//! @param page_title The title of the page to which the option is to be added.
		//! @param option_name The name of the option to be added.
		//! @note Throws a std::invalid_argument if the page does not exist.
		void add_option(std::string_view page_title, std::string option_name);

		//! Adds a nonterminal option (one which leads to another page) to the specified page of the menu.
		//! @param location_page_title The title of the page to which the option is to be added.
		//! @param option_name The name of the option to be added.
		//! @param target_page_title The title of the page to which the option links.
		void add_option(std::string_view location_page_title, std::string option_name, std::string_view target_page_title);

		//! Navigates to the specified page of the menu.
		//! @param title The title of the page.
		void set_page(std::string_view title);

		//! Selects the specified option on the specified page.
		//! @param page_title The title of the page whose option index is to be set.
		//! @param option_index The index of the option to be selected.
		void set_option(std::string_view page_title, int option_index);

		//! Removes all pages from the menu.
		void clear();

		//! Updates the menu. Should be called once per frame.
		void update();

		//! Gets all the terminal menu selections since the last call to poll_selections().
		//! @return A vector of pairs of strings, each representing a page name and option name. The pairs are ordered from least to most recently selected.
		std::vector<std::pair<std::string, std::string>> poll_selections();

		//! Draws the menu at @p origin.
		//! @param origin The origin point of the menu on the screen.
		//! @param horizontal_alignment The horizontal alignment of the menu relative to the origin point.
		//! @param vertical_alignment The vertical alignment of the menu relative to the origin point.
		void draw
			( units::screen_space::point origin
			, units::screen_space::h_align horizontal_alignment = units::screen_space::align_left
			, units::screen_space::v_align vertical_alignment = units::screen_space::align_top
			);
	private:
		struct page
		{
			struct option
			{
				std::string name;
				std::optional<int> target;

				//! @param name The name of the option.
				//! @param target The index of page to which this option links, or nullopt if the option is terminal.
				option(std::string name, std::optional<int> target) : name{std::move(name)}, target{target} {}
			};

			std::string title;
			std::vector<option> options;
			int option_index;

			page(std::string title) : title{std::move(title)}, option_index{0} {}
		};

		struct page_view
		{
			sdl::texture title_texture;
			mutable std::vector<sdl::texture> option_textures;

			page_view(sdl::texture title_texture, std::vector<sdl::texture> option_textures)
				: title_texture{std::move(title_texture)}, option_textures{std::move(option_textures)}
			{}
		};

		static constexpr int title_height = 60;
		static constexpr int option_height = 32;
		static constexpr int title_font_size = 48;
		static constexpr int option_font_size = 30;

		static constexpr units::colors::color _title_color = units::colors::black();
		static constexpr units::colors::color_vector _unselected_color_vector = units::colors::black_vector();
		static constexpr units::colors::color_vector _selected_color_vector = units::colors::red_vector();

		static sdl::sound_handle _hover_sound_handle, _select_sound_handle;

		static sdl::font_handle _title_font_handle, _option_font_handle;
		static sdl::texture_handle _ul_handle, _ur_handle, _dl_handle, _dr_handle, _u_handle, _d_handle, _l_handle, _r_handle, _tile_handle;

		static int _top_margin, _bottom_margin, _left_margin, _right_margin, _tile_width, _tile_height;

		friend class initializer<digraph_menu>;
		static initializer<digraph_menu> _initializer;
		static void initialize();

		std::vector<page> _pages;
		int _page_index;
		std::vector<std::pair<std::string, std::string>> _selections;

		std::vector<page_view> _page_views;
		units::screen_space::point _content_position;
		int _min_width, _min_height;
		int _content_width, _content_height;
		uptr<sdl::texture> _background;
		bool _render_is_current;

		//! Finds the first page with the given page title, if it exists.
		//! @param page_title The title of the page to be found.
		//! @return The index of the page or nullopt if no matching page was found.
		std::optional<int> find(std::string_view page_title);

		//! Renders or rerenders menu textures.
		void render();

		void refresh() final { _render_is_current = false; }
	};
}
