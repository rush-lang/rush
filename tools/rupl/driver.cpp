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
#include "tclr/tclr.hpp"
#include "cxxopts.hpp"

#include "rush/version.hpp"
#include "rush/core/source.hpp"
#include "rush/lexer/lex.hpp"

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <cwctype>


namespace rush::cui {
   std::ostream& setfg(std::ostream& os, bool) {
      return os;
   }

   std::ostream& setbg(std::ostream& os, bool) {
      return os;
   }
}


namespace tc = tclr;

std::string& ltrim(std::string& str, std::string const& delim = "\t\n\v\r\f ") {
   str.erase(0, str.find_first_not_of(delim));
   return str;
}

std::string& rtrim(std::string& str, std::string const& delim = "\t\n\v\r\f ") {
   str.erase(str.find_last_not_of(delim) + 1);
   return str;
}

#define RUPL_TITLE(VERSION) "Rush REPL " VERSION

int main() {
   auto opts = cxxopts::Options { "rupl", RUPL_TITLE(RUSH_VERSION) };
   opts.add_options()
      ("h,help", "Print usage information.")
      ("version", "Print version information.");

   while (true) {
      std::cout << tc::fg("rush", tc::magenta) << "> ";

      std::string input;
      std::getline(std::cin, input);
      if (input.empty()) continue;

      rtrim(input);
      if (input[0] == ':') {
         if (input == ":exit")
            break;
         else if (input == ":clear")
            std::cout << tc::clear;
         else if (input == ":help")
            std::cout << "Output help options and descriptions.." << std::endl;
      } else {

         if (input.back() == ':') {
            while (true) {
               std::cout << tc::fg("  ... ", tc::magenta);

               std::string additional;
               std::getline(std::cin, additional);
               input += "\n " + rtrim(additional);

               if (additional.empty() || (!std::iswspace(additional.front()) && additional.back() != ':'))
                  break;
            }
         }

         auto lxa = rush::lex(input);
         auto first = lxa.front();

         if (first.is_keyword()) {
            switch (first.keyword()) {
               case rush::keywords::var_:
               case rush::keywords::let_:
               case rush::keywords::if_:
               case rush::keywords::for_:
               case rush::keywords::while_:
               case rush::keywords::func_:
               case rush::keywords::class_:
               case rush::keywords::struct_: break;
               default: std::cout << " " << tc::fg(input.c_str(), tc::black, true) << std::endl;
            }
         } else {
            std::cout << " " << tc::fg(input.c_str(), tc::black, true) << std::endl;
         }
      }
   }
}
