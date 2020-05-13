/*************************************************************************
* Copyright 2018 Reegan Troy Layzell
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*************************************************************************/
#include "rush/version.hpp"
#include "rush/core/source.hpp"
#include "commands/help.hpp"
#include "commands/dump.hpp"
#include "cxxopts/cxxopts.hpp"

#include <vector>
#include <string>
#include <algorithm>
#include <filesystem>

#define COMPILER_TITLE(VERSION) "Rush Compiler " VERSION

namespace fs = std::filesystem;
namespace cmds = crush::commands;

using source = rush::source;

std::string to_lower(std::string s) {
   std::transform(s.begin(), s.end(), s.begin(), ::tolower);
   return std::move(s);
}

int main(int argc, char const** argv) {
   auto opts = cxxopts::Options { "crush", COMPILER_TITLE(RUSH_VERSION) };
   opts.add_options()
      ("h,help", "Print usage information.")
      ("dump-lex", "Display the results of lexical analysis as a list of tokens.")
      ("dump-llvm-ir", "Display the results of generating LLVM IR.", cxxopts::value<bool>())
      ("dump-parse", "Display the results of syntax analysis as a hierachical AST structure.")
      ("paths", "Paths to the source to be compiled.", cxxopts::value<std::vector<std::string>>())
      ("version", "Print version information.");
   opts.parse_positional({ "paths" });
   opts.positional_help("\"files...\"");

   auto results = opts.parse(argc, argv);
   if (results["help"].count() > 0) {
      cmds::help(opts.help());
      return 0;
   }

   if (results["version"].count() > 0) {
      cmds::version();
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

         srcs.push_back(source::from_file(p));
      } catch (fs::filesystem_error& e) {
         auto msg = to_lower(e.code().message());
         std::cerr << "error: " << msg << ": '" << path << "'" << std::endl;
      }
   }

   if (srcs.size() == 0) {
      std::cout << "error: no input files.\n" << std::endl;
      return 1;
   }

   bool run_default = false;
   if (results["dump-lex"].count() > 0) { cmds::dump_lex(srcs); run_default = true; }
   if (results["dump-parse"].count() > 0) { cmds::dump_parse(srcs); run_default = true; }
   if (results["dump-llvm-ir"].count() > 0) { cmds::dump_llvm_ir(srcs); run_default = true; }
   if (!run_default) crush::commands::dump_llvm_ir(srcs); // default behavior.
   return 0;
}
