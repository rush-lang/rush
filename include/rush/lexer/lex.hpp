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

#ifndef RUSH_LEXER_LEX_HPP
#define RUSH_LEXER_LEX_HPP

#include "rush/lexer/token.hpp"
#include "rush/lexer/analysis.hpp"

#include <iostream>
#include <string_view>

namespace rush {
   class source;
	struct lexer_options {

	};

   lexical_analysis lex(std::istream&, lexer_options const& = {});
	lexical_analysis lex(std::string_view, lexer_options const& = {});
	lexical_analysis lex(std::istream&, std::string_view id, lexer_options const& = {});
	lexical_analysis lex(std::string_view, std::string_view id, lexer_options const& = {});
   lexical_analysis lex(rush::source const&, lexer_options const& = {});
}

#endif // RUSH_LEXER_LEX_HPP
