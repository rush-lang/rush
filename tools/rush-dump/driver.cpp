#include "rush/lexer/lex.hpp"
#include "rush/parser/parse.hpp"
#include "cxxopts/cxxopts.hpp"

#include <vector>
#include <string>

void dump_lex(std::string src) {
	auto lxa = rush::lex(std::move(src));
	for (auto& tok : lxa) {
		std::cout << rush::debug_string(tok) << std::endl;
	}
}

int main(int argc, char const** argv) {
	auto options = cxxopts::Options { "rush-dump", "Diagnostic tool" };
	options.add_options()
		("h,help", "Print usage information and exit.")
		("l,lex", "Display results of the lexical analysis.")
		("p,parse", "Display results of the syntactical analysis.")
		("s,symbols", "Display symbol table information.")
		("srcs", "Display symbol table information.", cxxopts::value<std::vector<std::string>>())
	; options.parse_positional({ "srcs" });

	auto results = options.parse(argc, argv);

	if (results.count("srcs") > 0) {
		auto srcs = results["srcs"].as<std::vector<std::string>>();
		std::size_t page = 0;
		for (auto& src : srcs) {

			if (page > 0) {
				std::cout << "===================================" << std::endl;
				std::cout << std::endl;
			}

			if (results.count("lex") > 0) {
				std::cout << "lexical analysis: ----------\n" << std::endl;
				dump_lex(src);
				std::cout << std::endl;
			}

			if (results.count("parse") > 0) {
				std::cout << "syntax analysis: ----------\n"  << std::endl;
				rush::dump(src);
				std::cout << std::endl;
			}

			++page;
		}
	}

	return 0;
}