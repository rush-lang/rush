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

   // ultimately we're trying to find this.
   virtual void visit_function_type(ast::function_type const& type) override { _result = type.return_type(); }

   virtual void visit_named_ptrn(ast::named_pattern const& ptrn) override { ptrn.type().accept(*this); }
   virtual void visit_function_decl(ast::function_declaration const& decl) override { decl.type().accept(*this); }

   virtual void visit_unary_expr(ast::unary_expression const& expr) override { expr.result_type().accept(*this); }
   virtual void visit_binary_expr(ast::binary_expression const& expr) override { expr.result_type().accept(*this); }
   virtual void visit_ternary_expr(ast::ternary_expression const& expr) override { expr.result_type().accept(*this); }
   virtual void visit_lambda_expr(ast::lambda_expression const& expr) override { expr.result_type().accept(*this); }
   virtual void visit_invoke_expr(ast::invoke_expression const& expr) override { expr.result_type().accept(*this); }

   virtual void visit_identifier_expr(ast::identifier_expression const& expr) override {
      _result = !expr.is_unresolved()
              ? rush::visit(expr.declaration(), *this).result()
              : ast::types::undeclared;
   }

   virtual void visit_member_access_expr(ast::member_access_expression const& expr) override {
      _result = !expr.is_unresolved()
              ? rush::visit(expr.result_type(), *this).result()
              : ast::types::undeclared;
   }

private:
   ast::type_ref _result;
};

namespace rush::ast {
   ast::type_ref invoke_expression::result_type() const {
      auto v = invoke_result_type_resolver {};
      return rush::visit(callable(), v).result();
   }
}
