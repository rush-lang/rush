#pragma once

#ifndef RUSH_AST_EXPRS_IDENTIFIER_HPP
#define RUSH_AST_EXPRS_IDENTIFIER_HPP

#include "rush/ast/exprs/expression.hpp"
#include "rush/ast/decls/declaration.hpp"
#include "rush/ast/identifier.hpp"
#include "rush/ast/visitor.hpp"

#include <string>
#include <variant>
#include <cassert>

namespace rush::ast {
	class identifier_expression;
	namespace exprs {
      std::unique_ptr<identifier_expression> identifier(ast::identifier ident);
      std::unique_ptr<identifier_expression> identifier(ast::identifier::resolver&);
   }
}

namespace rush::ast {
   class declaration;

	// \brief Represents the identifier of a variable, constant
   //        or function within an expression.
	class identifier_expression : public expression {
		struct factory_tag_t {};
		friend std::unique_ptr<identifier_expression> exprs::identifier(ast::identifier ident);
      friend std::unique_ptr<identifier_expression> exprs::identifier(ast::identifier::resolver&);


	public:
		identifier_expression(ast::identifier ident, factory_tag_t) noexcept
			: _ident { std::move(ident) } {}

      bool is_unresolved() const noexcept {
         return _ident.is_unresolved();
      }

		std::string_view name() const {
         return _ident.name();
		}

      virtual ast::type_ref result_type() const noexcept override {
         return _ident.type();
      };

      declaration const& declaration() const noexcept {
         return _ident.declaration();
      }

		using node::accept;
		virtual void accept(ast::visitor& v) const override {
			v.visit_identifier_expr(*this);
		}

      virtual void attach(ast::node&, ast::context& context) override {}
      virtual void detach(ast::node&, ast::context& context) override {}

	private:
   	ast::identifier _ident;
	};

	namespace exprs {
		inline std::unique_ptr<identifier_expression> identifier(ast::identifier ident) {
			return std::make_unique<identifier_expression>(std::move(ident), identifier_expression::factory_tag_t {});
		}

      inline std::unique_ptr<identifier_expression> identifier(ast::identifier::resolver& res) {
         return std::make_unique<identifier_expression>(ast::identifier { res }, identifier_expression::factory_tag_t {});
      }
	} // rush::ast::exprs
} // rush::ast

#endif // RUSH_AST_EXPRS_IDENTIFIER_HPP
