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
#include "catch2/catch.hpp"
#include "rush/parser/parse.hpp"
#include "rush/ast/exprs/binary.hpp"
#include "rush/ast/exprs/literal.hpp"
#include "rush/ast/types/integral.hpp"
#include "rush/ast/visitor.hpp"

#include <string>

namespace ast = rush::ast;
namespace exprs = ast::exprs;

bool valid_parse(std::string input, std::unique_ptr<ast::node> root) {
   auto ctx = ast::context {};
	auto ast = rush::parse(input, ctx);
	return true;
}

TEST_CASE("rush::parse", "[unit][parser]") {
	// CHECK( valid_parse("let i = 0", decls::constant("i", ctx, exprs::literal(0))));
	// CHECK( valid_parse("let d = 0.1", decls::constant("d", ieee64_type, exprs::literal(0))));
	// CHECK( valid_parse("let s = \"abc\"", decls::constant("s", string_type, exprs::literal(0))));
}

TEST_CASE( "rush::parse (expressions)", "[unit][parser]" ) {
   auto ctx = ast::context {};

	// auto ast = rush::parse("123");
	SECTION( "integer literals" ) {
		CHECK( valid_parse("0", exprs::literal(0, ctx)) );
		CHECK( valid_parse("1", exprs::literal(1, ctx)) );
		CHECK( valid_parse("9", exprs::literal(9, ctx)) );
		CHECK( valid_parse("123", exprs::literal(9, ctx)) );

		CHECK( valid_parse("345u", exprs::literal(9, ctx)) );
		CHECK( valid_parse("645l", exprs::literal(9, ctx)) );
		CHECK( valid_parse("645ul", exprs::literal(9, ctx)) );
	}

	SECTION( "binary expressions" ) {
		CHECK( valid_parse("1 + 1",
			exprs::addition(
				exprs::literal(1, ctx),
				exprs::literal(1, ctx)
			)));

		CHECK( valid_parse("2 - 1",
			exprs::subtraction(
				exprs::literal(1, ctx),
				exprs::literal(1, ctx)
			)));

		CHECK( valid_parse("x * 2",
			exprs::multiplication(
				exprs::literal(1, ctx),
				exprs::literal(1, ctx)
			)));

		CHECK( valid_parse("x / y",
			exprs::division(
				exprs::literal(1, ctx),
				exprs::literal(1, ctx)
			)));
	}
}
