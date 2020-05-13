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
#pragma once

#ifndef CRUSH_COMMANDS_DUMP_HPP
#define CRUSH_COMMANDS_DUMP_HPP

#include "rush/core/source.hpp"
#include "rush/lexer/lex.hpp"
#include "rush/lexer/dump.hpp"
#include "rush/parser/dump.hpp"

#include <iostream>
#include <vector>

namespace crush::commands {

   void dump_llvm_ir(std::vector<rush::source>& srcs) {
      std::cout << "error: not yet supported." << std::endl;
   }

   void dump_lex(std::vector<rush::source>& srcs) {
      std::size_t page = 0;
      for (auto& src : srcs) {
         if (page > 0) {
            std::cout << "===================================" << std::endl;
            std::cout << std::endl;
         }

         std::cout << src.id() << " : lexical analysis ----------"  << std::endl;
         rush::dump(rush::lex(src));
         std::cout << std::endl;
         ++page;
      }
   }

   void dump_parse(std::vector<rush::source>& srcs) {
      std::size_t page = 0;
      for (auto& src : srcs) {
         if (page > 0) {
            std::cout << "===================================" << std::endl;
            std::cout << std::endl;
         }

         std::cout << src.id() << " : syntax analysis ----------"  << std::endl;
         rush::dump(src);
         std::cout << std::endl;
         ++page;
      }
   }
}

#endif // CRUSH_COMMANDS_DUMP_HPP
