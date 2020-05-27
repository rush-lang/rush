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

#ifndef RUSH_AST_TYPES_FUNCTION_HPP
#define RUSH_AST_TYPES_FUNCTION_HPP

#include "rush/ast/types/type.hpp"
#include "rush/ast/types/type_ref.hpp"
#include "rush/ast/types/type_resolver.hpp"
#include "rush/ast/types/builtin.hpp"
#include "rush/ast/decls/parameter.hpp"
#include "rush/ast/ptrns/pattern.hpp"

#include <memory>

namespace rush::ast {
   class function_type;

   template <>
	struct type_traits<function_type> {
		static constexpr ast::type_kind kind() noexcept {
			return ast::type_kind::function;
		}
	};

   class function_type : public ast::type {
   public:
      function_type(ast::type_ref return_type, ast::type_ref param_types)
         : _param_types { std::move(param_types) }
         , _return_type { std::move(return_type) } {}

      virtual ast::type_kind kind() const noexcept override {
         return ast::type_kind::function;
      }

      ast::type_ref return_type() const noexcept {
			return _return_type;
      }

      ast::type_ref const& parameter_types() const noexcept {
         return _param_types;
      }

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         v.visit_function_type(*this);
      }

   private:
      ast::type_ref _param_types;
      ast::type_ref _return_type;
   };
}

#endif // RUSH_AST_TYPES_FUNCTION_HPP
