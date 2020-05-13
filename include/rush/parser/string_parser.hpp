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
