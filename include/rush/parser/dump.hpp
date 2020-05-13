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

#ifndef RUSH_PARSER_DUMP_HPP
#define RUSH_PARSER_DUMP_HPP

#include <iostream>
#include <string>

namespace rush::ast {
   class node;
}

namespace rush {
   class source;
   class syntax_analysis;
   class parser_options;

   void dump(ast::node const&, std::ostream& = std::cout);
   void dump(std::string const&, std::ostream& = std::cout);
   void dump(rush::source const&, std::ostream& = std::cout);
   void dump(std::istream&, std::ostream& = std::cout);

   void dump(std::string const&, parser_options const&, std::ostream& = std::cout);
   void dump(rush::source const&, parser_options const&, std::ostream& = std::cout);
   void dump(std::istream&, parser_options const&, std::ostream& = std::cout);
   void dump(rush::syntax_analysis const&, std::ostream& = std::cout);
} // rush

#endif // RUSH_PARSER_DUMP_HPP
