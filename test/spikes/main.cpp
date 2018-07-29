#include "rush/lexer/lexer.hpp"
#include "rush/parser/parser.hpp"
#include <iostream>

namespace rush {
	template <typename Fn>
	void source_context(std::string src, Fn fn) {

	}
}

int main() {
	rush::source_context({
		"stdio.rs"
		"stdmath.rs",
		"main.rs",
	}, [](auto& src) {
		auto lxa = rush::lex(src.text());
		auto ast = rush::parse(lxa);
	});
}