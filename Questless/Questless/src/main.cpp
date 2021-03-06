//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.
//! @brief Runs an instance of the game Questless.

#include "doctest_wrapper/impl.hpp"

#include "game.hpp"

auto main(int argc, char* argv[]) -> int {
	int result = 0;

#ifdef _DEBUG
	doctest::Context context;
	context.setOption("no-breaks", false);
	context.setOption("success", false);
	context.applyCommandLine(argc, argv);
	result = context.run();
#else
	// Command-line arguments unused in release build. Supress unused variable warnings.
	(void)argc;
	(void)argv;
#endif

	ql::game{false}.run();

	return result;
}
