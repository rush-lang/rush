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
#include <catch2/catch.hpp>
#include "rush/ast/context.hpp"
#include "rush/ast/exprs/literal.hpp"
#include "rush/ast/types/integral.hpp"
#include "rush/ast/decls/variable.hpp"
#include "rush/ast/decls/constant.hpp"
#include "rush/ast/ptrns/named.hpp"

namespace ast = rush::ast;
namespace exprs = ast::exprs;
namespace decls = ast::decls;
namespace ptrns = ast::ptrns;

TEST_CASE( "rush::ast::constant_declaration", "[unit][ast]" ) {
   auto ctx = ast::context {};
	auto decl = decls::constant("abc", exprs::literal(1, ctx));

	REQUIRE( decl->names().begin()->name() == "abc" );
   REQUIRE( &decl->initializers().begin()->pattern() == &(*decl->names().begin()));
	REQUIRE( decl->initializers().begin()->expression().result_type() == ctx.int32_type() );
}

TEST_CASE( "rush::ast::variable_declaration", "[unit][ast]" ) {
   auto ctx = ast::context {};
	auto decl = decls::variable("def", exprs::literal(1, ctx));

	REQUIRE( decl->names().begin()->name() == "def" );
   REQUIRE( &decl->initializers().begin()->pattern() == &(*decl->names().begin()));
	REQUIRE( decl->initializers().begin()->expression().result_type() == ctx.int32_type() );
}
