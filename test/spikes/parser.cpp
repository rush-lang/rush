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
#include "rush/lexer/lex.hpp"
#include "rush/parser/parse.hpp"
#include "rush/parser/dump.hpp"
#include "rush/ast/ast.hpp"
#include "rush/ast/iterator.hpp"
#include <iostream>

char const* sprog = R"(
import std.io;

func main(args: [string]):
   let msg = "hello, world!"
   println(msg)
)";

int main() {
	std::cout << sprog << std::endl;
	std::cout << "--------------------" << std::endl;
   auto ctx = rush::ast::context {};
   auto src = rush::source::from_string(sprog);
	rush::dump(rush::parse(src, ctx));
}
