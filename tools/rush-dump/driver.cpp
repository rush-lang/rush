#include "rush/lexer/lex.hpp"
#include "rush/lexer/dump.hpp"
#include "rush/parser/parse.hpp"
#include "rush/parser/dump.hpp"
#include "cxxopts/cxxopts.hpp"

#include <vector>
#include <string>


int main(int argc, char const** argv) {
	auto options = cxxopts::Options { "rush-dump", "Rush Dump - Diagnostic tool" };
	options.add_options()
		("h,help", "Print usage information and exit.")
		("l,lex", "Display results of the lexical analysis.")
		("p,parse", "Display results of the syntactical analysis.")
		("s,symbols", "Display symbol table information.")
		("srcs", "Display symbol table information.", cxxopts::value<std::vector<std::string>>())
	; options.parse_positional({ "srcs" });
	options.positional_help("\"[ <expression> | <declaration> ]\"");

	auto results = options.parse(argc, argv);

	if (results.count("help") > 0) {
		std::cout << options.help() << std::endl;
		return 0;
	}

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
				rush::dump(rush::lex(src));
				std::cout << std::endl;
			}

			if (results.count("parse") > 0) {
				std::cout << "syntax analysis: ----------\n"  << std::endl;
				rush::dump(rush::parse(src));
				std::cout << std::endl;
			}

			++page;
		}
	} else {
		std::cout << "No expression or declaration supplied." << std::endl;
		std::cout << options.help() << std::endl;
	}

	return 0;
}