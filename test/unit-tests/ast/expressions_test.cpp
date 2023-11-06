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
#include <catch2/catch_all.hpp>
#include "rush/ast/context.hpp"
#include "rush/ast/types/builtin.hpp"
#include "rush/ast/types/integral.hpp"
#include "rush/ast/types/floating.hpp"
#include "rush/ast/exprs/literal.hpp"

namespace ast = rush::ast;
namespace exprs = ast::exprs;

TEST_CASE( "rush::literal_expression (boolean)", "[unit][ast]" ) {
   auto ctx = ast::context {};
	auto expr = exprs::literal(true, ctx);
}

// TEST_CASE( "rush::literal_expression (string)", "[unit][ast]" ) {
// 	auto expr = exprs::literal("hello", string_type);
// }

TEST_CASE( "rush::literal_expression (integral)", "[unit][ast]" ) {
   auto ctx = ast::context {};
	auto expr = exprs::literal(1, ctx);
}

TEST_CASE( "rush::literal_expression (floating)", "[unit][ast]" ) {
   auto ctx = ast::context {};
	auto expr = exprs::literal(1.3, ctx);
}
