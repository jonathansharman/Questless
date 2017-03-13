/**
* @file    Window.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for Window, a simple wrapper class around SDL_Window.
*/

#include "sdl/Window.h"

#include "SDL_image.h"

using namespace units;

namespace sdl
{
	Window::Window
		( std::string title
		, std::string icon_filename
		, bool fullscreen
		, int width
		, int height
		, bool h_centered
		, bool v_centered
		, bool resizable
		, bool grab_input
		)
		: _title{title}, _icon_filename{icon_filename}
	{
		_sdl_window = SDL_CreateWindow
			( title.c_str()
			, h_centered ? SDL_WINDOWPOS_CENTERED : SDL_WINDOWPOS_UNDEFINED
			, v_centered ? SDL_WINDOWPOS_CENTERED : SDL_WINDOWPOS_UNDEFINED
			, width, height
			, (fullscreen ? SDL_WINDOW_FULLSCREEN : 0) | (resizable ? SDL_WINDOW_RESIZABLE : 0) | (grab_input ? SDL_WINDOW_INPUT_GRABBED : 0)
			);
		if (_sdl_window == nullptr) {
			throw std::runtime_error{"Failed to create window."};
		}
		SDL_SetWindowIcon(_sdl_window, IMG_Load(_icon_filename.c_str()));
	}

	Window::Window
		( std::string title
		, std::string icon_filename
		, bool fullscreen
		, int width
		, int height
		, int x
		, int y
		, bool resizable
		, bool grab_input
		)
		: _title{title}, _icon_filename{icon_filename}
	{
		_sdl_window = SDL_CreateWindow
			( title.c_str()
			, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED
			, width, height
			, (fullscreen ? SDL_WINDOW_FULLSCREEN : 0) | (resizable ? SDL_WINDOW_RESIZABLE : 0) | (grab_input ? SDL_WINDOW_INPUT_GRABBED : 0)
			);
		if (_sdl_window == nullptr) {
			throw std::runtime_error{"Failed to create window."};
		}
		SDL_SetWindowIcon(_sdl_window, IMG_Load("textures/icon.png"));
		SDL_SetWindowPosition(_sdl_window, x, y);
	}

	Window::~Window()
	{
		SDL_DestroyWindow(_sdl_window);
	}

	void Window::recreate()
	{
		// Cached width and height no longer valid.
		_width = boost::none;
		_height = boost::none;

		SDL_Window* old_sdl_window = _sdl_window;
		int x_init = x();
		int y_init = y();
		_sdl_window = SDL_CreateWindow
			( _title.c_str()
			, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED
			, width(), height()
			, flags()
			);
		SDL_SetWindowIcon(_sdl_window, IMG_Load(_icon_filename.c_str()));
		SDL_SetWindowPosition(_sdl_window, x_init, y_init);
		SDL_DestroyWindow(old_sdl_window);
	}

	bool Window::maximized() const
	{
		return (flags() & SDL_WINDOW_MAXIMIZED) != 0;
	}

	bool Window::fullscreen() const
	{
		return (flags() & SDL_WINDOW_FULLSCREEN) != 0;
	}

	bool Window::resizable() const
	{
		return (flags() & SDL_WINDOW_RESIZABLE) != 0;
	}

	ScreenPoint Window::position() const
	{
		ScreenPoint p;
		SDL_GetWindowPosition(_sdl_window, &p.x, &p.y);
		return p;
	}
	int Window::x() const
	{
		int x;
		SDL_GetWindowPosition(_sdl_window, &x, nullptr);
		return x;
	}
	int Window::y() const
	{
		int y;
		SDL_GetWindowPosition(_sdl_window, nullptr, &y);
		return y;
	}

	ScreenVector Window::resolution() const
	{
		return ScreenVector{width(), height()};
	}
	int Window::width() const
	{
		if (_width) {
			return *_width;
		} else {
			int width;
			SDL_GetWindowSize(_sdl_window, &width, nullptr);
			_width = width;
			return width;
		}
	}
	int Window::height() const
	{
		if (_height) {
			return *_height;
		} else {
			int height;
			SDL_GetWindowSize(_sdl_window, nullptr, &height);
			_height = height;
			return height;
		}
	}
	ScreenPoint Window::center() const
	{
		return ScreenPoint{width() / 2, height() / 2};
	}
}