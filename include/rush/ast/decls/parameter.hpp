#pragma once

#ifndef RUSH_AST_PARAMETER_HPP
#define RUSH_AST_PARAMETER_HPP

#include "rush/ast/list.hpp"
#include "rush/ast/identifier.hpp"
#include "rush/ast/types/type.hpp"
#include "rush/ast/decls/declaration.hpp"

namespace rush::ast {
   class parameter;
	using parameter_list = detail::basic_list<parameter>;

	class parameter : public declaration {
   public:
      virtual std::string_view name() const override {
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

      inline std::unique_ptr<parameter_list> param_list() {
         return std::make_unique<parameter_list>();
      }

      inline std::unique_ptr<parameter_list> param_list(std::vector<std::unique_ptr<ast::parameter>> params) {
         return std::make_unique<parameter_list>(std::move(params));
      }
	} // rush::ast::decls
} // rush::ast


#endif // RUSH_AST_PARAMETER_HPP
