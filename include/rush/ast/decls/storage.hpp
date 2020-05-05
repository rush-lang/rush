#pragma once

#ifndef RUSH_AST_DECLS_STORAGE_HPP
#define RUSH_AST_DECLS_STORAGE_HPP

#include "rush/ast/identifier.hpp"
#include "rush/ast/exprs/expression.hpp"
#include "rush/ast/exprs/identifier.hpp"
#include "rush/ast/decls/declaration.hpp"
#include "rush/ast/types/type.hpp"
#include "rush/ast/visitor.hpp"


namespace rush::ast {
	class storage_declaration : public declaration {
	public:
      virtual std::string_view name() const noexcept override {
         return _name;
      };

      virtual ast::type_ref type() const noexcept override {
         return resolve_type();
      }

		ast::expression* initializer() const noexcept {
			return _init.get();
		}

      bool is_initialized() const noexcept {
         return _init.operator bool();
      }

      virtual void attach(ast::node& parent, ast::context& context) override {
         if (_init) _init->attach(*this, context);
      }

      virtual void detach(ast::node& parent, ast::context& context) override {
         if (_init) _init->detach(*this, context);
      }

	protected:
		storage_declaration(
         std::string name,
         ast::type_ref type,
         std::unique_ptr<expression> init)
         : _name { std::move(name) }
         , _type { std::move(type) }
         , _init { std::move(init) } {}

	private:
      std::string _name;
      mutable ast::type_ref _type;
		std::unique_ptr<expression> _init;
      mutable bool _resolving_type = false;

      ast::type_ref resolve_type() const {
         if (_resolving_type)
            return _type = ast::types::circular_ref;

         _resolving_type = true;
         auto result_type = (_type == ast::types::undefined && initializer())
            ? initializer()->result_type()
            : _type;
         _resolving_type = false;

         return _type = (_type != ast::types::circular_ref)
               ? result_type
               : _type;
      }
	};
} // rush::ast


#include "rush/ast/decls/constant.hpp"
#include "rush/ast/decls/variable.hpp"

#endif // RUSH_AST_DECLS_STORAGE_HPP
