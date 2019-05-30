#include "rush/ast/exprs/invocation.hpp"
#include "rush/ast/types/type.hpp"
#include "rush/ast/types/builtin.hpp"
#include "rush/ast/visitor.hpp"
#include "rush/ast/exprs/identifier.hpp"
#include "rush/ast/decls/function.hpp"
#include "rush/ast/decls/constant.hpp"
#include "rush/ast/decls/variable.hpp"

using namespace rush;

class invocation_result_type_resolver : public ast::visitor {
public:
   invocation_result_type_resolver()
      : _result { ast::types::undefined } {}

   ast::type_ref result() const noexcept {
      return _result;
   }

   virtual void visit_identifier_expr(ast::identifier_expression const& expr) override { expr.declaration().accept(*this); }
   virtual void visit_function_decl(ast::function_declaration const& decl) override { _result = decl.return_type(); }
   virtual void visit_constant_decl(ast::constant_declaration const& decl) override { _result = decl.type(); }
   virtual void visit_variable_decl(ast::variable_declaration const& decl) override { _result = decl.type(); }

private:
   ast::type_ref _result;
};

namespace rush::ast {
   ast::type_ref invocation_expression::result_type() const {
      auto v = invocation_result_type_resolver {};
      this->callable().accept(v);
      return v.result();
   }
}
