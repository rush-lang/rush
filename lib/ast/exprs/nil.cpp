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
#include "rush/ast/types/optional.hpp"
#include "rush/ast/ptrns/type_annotation.hpp"
#include "rush/ast/decls/variable.hpp"
#include "rush/ast/decls/constant.hpp"
#include "rush/ast/exprs/binary.hpp"
#include "rush/ast/context.hpp"

using namespace rush;

class nil_type_resolver : public ast::visitor {
public:
   nil_type_resolver(ast::node const* nil, ast::context& context)
      : _result { ast::types::undefined }
      , _nil { nil }
      , _context { context } {}


   ast::type_ref result() const {
      struct is_optional_type_checker : ast::visitor {
         bool result = false;
         virtual void visit_optional_type(ast::optional_type const&) { result = true; }
      };

      return !rush::visit(_result, is_optional_type_checker {}).result
           ? _context.optional_type(_result)
           : _result;
   }

   virtual void visit_type_annotation_ptrn(ast::type_annotation_pattern const& ptrn) override {
      _result = ptrn.type();
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
   virtual void visit_variable_decl(ast::variable_declaration const& decl) override { rush::visit(decl.pattern(), *this); }
   virtual void visit_constant_decl(ast::constant_declaration const& decl) override { rush::visit(decl.pattern(), *this); }

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
