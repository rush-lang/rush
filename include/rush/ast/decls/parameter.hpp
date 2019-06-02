#pragma once

#ifndef RUSH_AST_PARAMETER_HPP
#define RUSH_AST_PARAMETER_HPP

#include "rush/ast/decls/declaration.hpp"
#include "rush/ast/identifier.hpp"
#include "rush/ast/types/type.hpp"

namespace rush::ast {
	class parameter : public declaration {
   public:
      virtual std::string name() const override {
         return _name;
      }

      virtual ast::type_ref type() const noexcept override {
         return _type;
      }

		virtual declaration_kind kind() const noexcept override {
         return ast::declaration_kind::parameter;
      }

      parameter(
         std::string name,
         ast::type_ref type)
         : _name { std::move(name) }
         , _type { std::move(type) } {}

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         v.visit_parameter_decl(*this);
      }

      virtual void attach(ast::node&, ast::context& context) override {}
      virtual void detach(ast::node&, ast::context& context) override {}

   private:
      std::string _name;
      ast::type_ref _type;
   };

	namespace decls {
		inline std::unique_ptr<ast::parameter> param(std::string name, ast::type_ref type) {
         return std::make_unique<parameter>(std::move(name), std::move(type));
      }
	}
} // rush::ast

#endif // RUSH_AST_PARAMETER_HPP
