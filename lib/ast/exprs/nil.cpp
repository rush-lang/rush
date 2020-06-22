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
#include "rush/ast/exprs/nil.hpp"
#include "rush/ast/exprs/lambda.hpp"
#include "rush/ast/types/optional.hpp"
#include "rush/ast/ptrns/rest.hpp"
#include "rush/ast/ptrns/binding.hpp"
#include "rush/ast/ptrns/type_annotation.hpp"
#include "rush/ast/decls/function.hpp"
#include "rush/ast/stmts/result.hpp"
#include "rush/ast/exprs/binary.hpp"
#include "rush/ast/exprs/array.hpp"
#include "rush/ast/exprs/list.hpp"
#include "rush/ast/context.hpp"

using namespace rush;

class nil_type_resolver : public ast::visitor {
public:
   nil_type_resolver(ast::node const* nil, ast::context& context)
      : _result { ast::types::undefined }
      , _nil { nil }
      , _context { context } {}


   ast::type_ref result() const {
      return !_result.is<ast::optional_type>()
           ? _context.optional_type(_result)
           : _result;
   }

   virtual void visit_expr_list(ast::expression_list const& expr) override {
      if (auto p = expr.parent()) p->accept(*this);
   }

   virtual void visit_rest_ptrn(ast::rest_pattern const& ptrn) override {
      ptrn.pattern().accept(*this);
   }

   virtual void visit_binding_ptrn(ast::binding_pattern const& ptrn) override {
      ptrn.pattern().accept(*this);
   }

   virtual void visit_type_annotation_ptrn(ast::type_annotation_pattern const& ptrn) override {
      _result = ptrn.type();
   }

   virtual void visit_result_stmt(ast::result_statement const& stmt) override {
      auto it = ast::find_ancestor<ast::lambda_expression>(&stmt);
      if (it != ast::ancestor_iterator())
      { _result = it->return_type(); }
      else {
         auto it = ast::find_ancestor<ast::function_declaration>(&stmt);
         if (it != ast::ancestor_iterator()) _result = it->return_type();
      }
   }

   virtual void visit_literal_expr(ast::array_literal_expression const& expr) override {
      if (auto p = expr.parent()) {
         p->accept(*this);
         if (auto type = _result.as<ast::optional_type>()) {
            _result = type->underlying_type();
         }
         if (auto type = _result.as<ast::array_type>()) {
            _result = type->underlying_type();
         }
      }
   }

   virtual void visit_binary_expr(ast::binary_expression const& expr) override {
      if (_nil != &expr.left_operand()) {
         switch (expr.opkind()) {
         default: return;
         case ast::binary_operator::assignment:
            _result = expr.left_operand().result_type();
         }
      }
   }

private:
   ast::type_ref _result;
   ast::node const* _nil;
   ast::context& _context;
};

namespace rush::ast {
   ast::type_ref nil_expression::resolve_type() const {
      return parent()
           ? rush::visit(*parent(), nil_type_resolver { this, *context() }).result()
           : ast::types::undeclared;
   }
}
