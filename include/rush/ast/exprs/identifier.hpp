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
      std::unique_ptr<identifier_expression> identifier(ast::identifier);
      std::unique_ptr<identifier_expression> identifier(ast::declaration::resolver*);
   }
}

namespace rush::ast {
   class declaration;

	// \brief Represents the identifier of a variable, constant
   //        or function within an expression.
	class identifier_expression : public expression {
		struct factory_tag_t {};
		friend std::unique_ptr<identifier_expression> exprs::identifier(ast::identifier);
      friend std::unique_ptr<identifier_expression> exprs::identifier(ast::declaration::resolver*);

      using base_resolver_callback_t = declaration::resolver_callback_t;

      class resolver_callback_t : public base_resolver_callback_t {
      public:
         resolver_callback_t(identifier_expression* self)
            : _self { self } {}

         virtual void operator ()(ast::declaration const* decl) const override {
            _self->_ident = decl->identifier();
         }

      private:
         identifier_expression* _self;
      };

	public:
      using resolver = ast::declaration::resolver;

		identifier_expression(ast::identifier ident, factory_tag_t) noexcept
			: _ident { ident } {}

      identifier_expression(resolver* resolver, factory_tag_t) noexcept
         : _ident { resolver_callback_t { this } } { resolver->listen(&std::get<resolver_callback_t>(_ident)); }


      bool is_unresolved() const noexcept {
         return std::holds_alternative<resolver_callback_t>(_ident);
      }

		std::string name() const {
         assert(!is_unresolved() && "attempted to get the 'name' of an unresolved identifier.");
         return std::get<ast::identifier>(_ident).name();
		}

      virtual ast::type_ref result_type() const noexcept override {
         assert(!is_unresolved() && "attempted to get the 'type' of an unresolved identifier.");
         return std::get<ast::identifier>(_ident).type();
      };

      declaration const& declaration() const noexcept {
         assert(!is_unresolved() && "attempted to get the 'declaration' of an unresolved identifier.");
         return std::get<ast::identifier>(_ident).declaration();
      }

		using node::accept;
		virtual void accept(ast::visitor& v) const override {
			v.visit_identifier_expr(*this);
		}

	private:
   	std::variant<
         ast::identifier,
         resolver_callback_t> _ident;
	};

	namespace exprs {
		inline std::unique_ptr<identifier_expression> identifier(ast::identifier ident) {
			return std::make_unique<identifier_expression>(ident, identifier_expression::factory_tag_t {});
		}

      inline std::unique_ptr<identifier_expression> identifier(ast::declaration::resolver* res) {
         return std::make_unique<identifier_expression>(res, identifier_expression::factory_tag_t {});
      }
	} // rush::ast::exprs
} // rush::ast

#endif // RUSH_AST_EXPRS_IDENTIFIER_HPP
