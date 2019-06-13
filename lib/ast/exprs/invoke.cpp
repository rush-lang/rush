#include "rush/ast/exprs/invoke.hpp"
#include "rush/ast/types/type.hpp"
#include "rush/ast/types/builtin.hpp"
#include "rush/ast/visitor.hpp"
#include "rush/ast/expressions.hpp"
#include "rush/ast/declarations.hpp"

using namespace rush;

class invoke_result_type_resolver : public ast::visitor {
public:
   invoke_result_type_resolver()
      : _result { ast::types::undefined } {}

   ast::type_ref result() const noexcept {
      return _result;
   }

   virtual void visit_function_type(ast::function_type const& type) override {
      // ultimately we're trying to find this.
      _result = type.return_type();
   }

   virtual void visit_constant_decl(ast::constant_declaration const& decl) override { decl.type().accept(*this); }
   virtual void visit_variable_decl(ast::variable_declaration const& decl) override { decl.type().accept(*this); }
   virtual void visit_function_decl(ast::function_declaration const& decl) override { decl.type().accept(*this); }

   virtual void visit_unary_expr(ast::unary_expression const& expr) override { expr.result_type().accept(*this); }
   virtual void visit_binary_expr(ast::binary_expression const& expr) override { expr.result_type().accept(*this); }
   virtual void visit_ternary_expr(ast::ternary_expression const& expr) override { expr.result_type().accept(*this); }
   virtual void visit_invoke_expr(ast::invoke_expression const& expr) override { expr.result_type().accept(*this); }

   virtual void visit_identifier_expr(ast::identifier_expression const& expr) override {
      if (!expr.is_unresolved()) { expr.declaration().accept(*this); return; }
      _result = undeclared_identifier_error_type;
   }

private:
   static const ast::builtin_error_type undeclared_identifier_error_type;
   ast::type_ref _result;
};

const ast::builtin_error_type invoke_result_type_resolver::
   undeclared_identifier_error_type =
   ast::types::error_type("undeclared identifier.");

namespace rush::ast {
   ast::type_ref invoke_expression::result_type() const {
      auto v = invoke_result_type_resolver {};
      return rush::visit(callable(), v).result();
   }
}
