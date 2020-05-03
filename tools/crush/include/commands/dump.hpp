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
