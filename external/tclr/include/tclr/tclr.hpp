#pragma once

#ifndef TCLR_TCLR_HPP
#define TCLR_TCLR_HPP

#include <string>

namespace tclr {
   enum color_t {
      black,
      red,
      green,
      yellow,
      blue,
      magenta,
      cyan,
      white,
      faint,
   };

   char const* const reset = "\033[0m";
   char const* const clear = "\033[2J\033[1;1H";

   inline std::string fg(char const* str, color_t col, bool bold = false) {
      std::string result;
      switch (col) {
         case black:    result = "\x1B[30"; break;
         case red:      result = "\x1B[31"; break;
         case green:    result = "\x1B[32"; break;
         case yellow:   result = "\x1B[33"; break;
         case blue:     result = "\x1B[34"; break;
         case magenta:  result = "\x1B[35"; break;
         case cyan:     result = "\x1B[36"; break;
         case white:    result = "\x1B[37"; break;
         case faint:    result = "\x1B[02"; break;
         default:       return std::move(result);
      }

      if (bold) result += ";1";

      result += "m";
      result += str;
      result += reset;
      return std::move(result);
   }

   inline std::string bg(char const* str, color_t col) {
      return str;
   }
} // tclr

#endif // TCLR_TCLR_HPP
