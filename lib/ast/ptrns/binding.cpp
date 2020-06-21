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
#include "rush/ast/visitor.hpp"
#include "rush/ast/decls/constant.hpp"
#include "rush/ast/decls/variable.hpp"
#include "rush/ast/decls/parameter.hpp"
#include "rush/ast/exprs/invoke.hpp"
#include "rush/ast/ptrns/binding.hpp"
#include "rush/ast/ptrns/list.hpp"

using namespace rush;
using namespace rush::ast;

class binding_kind_resolver : public ast::visitor {
public:
   binding_kind result() { return _result; }
   virtual void visit_ptrn_list(ast::pattern_list const& ptrn) override { if (auto p = ptrn.parent()) p->accept(*this); }
   virtual void visit_expr_list(ast::expression_list const& expr) override { if (auto p = expr.parent()) p->accept(*this); }
   virtual void visit_invoke_expr(ast::invoke_expression const&) override { _result = binding_kind::parameter; }
   virtual void visit_constant_decl(ast::constant_declaration const&) override { _result = binding_kind::initializer; }
   virtual void visit_variable_decl(ast::variable_declaration const&) override { _result = binding_kind::initializer; }
   virtual void visit_parameter_decl(ast::parameter_declaration const&) override { _result = binding_kind::default_value; }
private:
   binding_kind _result;
};

namespace rush::ast {
   binding_kind binding_pattern::resolve_binding_kind() const {
      return parent()
         ? rush::visit(*parent(), binding_kind_resolver {}).result()
         : binding_kind::unknown;
   }
}
