#include "rush/core/source.hpp"
#include "commands/help.hpp"
#include "commands/dump.hpp"
#include "cxxopts/cxxopts.hpp"

#include <vector>
#include <string>
#include <algorithm>
#include <filesystem>


namespace fs = std::filesystem;
namespace cmds = crush::commands;

using source = rush::source;

std::string to_lower(std::string s) {
   std::transform(s.begin(), s.end(), s.begin(), ::tolower);
   return std::move(s);
}

int main(int argc, char const** argv) {
   auto opts = cxxopts::Options { "crush", "Rush Compiler" };
   opts.add_options()
      ("h,help", "Print usage information and exit.")
      ("dump-parse", "Display the results of syntax analysis as a hierachical AST structure.")
      ("dump-llvm-ir", "Display the results of generating LLVM IR.", cxxopts::value<bool>())
      ("paths", "Paths to the source to be compiled.", cxxopts::value<std::vector<std::string>>());
   opts.parse_positional({ "paths" });
   opts.positional_help("\"paths..\"");

   auto results = opts.parse(argc, argv);
   if (results["help"].count() > 0) {
      cmds::help(opts.help());
      return 0;
   }

   auto paths = results["paths"].count() > 0
      ? results["paths"].as<std::vector<std::string>>()
      : std::vector<std::string> {};
   std::vector<source> srcs;
   for (auto& path : paths) {
      try {
         auto p = fs::canonical(path);
         if (fs::is_directory(p)) {
            std::cout << "warn: directory " << p << " ignored." << std::endl;
            continue;
         }

         srcs.push_back(source::file(p));
      } catch (fs::filesystem_error& e) {
         auto msg = to_lower(e.code().message());
         std::cerr << "error: " << msg << ": '" << path << "'" << std::endl;
      }
   }

   if (srcs.size() == 0) {
      std::cout << "error: no input files.\n" << std::endl;
      return 1;
   }

   if (results["dump-parse"].count() > 0) { cmds::dump_parse(srcs); return 0; }
   if (results["dump-llvm-ir"].count() > 0) { cmds::dump_llvm_ir(srcs); return 0; }

   // default behavior.
   crush::commands::dump_llvm_ir(srcs);
   return 0;
}
