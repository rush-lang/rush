#pragma once

#ifndef RUSH_AST_DECLS_STORAGE_HPP
#define RUSH_AST_DECLS_STORAGE_HPP

#include "rush/ast/identifier.hpp"
#include "rush/ast/exprs/expression.hpp"
#include "rush/ast/exprs/identifier.hpp"
#include "rush/ast/decls/declaration.hpp"
#include "rush/ast/types/type.hpp"
#include "rush/ast/visitor.hpp"

#include <string>

namespace rush::ast {
	class storage_declaration : public declaration {
	public:
      virtual std::string name() const noexcept override {
         return _name;
      };

      virtual ast::type_ref type() const noexcept override {
         return resolve_type();
      }

		expression* initializer() const noexcept {
			return _init.get();
		}

      bool is_initialized() const noexcept {
         return _init.operator bool();
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

      ast::type_ref resolve_type() const {
         return (_type == ast::types::undefined && initializer())
            ? _type = initializer()->result_type()
            : _type;
      }
	};
} // rush::ast


#include "rush/ast/decls/constant.hpp"
#include "rush/ast/decls/variable.hpp"

#endif // RUSH_AST_DECLS_STORAGE_HPP
