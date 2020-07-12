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
#include "rush/lexer/result.hpp"
#include "rush/lexer/dump.hpp"
#include "rush/lexer/lex.hpp"

#include <iostream>

namespace rush {
	void dump(lexical_analysis const& input) {
		return dump(input, std::cout);
	}

	void dump(lexical_analysis const& input, std::ostream& out) {
      rush::source const* src = nullptr;
		for (auto& tok : input) {
         if (src != &tok.source()) {
            src = &tok.source();
            if (src != nullptr)
               out << "--- source: \'" <<  src->id() << '\'' << std::endl;
         }

         out << rush::debug_string(tok)
             << std::endl;
      }
	}
} // rush
