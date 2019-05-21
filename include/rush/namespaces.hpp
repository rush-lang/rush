#pragma once

#ifndef HEADER_RUSH_NAMESPACES_HPP
#define HEADER_RUSH_NAMESPACES_HPP

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

#endif // HEADER_RUSH_NAMESPACES_HPP
