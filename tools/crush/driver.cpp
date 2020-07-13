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
#include "fmt/format.h"
#include "rush/core/source.hpp"
#include "commands/help.hpp"
#include "commands/dump.hpp"
#include "cxxopts/cxxopts.hpp"
#include "tclr/tclr.hpp"

#include <vector>
#include <string>
#include <algorithm>
#include <filesystem>
#include <utility>

#define COMPILER_TITLE(VERSION) "Rush Compiler " VERSION

namespace tc = tclr;
namespace fs = std::filesystem;
namespace cmds = crush::commands;

using source = rush::source;

std::string to_lower(std::string s) {
   std::transform(s.begin(), s.end(), s.begin(), ::tolower);
   return std::move(s);
}

void log_error(std::string msg) {
   std::cerr << "crush: " << tc::fg("error", tc::red) << ": " << msg << std::endl;
}

void log_warning(std::string msg) {
   std::cerr << "crush: " << tc::fg("warning", tc::yellow) << ": " << msg << std::endl;
}

template <typename... Ts>
inline void log_error(const char* str, Ts&&... args) {
   log_error(fmt::format(str, std::forward<Ts>(args)...));
}

template <typename... Ts>
inline void log_warning(const char* str, Ts&&... args) {
   log_warning(fmt::format(str, std::forward<Ts>(args)...));
}

int main(int argc, char** argv) {
   auto opts = cxxopts::Options { "crush", COMPILER_TITLE(RUSH_VERSION) };
   opts.add_options()
      ("h,help", "Print usage information.")
      ("version", "Print version information.")
      ("verbose", "Print verbose information.")
      ("dump-lex", "Display the results of lexical analysis as a list of tokens.")
      ("dump-parse", "Display the results of syntax analysis as a hierachical AST structure.")
      ("dump-llvm-ir", "Display the results of generating LLVM IR.")
      ("indent", "Indentation style [ spaces | tabs | auto ] and width separated by a colon \':\'.", cxxopts::value<std::string>(), "style[:n]")
      ("files", "Source files to be compiled.", cxxopts::value<std::vector<std::string>>())
      ("fno-core", "Compile without the core module.")
      ("fno-builtin-aliases", "Compile without the built-in type aliases.");
   opts.parse_positional({ "files" });
   opts.positional_help("\"files...\"");
   opts.allow_unrecognised_options();

   try {
      auto results = opts.parse(argc, argv);
      if (results["help"].count() > 0) {
         cmds::help(opts.help());
         return 0;
      }

      if (results["version"].count() > 0) {
         cmds::version();
         return 0;
      }

      auto files = results["files"].count() > 0
         ? results["files"].as<std::vector<std::string>>()
         : std::vector<std::string> {};
      std::vector<source> srcs;
      for (auto& file : files) {
         try {
            auto p = fs::canonical(file);
            if (fs::is_directory(p)) {
               log_warning("directory {} ignored.", p.string());
               // std::cout << "crush: warn: directory " << p << " ignored." << std::endl;
               continue;
            }

            srcs.push_back(source::from_file(p));
         } catch (fs::filesystem_error& e) {
            auto msg = to_lower(e.code().message());
            log_error("{}: '{}'", msg, file);
            // std::cerr << "crush: " << tc::fg("error", tc::red) << msg << ": '" << file << "'" << std::endl;
         }
      }

      if (srcs.size() == 0) {
         log_error("no input files.\n");
         return 1;
      }

      bool run_default = true;
      if (results["dump-lex"].count() > 0) { cmds::dump_lex(srcs, results); run_default = false; }
      if (results["dump-parse"].count() > 0) { cmds::dump_parse(srcs, results); run_default = false; }
      if (results["dump-llvm-ir"].count() > 0) { cmds::dump_llvm_ir(srcs, results); run_default = false; }
      if (run_default) crush::commands::dump_llvm_ir(srcs, results); // default behavior.
      return 0;
   }
   catch (cxxopts::option_not_exists_exception const& e) {
      log_error("{}.\n", to_lower(e.what()));
      // std::cout << "crush: " << tc::fg("error", tc::red) << ": " << to_lower(e.what()) << ".\n" << std::endl;
      return 1;
   }
}
