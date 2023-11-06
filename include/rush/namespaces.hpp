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

#ifndef RUSH_NAMESPACES_HPP
#define RUSH_NAMESPACES_HPP

// forward declarations.
namespace rush {
	namespace tokens {}
	namespace symbols {}
	namespace keywords {}

	namespace ast {}
	namespace ast::exprs {}
	namespace ast::decls {}
	namespace ast::stmts {}
   namespace ast::types {}

	namespace sema {}
}

/// contains the set of namespaces defined within rush,
/// flattened within a single scope; this is for convenience
/// purposes. rather than fully qualifying each symbol with
/// it's structured namespace, you can simply declare that
/// you're using rush::namespaces at the top of your source file.
namespace rush::namespaces {
	namespace tokens = ::rush::tokens;
	namespace symbols = ::rush::symbols;
	namespace keywords = ::rush::keywords;

	namespace ast = ::rush::ast;
	namespace exprs = ::rush::ast::exprs;
	namespace decls = ::rush::ast::decls;
	namespace stmts = ::rush::ast::stmts;
   namespace types = ::rush::ast::types;

	namespace sema = ::rush::sema;
}

#endif // RUSH_NAMESPACES_HPP
