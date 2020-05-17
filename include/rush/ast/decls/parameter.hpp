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

#ifndef RUSH_AST_PARAMETER_HPP
#define RUSH_AST_PARAMETER_HPP

#include "rush/ast/list.hpp"
#include "rush/ast/identifier.hpp"
#include "rush/ast/types/type_ref.hpp"
#include "rush/ast/decls/nominal.hpp"

namespace rush::ast {
   class parameter;
	using parameter_list = detail::basic_list<parameter>;

	class parameter : public nominal_declaration {
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

   protected:
      virtual void attached(ast::node*, ast::context&) override {}
      virtual void detached(ast::node*, ast::context&) override {}

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
