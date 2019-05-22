#include "rush/parser/parse.hpp"
#include "rush/parser/dump.hpp"
#include "cxxopts/cxxopts.hpp"

#include <vector>
#include <string>

int main(int argc, char const** argv) {
   auto options = cxxopts::Options { "crush", "Rush Compiler" };
   options.add_options()
      ("h,help", "Print usage information and exit.")
      ("dump-parse", "Display the results of syntax analysis as a hierachical AST structure.")
      ("sources", "The source to be compiled.", cxxopts::value<std::vector<std::string>>())
   ; options.parse_positional({ "sources" });
	options.positional_help("\"[ <expression> | <declaration> ]\"");

   auto results = options.parse(argc, argv);
   if (results.count("help") > 0) {
		std::cout << options.help() << std::endl;
		return 0;
	}

   if (results.count("sources") > 0) {
      auto sources = results["sources"].as<std::vector<std::string>>();
      if (results.count("dump-parse") > 0) {
         std::size_t page = 0;
         for (auto& src : sources) {

            if (page > 0) {
               std::cout << "===================================" << std::endl;
               std::cout << std::endl;
            }

            std::cout << "syntax analysis: ----------\n"  << std::endl;
            rush::dump(rush::parse(src));
            std::cout << std::endl;
            ++page;
         }
      }
   } else {
      std::cout << "No expression or declaration supplied." << std::endl;
      std::cout << options.help() << std::endl;
   }
}
