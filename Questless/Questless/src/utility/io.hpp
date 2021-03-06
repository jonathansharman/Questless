//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.
//! @brief Utility functions for I/O tasks.

#pragma once

#include <string>

namespace ql {
	//! The contents, as a string, of the file with filepath @p filepath.
	auto contents_of_file(char const* filepath) -> std::string;
}
