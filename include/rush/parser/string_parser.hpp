#pragma once

#ifndef RUSH_PARSER_STRING_PARSER_HPP
#define RUSH_PARSER_STRING_PARSER_HPP

#include "rush/parser/parser.hpp"

namespace rush {
   //! \brief Parses the contents of string literal tokens.
   //         If string template punctuation is found within the string,
   //         the tokens between the punctuation are extracted and fed
   //         to the primary parser for evaluation.
   //         The result is a string template or, in the case of no
   //         template punctuation, a string literal expression.
   class string_parser {
   public:
      string_parser(rush::parser& primary)
         : _primary { primary } {}

   private:
      rush::parser& _primary;
   };
}

#endif // RUSH_PARSER_STRING_PARSER_HPP
