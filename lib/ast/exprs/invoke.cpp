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
#include "rush/ast/exprs/invoke.hpp"
#include "rush/ast/types/type.hpp"
#include "rush/ast/visitor.hpp"
#include "rush/ast/expressions.hpp"
#include "rush/ast/declarations.hpp"
#include "rush/ast/patterns.hpp"

using namespace rush;

class invoke_result_type_resolver : public ast::visitor {
public:
   invoke_result_type_resolver()
      : _result { ast::types::undefined } {}

   ast::type_ref result() const noexcept { return _result; }

   virtual void visit_user_type(ast::user_type const& type) override { _result = type; }
   virtual void visit_function_type(ast::function_type const& type) override { _result = type.return_type(); }
   virtual void visit_builtin_error_type(ast::builtin_error_type const& type) override { _result = type; }
private:
   ast::type_ref _result;
};

namespace rush::ast {
   ast::type_ref invoke_expression::result_type() const {
      auto v = invoke_result_type_resolver {};
      return rush::visit(callable().result_type(), v).result();
   }
}
